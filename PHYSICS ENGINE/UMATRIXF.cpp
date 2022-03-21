#include "UMATRIXF.h"
namespace UMF {
	UMATRIXF::UMATRIXF()
	{
		this->m = 0;
		this->n = 0;
		vectors = nullptr;
	}

	UMATRIXF::UMATRIXF(unsigned int m, unsigned int n)
	{
		this->m = m;
		this->n = n;
		int _size = m * n;
		vectors = (float*)calloc(_size, sizeof(float));
	}

	UMATRIXF::~UMATRIXF()
	{
		if (this->vectors != nullptr)
		{
			std::memset(this->vectors, 0, sizeof(float) * m * n);
			std::free(this->vectors);
		}
	}

	void UMATRIXF::operator = (const float* matrix)
	{
		unsigned int count = this->m * this->n;
		unsigned int size = count * sizeof(float);
		std::memcpy(vectors, matrix, size);
	}

	void UMATRIXF::operator = (const UMATRIXF& matrix)
	{
		this->m = matrix.m;
		this->n = matrix.n;
		if (this->vectors != nullptr)
			std::free(this->vectors);
		this->vectors = (float*)malloc(sizeof(float) * matrix.m * matrix.n);
		std::memcpy(this->vectors, matrix.vectors, sizeof(float) * matrix.m * matrix.n);
	}

	UMATRIXF* UMATRIXF::operator * (const  UMATRIXF& B)
	{
		if (this->n == B.m)
		{
			UMATRIXF* C = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			//A = m x n and B = n x p So C=AB is  m x p
			unsigned int Cm = this->m;
			unsigned int Cn = B.n;
			unsigned int size_C = Cm * Cn;
			float* matrix_C = (float*)malloc(size_C * sizeof(float));
			unsigned int x = 0, y = 0;
			for (unsigned int i = 0; i < size_C; i++)
			{
				x = i / Cn; //Why it is different with the coordinate position of vecotr2D on screen?
				y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
				float sum = 0;//														     (1,0)
				for (unsigned int k = 0; k < this->n; k++) // from 0 to n
				{
					unsigned int index_A = k + x * this->n; //Axk
					unsigned int index_B = y + k * B.n;     //Bky
					float temp = this->vectors[index_A] * B.vectors[index_B];

					sum += temp;
				}
				matrix_C[i] = sum; //Cxy = Sigma (Axk * Bky)
			}
			C->m = Cm;
			C->n = Cn;
			C->vectors = matrix_C;
			return C;
		}
		else
		{
			perror("UMATRIXF<*> : The size of row of A  and column of B are not the same");
			return nullptr;
		}
	}

	UMATRIXF* UMATRIXF::operator * (const  float f)
	{
		UMATRIXF* C = (UMATRIXF*)malloc(sizeof(UMATRIXF));
		unsigned int count = this->m * this->n;
		C->vectors = (float*)malloc(count * sizeof(float));
		memcpy(C->vectors, this->vectors, count * sizeof(float));
		unsigned int length = this->m * this->n;
		for (unsigned int k = 0; k < length; k++)// from 0 to n
			C->vectors[k] = C->vectors[k] * f;
		C->m = this->m;
		C->n = this->n;
		return C;
	}

	UMATRIXF* UMATRIXF::operator + (const float f)
	{
		UMATRIXF* C = (UMATRIXF*)malloc(sizeof(UMATRIXF));
		unsigned int count = this->m * this->n;
		C->vectors = (float*)malloc(count * sizeof(float));
		std::memcpy(C->vectors, this->vectors, count * sizeof(float));
		unsigned int length = this->m * this->n;
		for (unsigned int k = 0; k < length; k++)
			C->vectors[k] = C->vectors[k] + f;
		C->m = this->m;
		C->n = this->n;
		return C;
	}

	UMATRIXF* UMATRIXF::T()
	{
		unsigned int size_A = m * n;
		unsigned int x = 0, y = 0, temp_m = 0, j = 0, k = 0;
		//-----------------
		float* AT = (float*)malloc(size_A * sizeof(float));
		for (unsigned int i = 0; i < size_A; i++)
		{
			x = i / n;
			y = i % n;
			j = x + y * m; //reverse
			AT[j] = vectors[y + x * n];
		}
		//-----------------
		UMATRIXF* TRANSA = (UMATRIXF*)malloc(sizeof(UMATRIXF));
		TRANSA->m = this->n;
		TRANSA->n = this->m;
		TRANSA->vectors = AT;
		return TRANSA;
	}

