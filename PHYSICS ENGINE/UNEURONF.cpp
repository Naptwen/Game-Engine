#include "UNEURONF.h"
namespace UNF {
	float NEURON_BLOCKF::ACTIVATION(float y, const char ACT_TYPE)
	{
		switch (ACT_TYPE)
		{
		case ELU:
			return ((0 < y) ? 0.01 * (float)(exp(y) - 1) : y);
		case ReLU:
			return ((0 < y) ? y : 0);
		case Leaky_ReLU:
			return ((0 < y) ? 0.01 * y : y);
		case Noisy_ReLU:
			return ((0 < y) ? 0 : y);
		case Soft_ReLU:
			return log(1 + exp(y));
		case SWISH:
			return y / (1 + exp(-y));
		case LOGISTIC:
			return 1 / (1 + exp(-y));
		case HYPERBOLIC:
			return (exp(y) - exp(-y)) / (exp(y) + exp(-y));
		case SIGMOID:
			return (exp(y) / (exp(y) + 1));
		default:
			break;
		}
		return y;
	}

	void NEURON_BLOCKF::ACT_FUN(UMATRIXF* TEMP, UMATRIXF* Z, const char act_type)
	{
		int _size = TEMP->m;
#pragma omp parallel for
		for (int i = 0; i < _size; i++)
		{
			TEMP->vectors[i] = ACTIVATION(Z->vectors[i], act_type);
		}
	}