	void UMATRIXF::pinfo()
	{
		printf("-----------------------------\n");
		unsigned int length = this->m * this->n;
		printf("m = %d, n = %d \n", this->m, this->n);
		int y = 1;
		for (unsigned int i = 0; i < length; i++)
		{
			std::printf("[%.10g]", this->vectors[i]);
			if (y == this->n)
			{
				printf("\n");
				y = 1;
			}
			else
				y++;
		}
		printf("-----------------------------\n");
	}

	void UMATRIXF::m2txt(const char* name)
	{
		unsigned int length = m * n;
		std::string out_line;
		std::ofstream out(name);
		unsigned int x = 1;
		for (unsigned int i = 0; i < length; i++)
		{
			out << this->vectors[i];
			out << " , ";
			if (x == this->n)
			{
				out << "\n";
				x = 1;
			}
			else
				x++;
		}
		out.close();
	}

	void UMATRIXF::ZEROS()
	{
		if (this->vectors != NULL)
			std::free(this->vectors);
		unsigned int size = 0;
		size = this->m * this->n;
		this->vectors = (float*)calloc(size, sizeof(float));
	}

	void UMATRIXF::IDENTITY()
	{
		unsigned int k = this->m;
		if (this->m < this->n)
			k = this->n;
		this->ZEROS();
		for (unsigned int i = 0; i < k; i++)
			this->vectors[i + i * this->n] = 1;
	}

	void UMATRIXF::ONES()
	{
		unsigned int size = this->m * this->n;
		for (unsigned int i = 0; i < size; i++)
		{
			this->vectors[i] = 1;
		}
	}


	UMATRIXF* ROWCUT(UMATRIXF* A, unsigned int st, unsigned int ed)
	{
		if ((int)(ed - st) >= 0 && ed < A->n)
		{
			UMATRIXF* RA = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			unsigned int range = ed - st + 1;
			RA->vectors = (float*)malloc(sizeof(float) * range * A->m);
			unsigned int k = 0;
			for (unsigned int j = 0; j < A->m; j++)
				for (unsigned int i = st; i <= ed; i++)
				{
					RA->vectors[k] = A->vectors[i + j * A->n];
					k++;
				}
			RA->m = A->m;
			RA->n = ed - st + 1;
			return RA;
		}
		else
		{
			perror("UMATRIXF<ROWCUT> : The size of the st or ed is out of index");
			return NULL;
		}
	}

	UMATRIXF* COLUMCUT(UMATRIXF* A, unsigned int st, unsigned int ed)
	{
		if ((int)(ed - st) >= 0 && ed < A->m)
		{
			UMATRIXF* CA = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			unsigned int range = ed - st + 1;
			CA->vectors = (float*)malloc(sizeof(float) * range * A->n);
			unsigned int k = 0;
			for (unsigned int i = st; i <= ed; i++)
				for (unsigned int j = 0; j < A->n; j++)
				{
					CA->vectors[k] = A->vectors[j + i * A->n];
					k++;
				}
			CA->n = A->n;
			CA->m = ed - st + 1;
			return CA;
		}
		else
		{
			perror("UMATRIXF<COLUMCUT> : The size of the st or ed is out of index");
			return NULL;
		}
	}

	UMATRIXF* MATRIXADDCOL(UMATRIXF* A, UMATRIXF* B)
	{
		if (A->n != B->n)
		{
			perror("UMATRIXF<MATRIXADDCOL> : The size of the A and B's the number of row are not the same");
		}
		else
		{
			int size_AlB = ((A->m) * (A->n) + (B->m) * (B->n));
			int AlB_m = A->m + B->m;
			int AlB_n = A->n;
			UMATRIXF* AlB = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			AlB->m = AlB_m;
			AlB->n = AlB_n;
			AlB->vectors = (float*)malloc(sizeof(float) * size_AlB);
			for (int i = 0; i < size_AlB; i++)
			{
				int m = i / AlB_n;
				int n = i % AlB_n;
				if (m < A->m)
					AlB->vectors[i] = A->vectors[n + m * A->n];
				else
				{
					int Bm = m - A->m;
					AlB->vectors[i] = B->vectors[n + Bm * B->n];
				}
			}
			return AlB;
		}
		return nullptr;
	}