	void NEURON_BLOCKF::E_A_CAL(float* E_A_LIST, float target[])
	{
		int num = this->LAYER_M_LIST[LAYER_M_LIST.size() - 1];
		int _n = this->TOTAL_A_LAYER.n;
#pragma omp parallel for
		for (int i = 0; i < num; i++)
		{
			if (this->ERROR_TYPE == CROSS)
				E_A_LIST[i] += -target[i] / this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n]
				+ (1 - target[i]) / (1 - this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n]);
			else
				E_A_LIST[i] += target[i] - this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n];
		}

	}

	void NEURON_BLOCKF::A_Z_CAL(float* A_Z_LIST, unsigned int k)
	{
		int _n = this->TOTAL_A_LAYER.n;
		const char fn_TYPE = this->ACT_TYPE_LIST[k];
		int _size = LAYER_M_LIST[k];
#pragma omp parallel for private(fn_TYPE)
		for (int i = 0; i < _size; i++)
		{
			float x = this->TOTAL_Z_LAYER.vectors[k + i * _n];
			(fn_TYPE == ReLU) ? A_Z_LIST[i] = 1 :
				(fn_TYPE == Soft_ReLU) ? A_Z_LIST[i] = 1 / (1 + exp(x)) :
				(fn_TYPE == Noisy_ReLU) ? A_Z_LIST[i] = ((0 < x) ? 0 : 1) :
				(fn_TYPE == SWISH) ? A_Z_LIST[i] = (x - 1) / (exp(x) + 1) - x / ((exp(x) + 1) * (exp(x) + 1)) + 1 :
				(fn_TYPE == LOGISTIC) ? A_Z_LIST[i] = exp(x) / ((exp(x) + 1) * (exp(x) + 1)) :
				(fn_TYPE == HYPERBOLIC) ? A_Z_LIST[i] = 4 * exp(2 * x) / ((exp(2 * x) + 1) * (exp(2 * x) + 1)) :
				(fn_TYPE == SIGMOID) ? A_Z_LIST[i] = (exp(x) / (exp(x) + 1)) * (1 - (exp(x) / (exp(x) + 1))) : A_Z_LIST[i] = 1;
		}
	}

	void NEURON_BLOCKF::Z_W_CAL(float* Z_W_LIST, unsigned int k)
	{
		int _n = this->TOTAL_A_LAYER.n;
		int _k = k - 1;
		int _size = this->LAYER_M_LIST[_k];

#pragma omp parallel for 
		for (int i = 0; i < _size; i++)
			Z_W_LIST[i] = this->TOTAL_A_LAYER.vectors[_k + i * _n];

	}

	void NEURON_BLOCKF::TOTAL_E(float* E_TOTAL_LIST, float* E_A_LIST, float* A_Z_LIST, unsigned int e_size, unsigned int k)
	{
		int _m = this->TOTAL_W_LAYER.m;
		int _n = this->TOTAL_W_LAYER.n;
		int _e_size = e_size;
		int _k = k;
#pragma omp parallel for collapse(2) 
		for (int i = 0; i < _m; i++)
		{
			float sum = 0;
			for (int j = 0; j < _e_size; j++)
			{
				sum += this->TOTAL_W_LAYER.vectors[_k + j * _n]
					* E_A_LIST[j]
					* A_Z_LIST[j];
			}
			E_TOTAL_LIST[i] += sum;
		}
	}

	void  NEURON_BLOCKF::UPDATE_WEIGHT(float* ERROR_LIST, float* A_Z_LIST, float* Z_W_LIST, unsigned int  k)
	{
		int _m = this->TOTAL_W_LAYER.m;
		int _n = this->TOTAL_W_LAYER.n;
#pragma omp parallel for private(_n)
		for (int i = 0; i < _m; i++)
		{
			if (this->BTYPE == MOMEN)
			{
				float var = this->vr * this->TOTAL_M_LAYER.vectors[k + i * _n]
					+ this->lr * ERROR_LIST[i] * A_Z_LIST[i] * Z_W_LIST[i];

				this->TOTAL_M_LAYER.vectors[k + i * _n] = var;
				this->TOTAL_W_LAYER.vectors[k + i * _n] -= var;
			}
			else if (this->BTYPE == STOCHASTIC)
			{
				float var = this->vr * this->TOTAL_M_LAYER.vectors[k + i * _n]
					- +(1 - this->vr) * ERROR_LIST[i] * A_Z_LIST[i] * Z_W_LIST[i];

				this->TOTAL_M_LAYER.vectors[k + i * _n] = var;
				this->TOTAL_W_LAYER.vectors[k + i * _n] -= this->lr * var;
			}
			else if (this->BTYPE == NEST)
			{
				float var = this->vr * this->TOTAL_M_LAYER.vectors[k - 1 + i * _n]
					- this->lr * (ERROR_LIST[i] - this->vr * this->TOTAL_M_LAYER.vectors[k - 1 + i * _n]) * A_Z_LIST[i] * Z_W_LIST[i];

				this->TOTAL_M_LAYER.vectors[k + i * _n] = var;
				this->TOTAL_W_LAYER.vectors[k + i * _n] -= var;
			}
			else
			{
				this->TOTAL_W_LAYER.vectors[k + i * _n] +=
					this->lr * ERROR_LIST[i] * A_Z_LIST[i] * Z_W_LIST[i];
			}
		}
	}

	const char* NEURON_BLOCKF::TYPE2STR(const char TYPE)
	{
		switch (TYPE)
		{
		case ReLU:
			return("ReLU\n");
			break;
		case Soft_ReLU:
			return("Soft ReLU\n");
			break;
		case Noisy_ReLU:
			return("Noisy_ReLU\n");
			break;
		case Leaky_ReLU:
			return("Leaky_ReLU\n");
			break;
		case ELU:
			return("ELU\n");
			break;
		case SWISH:
			return("SWISH by google\n");
			break;
		case LOGISTIC:
			return("LOGISTIC\n");
			break;
		case HYPERBOLIC:
			return("HYPERBOLIC\n");
			break;
		case MOMEN:
			return("MOMENTUM\n");
			break;
		case NESTEROV:
			return("NESTROV\n");
			break;
		default:
			return("NONE\n");
			break;
		}
	}

	void NEURON_BLOCKF::operator = (const NEURON_BLOCKF COPY)
	{
		this->TOTAL_B_LAYER = COPY.TOTAL_B_LAYER;
		this->TOTAL_W_LAYER = COPY.TOTAL_W_LAYER;
		this->TOTAL_Z_LAYER = COPY.TOTAL_Z_LAYER;
		this->TOTAL_A_LAYER = COPY.TOTAL_A_LAYER;
		this->TOTAL_M_LAYER = COPY.TOTAL_A_LAYER;
		this->ACT_TYPE_LIST = COPY.ACT_TYPE_LIST;
		this->LAYER_M_LIST = COPY.LAYER_M_LIST;
		this->vr = COPY.vr;
		this->lr = COPY.lr;
		this->BTYPE = COPY.BTYPE;
		this->ERROR_TYPE = COPY.ERROR_TYPE;
	}

	void NEURON_BLOCKF::ADD_LAYER(unsigned int m, const char activation)
	{
		//--------------------------------------------
		int m_max = TOTAL_W_LAYER.m;
		if (TOTAL_W_LAYER.m < m)
			m_max = m;
		int n_max = TOTAL_W_LAYER.n + 1;
		//--------------------------------------------
		TOTAL_W_LAYER = UMATRIXF(m_max, n_max);
		TOTAL_Z_LAYER = UMATRIXF(m_max, n_max);
		TOTAL_A_LAYER = UMATRIXF(m_max, n_max);
		TOTAL_M_LAYER = UMATRIXF(m_max, n_max);
		TOTAL_B_LAYER = UMATRIXF(m_max, n_max);

		ACT_TYPE_LIST.push_back(activation);
		LAYER_M_LIST.push_back(m);
	}

	void NEURON_BLOCKF::W_INIT(const char TYPE)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		int size = TOTAL_W_LAYER.m * TOTAL_W_LAYER.n;
		int n_size = TOTAL_W_LAYER.n;