	UMATRIXF* MATRIXADDROW(UMATRIXF* A, UMATRIXF* B)
	{
		if (A->m != B->m)
		{
			perror("UMATRIXF<MATRIXADDROW> : The size of the A and B's the number of column are not the same");
		}
		else
		{
			unsigned int size_A_B = ((A->m) * (A->n) + (B->m) * (B->n));
			unsigned int A_B_m = A->m;
			unsigned int A_B_n = A->n + B->n;
			UMATRIXF* A_B = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			A_B->m = A_B_m;
			A_B->n = A_B_n;
			A_B->vectors = (float*)malloc(sizeof(float) * size_A_B);
			for (unsigned int i = 0; i < size_A_B; i++)
			{
				unsigned int m = i / A_B_n;
				unsigned int n = i % A_B_n;
				if (n < A->n)
					A_B->vectors[i] = A->vectors[n + m * A->n];
				else
				{
					int Bn = n - A->n;
					A_B->vectors[i] = B->vectors[Bn + m * B->n];
				}
			}
			return A_B;
		}
		return nullptr;
	}

	UMATRIXF* GAUSSELI(UMATRIXF* A, int limit) {
		//Initializing
		UMATRIXF* G = (UMATRIXF*)malloc(sizeof(UMATRIXF));
		G->m = A->m;
		G->n = A->n;
		int size_G = G->m * G->n;
		G->vectors = (float*)malloc(sizeof(float) * size_G);
		memcpy(G->vectors, A->vectors, sizeof(float) * size_G);

		int n = 0;
		int m = 0;
		for (int n = 0; n < limit; n++)
		{
			float current = G->vectors[n + m * G->n];
			//If it is Zero
			if (current == 0)
				for (int k = 0; k < G->m; k++)
					if (G->vectors[n + k * G->n] != 0)
						swap_rows(G, k, m);
			//IF the column has at least one nonzero rows.
			if (current != 0)
			{
				//if it is no 1
				if (current != 1)
					Div_rows(G, m, G->vectors[n + m * G->n]);
				//Seaching other nonzero in the same column
				for (int k = 0; k < G->m; k++)
					if (m != k && G->vectors[n + k * G->n] != 0)
						Multi_Add_rows(G, k, m, -G->vectors[n + k * G->n]);
				//increase the row 
				//Since in Linear Algebra # pivot < # colum
				m++;
			}
		}
		return G;
	}

	UMATRIXF* COPY_ROW(UMATRIXF* A, unsigned int start, unsigned int howmany)
	{
		if (start >= A->n)
		{
			perror("UMATRIXF<COPY_ROW> : The start is out of the Row of the matrix");
		}
		else
		{
			UMATRIXF* A_B = (UMATRIXF*)malloc(sizeof(UMATRIXF));
			A_B->m = A->m;
			A_B->n = howmany;

			int size_A_B = A_B->m * A_B->n;
			A_B->vectors = (float*)malloc(size_A_B * sizeof(float));
			for (int i = 0; i < size_A_B; i++)
			{
				int m = i / A_B->n;
				int n = i % A_B->n;
				A_B->vectors[n + m * A_B->n] = A->vectors[start + m * A->n];
			}
			return A_B;
		}
		return nullptr;
	}

	extern void swap_rows(UMATRIXF* A, int r1, int r2)
	{
		if (r1 != r2)
			for (int k = 0; k < A->n; k++)
			{
				float temp = A->vectors[k + r1 * A->n];
				A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n];
				A->vectors[k + r2 * A->n] = temp;
			}
	}

	extern void Div_rows(UMATRIXF* A, int r1, float num)
	{
		for (int k = 0; k < A->n; k++)
		{
			A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] / num;
		}
	}

	extern void Multi_Add_rows(UMATRIXF* A, int r1, int r2, float num)
	{
		for (int k = 0; k < A->n; k++)
		{
			A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n] * num;
		}
	}

}