#pragma omp parallel for for private(n_size)
		for (int i = 0; i < size; i++)
		{
			unsigned int x = i % n_size;
			unsigned int y = i / n_size;
			if (x != 0 && y < LAYER_M_LIST[x])
			{
				if (TYPE == XAVIER)
				{
					double min = -sqrt(6 / (double)(LAYER_M_LIST[x] + LAYER_M_LIST[x - 1]));
					double max = sqrt(6 / (double)(LAYER_M_LIST[x] + LAYER_M_LIST[x - 1]));
					std::uniform_real_distribution<double> rng(min, max);
					TOTAL_W_LAYER.vectors[i] = (float)rng(gen);
				}
				else if (TYPE == HENORM)
				{
					double min = -sqrt(6 / (double)(LAYER_M_LIST[x - 1]));
					double max = sqrt(6 / (double)(LAYER_M_LIST[x - 1]));
					std::uniform_real_distribution<double> rng(min, max);
					TOTAL_W_LAYER.vectors[i] = (float)rng(gen);
				}
				else
				{
					std::uniform_real_distribution<double> rng(0, 1);
					TOTAL_W_LAYER.vectors[i] = (float)rng(gen);
				}

			}
		}
	}

	void NEURON_BLOCKF::INPUT(float input[], unsigned int size)
	{
		int N = TOTAL_A_LAYER.n;
		int M = TOTAL_A_LAYER.m;
		int MN_size = N * M;
		int data_size = size;
#pragma omp parallel for 
		for (int i = 0; i < MN_size; i++)
		{
			int x = i % N;
			int y = i / N;
			if (x == 0)
			{
				if (y < data_size)
					TOTAL_A_LAYER.vectors[i] = input[y];
				else if (y == M - 1)
					TOTAL_A_LAYER.vectors[i] = 1;
				else
					TOTAL_A_LAYER.vectors[i] = 0;
				TOTAL_W_LAYER.vectors[i] = 1;
			}
		}
	}

	void NEURON_BLOCKF::OUTPUT(float* output, unsigned int size)
	{
		int N = TOTAL_A_LAYER.n;
		int data_size = size;
#pragma omp parallel for 
		for (int i = 0; i < data_size; i++)
			output[i] = TOTAL_A_LAYER.vectors[N - 1 + N * i];
	}

	void NEURON_BLOCKF::CPU_RUN(bool NOMALIZATION)
	{
		int _m = TOTAL_W_LAYER.m;
		int _n = TOTAL_W_LAYER.n;
		UMATRIXF* TEMP;
		UMATRIXF* RW;
		UMATRIXF* RB;
		UMATRIXF* XX;
		UMATRIXF* XB;
		UMATRIXF* XBA;
		for (int i = 1; i < _n; i++)
		{
			TEMP = ROWCUT(&this->TOTAL_A_LAYER, i - 1, i - 1);
			RW = ROWCUT(&this->TOTAL_W_LAYER, i, i);
			RB = ROWCUT(&this->TOTAL_B_LAYER, i, i);
			XX = COPY_ROW(RW, 0, _m - 1);
			XB = MATRIXADDROW(XX, RB);
			XBA = (*XB) * (*TEMP);

			int _size = this->LAYER_M_LIST[i];

#pragma omp parallel for 
			for (int j = 0; j < _size; j++)
				this->TOTAL_Z_LAYER.vectors[i + j * _n] = XBA->vectors[j];

			ACT_FUN(TEMP, XBA, ACT_TYPE_LIST[i]);

			if (NOMALIZATION && i != _n - 1)
				Mean_Normalize(TEMP->vectors, this->LAYER_M_LIST[i]);

#pragma omp parallel for 
			for (int j = 0; j < _size; j++)
				this->TOTAL_A_LAYER.vectors[i + j * _n] = TEMP->vectors[j];

			std::free(TEMP->vectors);
			std::free(RW->vectors);
			std::free(RB->vectors);
			std::free(XX->vectors);
			std::free(XB->vectors);
			std::free(XBA->vectors);

			std::free(TEMP);
			std::free(RW);
			std::free(RB);
			std::free(XX);
			std::free(XB);
			std::free(XBA);
		}
	}

	void NEURON_BLOCKF::LEARN_SET(float vr_, float lr_, const char ERROR_TYPE, const char B_TYPE)
	{
		this->vr = vr_;
		this->lr = lr_;
		this->BTYPE = B_TYPE;
		if (ERROR_TYPE != CROSS)
			this->ERROR_TYPE = 1;
		else
			this->ERROR_TYPE = ERROR_TYPE;
	}

	double NEURON_BLOCKF::LOSS_CHECK(float* target)
	{
		int num = this->LAYER_M_LIST[LAYER_M_LIST.size() - 1];
		int _n = this->TOTAL_A_LAYER.n;
		double sum = 0;
		if (this->ERROR_TYPE == CROSS)
		{
#pragma omp parallel for reduction(-:sum) shared(num)
			for (int i = 0; i < num; i++)
				sum -= ((double)target[i] * log(this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n])
					+ (1 - (double)target[i]) * log(1 - this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n]));
			sum = sum / (double)num;
		}
		else
		{
#pragma omp parallel for reduction(+:sum) shared(num)
			for (int i = 0; i < num; i++)
				sum += ((double)target[i] - this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n])
				* ((double)target[i] - this->TOTAL_A_LAYER.vectors[_n - 1 + i * _n]);
			sum = sum / (2 * (double)num);
		}
		return sum;
	}

	void NEURON_BLOCKF::CPU_BACK_PROPA(float target[])
	{
		int _m = TOTAL_W_LAYER.m;
		int _n = TOTAL_W_LAYER.n;
		int _e = LAYER_M_LIST[LAYER_M_LIST.size() - 1];
		//1. 
		float* E_A_LIST = (float*)calloc(_e, sizeof(float));
		float* A_Z_LIST = (float*)calloc(_m, sizeof(float));
		float* Z_W_LIST = (float*)calloc(_m, sizeof(float));
		float* E_TOTAL_LIST = (float*)calloc(_m, sizeof(float));

		E_A_CAL(E_A_LIST, target);
		for (int k = _n - 1; k > 0; k--)
		{
			A_Z_CAL(A_Z_LIST, k);
			Z_W_CAL(Z_W_LIST, k);
			TOTAL_E(E_TOTAL_LIST, E_A_LIST, A_Z_LIST, _e, k);
			UPDATE_WEIGHT(E_A_LIST, A_Z_LIST, Z_W_LIST, k);
			//update the total error list
			_e = this->LAYER_M_LIST[k - 1];
			std::free(E_A_LIST);
			E_A_LIST = (float*)calloc(_e, sizeof(float));
			memcpy(E_A_LIST, E_TOTAL_LIST, sizeof(float) * _e);
		}
		std::free(E_A_LIST);
		std::free(A_Z_LIST);
		std::free(Z_W_LIST);
		std::free(E_TOTAL_LIST);
	}

	void NEURON_BLOCKF::pinfo()
	{
		printf("---------W--------------\n");
		this->TOTAL_W_LAYER.pinfo();
		printf("---------Z--------------\n");
		this->TOTAL_Z_LAYER.pinfo();
		printf("---------A--------------\n");
		this->TOTAL_A_LAYER.pinfo();
		printf("---------M--------------\n");
		this->TOTAL_M_LAYER.pinfo();
		printf("---------B--------------\n");
		this->TOTAL_B_LAYER.pinfo();
	}

	void NEURON_BLOCKF::NEURON_SAVE(const char* name)
	{

		std::ofstream out(name);
		out << this->vr << "," << this->lr << "," << int(this->BTYPE) << ",\n";
		out << this->TOTAL_W_LAYER.m << "," << this->TOTAL_W_LAYER.m << ",\n";

		for (unsigned int i = 0; i < this->LAYER_M_LIST.size(); i++)
			out << int(this->LAYER_M_LIST[i]) << ",";
		out << "\n";

		for (unsigned int i = 0; i < ACT_TYPE_LIST.size(); i++)
			out << int(this->ACT_TYPE_LIST[i]) << ",";
		out << "\n";

		for (unsigned int i = 0; i < TOTAL_W_LAYER.m; i++)
		{
			for (unsigned int j = 0; j < TOTAL_W_LAYER.n; j++)
				out << TOTAL_W_LAYER.vectors[j + i * TOTAL_W_LAYER.n] << ",";
			out << "\n";
		}
		out.close();
	}

	bool NEURON_BLOCKF::CHECK_VAR()
	{
		int size = TOTAL_W_LAYER.m * TOTAL_W_LAYER.n;
#pragma omp parallel for 
		for (int i = 0; i < size; i++)
		{
			if (std::isinf(TOTAL_B_LAYER.vectors[i]) || std::isnan(TOTAL_B_LAYER.vectors[i]))
				return true;
			if (std::isinf(TOTAL_W_LAYER.vectors[i]) || std::isnan(TOTAL_W_LAYER.vectors[i]))
				return true;
			if (std::isinf(TOTAL_Z_LAYER.vectors[i]) || std::isnan(TOTAL_Z_LAYER.vectors[i]))
				return true;
			if (std::isinf(TOTAL_A_LAYER.vectors[i]) || std::isnan(TOTAL_A_LAYER.vectors[i]))
				return true;
			if (std::isinf(TOTAL_M_LAYER.vectors[i]) || std::isnan(TOTAL_M_LAYER.vectors[i]))
				return true;
		}
		return false;
	}
	//Using multiple process omp

	void Median_Normalize(float* data, unsigned int size)
	{
		float* temp = (float*)malloc(sizeof(float) * size);
		memmove(temp, data, sizeof(float) * size);
		float Medium = Find_Mean(temp, size);
		free(temp);

		//standard diviation
		float sd = Standard(data, Medium, size);
		//Normalize
#pragma omp parallel for 
		for (unsigned int i = 0; i < size; i++)
			data[i] = (data[i] - Medium) / sd;
	}
	//Using multiple process omp

	void Mean_Normalize(float* data, unsigned int size)
	{
		float sum = 0;
		for (unsigned int i = 0; i < size; i++)
			sum += data[i];
		float mean = sum / size;
		float sd = Standard(data, mean, size);
#pragma omp parallel for 
		for (unsigned int i = 0; i < size; i++)
			data[i] = (data[i] - mean) / sd;
	}
	//Using multiple process omp

	void Quantile_Normarlize(float* data, unsigned int size)
	{
		float max = maxArray(data, size);
		float min = minArray(data, size);
		if (max - min != 0)
		{
#pragma omp parallel for
			for (unsigned int i = 0; i < size; i++)
				data[i] = (data[i] - min) / (max - min);
		}
	}
	//Using multiple process omp

	int minArray(float* data, unsigned int size)
	{
		float min = data[0];
		int num = 0;
#pragma omp parallel for 
		for (unsigned int i = 1; i < size; i++)
			if (min > data[i])
			{
				min = data[i];
				num = i;
			}
		return num;
	}
	//Using multiple process omp

	int maxArray(float* data, unsigned int size)
	{
		float max = data[0];
		int num = 0;
#pragma omp parallel for 
		for (unsigned int i = 1; i < size; i++)
			if (max < data[i])
			{
				max = data[i];
				num = i;
			}
		return num;
	}
	//Using multiple process omp

	void buble_sorting(float* data, unsigned int size)
	{
#pragma omp parallel for shared(data)
		for (unsigned int i = 0; i < size - 1; i++)
		{
			float temp = data[i];
			if (data[i + 1] < data[i])
			{
				data[i] = data[i + 1];
				data[i + 1] = temp;
				i = 0;
			}
		}
	}

	float Find_Mean(float data[], unsigned int size)
	{
		buble_sorting(data, size);
		float Mean = 0;
		if (size % 2 == 0)
		{
			int A = size / 2;
			int B = size / 2 + 1;
			Mean = data[(A + B) / 2];
		}
		else
		{
			Mean = data[size / 2 + 1];
		}
		return Mean;
	}
	//Using multiple process omp

	float Standard(float* data, float Ed, unsigned int size)
	{
		float sd = 0;
#pragma omp parallel for reduction(+:sd)
		for (unsigned int i = 0; i < size; i++)
			sd += pow(data[i] - Ed, 2);
		sd = sd / size;
		sd = sqrt(sd);
		return sd;
	}
	//Using multiple process omp

	float average(float* data, unsigned int size)
	{
		float avg = 0;
#pragma omp parallel for reduction (+:avg)
		for (unsigned int i = 0; i < size; i++)
			avg += data[i];
		return avg / (float)size;
	}
	//Using multiple process omp

	float variance(float* data, float avg, unsigned int size)
	{
		float var = 0;
#pragma omp parallel for reductino (+:var)
		for (unsigned int i = 0; i < size; i++)
			var += (data[i] - avg) * (data[i] - avg);
		return var / (float)size;
	}

	float z_score(float true_mean, float avg, float var)
	{
		if (var == 0)
			return 0;
		return (true_mean - avg) / var;
	}

	float cumulative_std(float z)
	{
		return erf(z * 0.70710678118) * 0.5 + 0.5;
	}

	float Z_SCORE_TEST(float* list, unsigned int size)
	{
		float avg = average(list, size);
		float var = variance(list, avg, size);
		float z = z_score(0, avg, var);
		float c = cumulative_std(z);
		return c;
	}

	float STANDARD(float* list, unsigned int size)
	{
		float avg = average(list, size);
		float var = variance(list, avg, size);
		return sqrt(var);
	}

}
