#ifdef USE_EXPORT_KEYWORD
export
#endif
#include "TMATRIX.h"

template<typename vtype>
TMATRIX<vtype>::TMATRIX()
{
	this->m = 0;
	this->n = 0;
	vectors = nullptr;
}
template<typename vtype>
TMATRIX<vtype>::TMATRIX(unsigned int m, unsigned int n)
{
	this->m = m;
	this->n = n;
	int _size = m * n;
	vectors = (vtype*)calloc(_size, sizeof(vtype));
}
template<typename vtype>
TMATRIX<vtype>::~TMATRIX()
{
	if (this->vectors != nullptr)
	{
		std::memset(this->vectors, 0, sizeof(vtype) * m * n);
		std::free(this->vectors);
	}
}
template<typename vtype>
void TMATRIX<vtype>::operator = (const vtype* matrix)
{
	unsigned int count = this->m * this->n;
	unsigned int size = count * sizeof(vtype);
	std::memcpy(vectors, matrix, size);
}
template<typename vtype>
void TMATRIX<vtype>::operator = (const TMATRIX<vtype>& matrix)
{
	this->m = matrix.m;
	this->n = matrix.n;
	if (this->vectors != nullptr)
		std::free(this->vectors);
	this->vectors = (vtype*)malloc(sizeof(vtype) * matrix.m * matrix.n);	
	std::memcpy(this->vectors, matrix.vectors, sizeof(vtype) * matrix.m * matrix.n);
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::operator + (const  TMATRIX<vtype>& B)
{
	if (this->n == B.n && this->m == B.m)
	{
		TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		//A = m x n and B = n x p So C=AB is  m x p
		C->m = this->m;
		C->n = this->n;
		unsigned int size_C = C->m * C->n;
		vtype* matrix_C = (vtype*)malloc(size_C * sizeof(vtype));
		unsigned int x = 0, y = 0;
		for (unsigned int i = 0; i < size_C; i++)
		{
			matrix_C[i] = this->vectors[i] + B.vectors[i];
		}
		C->vectors = matrix_C;
		return C;
	}
	else
	{
		perror("TMATRIX<-> : The size of A  and B are not the same");
		return nullptr;
	}
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::operator - (const  TMATRIX<vtype>& B)
{
	if (this->n == B.n && this->m == B.m)
	{
		TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		//A = m x n and B = n x p So C=AB is  m x p
		C->m = this->m;
		C->n = this->n;
		unsigned int size_C = C->m * C->n;
		vtype* matrix_C = (vtype*)malloc(size_C * sizeof(vtype));
		unsigned int x = 0, y = 0;
		for (unsigned int i = 0; i < size_C; i++)
		{
			matrix_C[i] = this->vectors[i] - B.vectors[i];
		}
		C->vectors = matrix_C;
		return C;
	}
	else
	{
		perror("TMATRIX<-> : The size of A  and B are not the same");
		return nullptr;
	}
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::operator * (const  TMATRIX<vtype>& B)
{
	if (this->n == B.m)
	{
		TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		//A = m x n and B = n x p So C=AB is  m x p
		unsigned int Cm = this->m;
		unsigned int Cn = B.n;
		unsigned int size_C = Cm * Cn;
		vtype* matrix_C = (vtype*)malloc(size_C * sizeof(vtype));
		unsigned int x = 0, y = 0;
		for (unsigned int i = 0; i < size_C; i++)
		{
			x = i / Cn; //Why it is different with the coordinate position of vecotr2D on screen?
			y = i % Cn; //the reason is that (1,0) on 2D is (0,0)(1,0) but in matrix (0,0) so placed in different position.
			vtype sum = 0;//														     (1,0)
			for (unsigned int k = 0; k < this->n; k++) // from 0 to n
			{
				unsigned int index_A = k + x * this->n; //Axk
				unsigned int index_B = y + k * B.n;     //Bky
				vtype temp = this->vectors[index_A] * B.vectors[index_B];

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
		perror("TMATRIX<*> : The size of row of A  and column of B are not the same");
		return nullptr;
	}
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::operator * (const  vtype f)
{
	TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
	unsigned int count = this->m * this->n;
	C->vectors = (vtype*)malloc(count * sizeof(vtype));
	memcpy(C->vectors, this->vectors, count * sizeof(vtype));
	unsigned int length = this->m * this->n;
	for (unsigned int k = 0; k < length; k++)// from 0 to n
		C->vectors[k] = C->vectors[k] * f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::operator + (const vtype f)
{
	TMATRIX<vtype>* C = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
	unsigned int count = this->m * this->n;
	C->vectors = (vtype*)malloc(count * sizeof(vtype));
	std::memcpy(C->vectors, this->vectors, count * sizeof(vtype));
	unsigned int length = this->m * this->n;
	for (unsigned int k = 0; k < length; k++)
		C->vectors[k] = C->vectors[k] + f;
	C->m = this->m;
	C->n = this->n;
	return C;
}
template<typename vtype>
TMATRIX<vtype>* TMATRIX<vtype>::T()
{
	unsigned int size_A = m * n;
	unsigned int x = 0, y = 0, temp_m = 0, j = 0, k = 0;
	//-----------------
	vtype* AT = (vtype*)malloc(size_A * sizeof(vtype));
	for (unsigned int i = 0; i < size_A; i++)
	{
		x = i / n;
		y = i % n;
		j = x + y * m; //reverse
		AT[j] = vectors[y + x * n];
	}
	//-----------------
	TMATRIX<vtype>* TRANSA = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
	TRANSA->m = this->n;
	TRANSA->n = this->m;
	TRANSA->vectors = AT;
	return TRANSA;
}
template<typename vtype>
void TMATRIX<vtype>::pinfo()
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
template<typename vtype>
void TMATRIX<vtype>::m2txt(const char* name)
{
	unsigned int length = m * n;
	std::string out_line;
	std::ofstream out(name);
	unsigned int x = 1;
	for (unsigned int i = 0; i < length; i++)
	{
		out << this->vectors[i] << " , ";
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
template<typename vtype>
void TMATRIX<vtype>::ZEROS()
{
	if (this->vectors != NULL)
		std::free(this->vectors);
	unsigned int size = 0;
	size = this->m * this->n;
	this->vectors = (vtype*)calloc(size, sizeof(vtype));
}
template<typename vtype>
void TMATRIX<vtype>::IDENTITY()
{
	unsigned int k = this->m;
	if (this->m < this->n)
		k = this->n;
	this->ZEROS();
	for (unsigned int i = 0; i < k; i++)
		this->vectors[i + i * this->n] = 1;
}
template<typename vtype>
void TMATRIX<vtype>::ONES()
{
	unsigned int size = this->m * this->n;
	for (unsigned int i = 0; i < size; i++)
	{
		this->vectors[i] = 1;
	}
}

template<typename vtype>
TMATRIX<vtype>* ROWCUT(TMATRIX<vtype>* A, unsigned int st, unsigned int ed)
{
	if ((int)(ed - st) >= 0 && ed < A->n)
	{
		TMATRIX<vtype>* RA = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		unsigned int range = ed - st + 1;
		RA->vectors = (vtype*)malloc(sizeof(vtype) * range * A->m);
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
		perror("TMATRIX<ROWCUT> : The size of the st or ed is out of index");
		return NULL;
	}
}
template<typename vtype>
TMATRIX<vtype>* COLUMCUT(TMATRIX<vtype>* A, unsigned int st, unsigned int ed)
{
	if ((int)(ed - st) >= 0 && ed < A->m)
	{
		TMATRIX<vtype>* CA = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		unsigned int range = ed - st + 1;
		CA->vectors = (vtype*)malloc(sizeof(vtype) * range * A->n);
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
		perror("TMATRIX<COLUMCUT> : The size of the st or ed is out of index");
		return NULL;
	}
}
template<typename vtype>
TMATRIX<vtype>* MATRIXADDCOL(TMATRIX<vtype>* A, TMATRIX<vtype>* B)
{
	if (A->n != B->n)
	{
		perror("TMATRIX<MATRIXADDCOL> : The size of the A and B's the number of row are not the same");
	}
	else
	{
		int size_AlB = ((A->m) * (A->n) + (B->m) * (B->n));
		int AlB_m = A->m + B->m;
		int AlB_n = A->n;
		TMATRIX<vtype>* AlB = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		AlB->m = AlB_m;
		AlB->n = AlB_n;
		AlB->vectors = (vtype*)malloc(sizeof(vtype) * size_AlB);
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
template<typename vtype>
TMATRIX<vtype>* MATRIXADDROW(TMATRIX<vtype>* A, TMATRIX<vtype>* B)
{
	if (A->m != B->m)
	{
		perror("TMATRIX<MATRIXADDROW> : The size of the A and B's the number of column are not the same");
	}
	else
	{
		unsigned int size_A_B = ((A->m) * (A->n) + (B->m) * (B->n));
		unsigned int A_B_m = A->m;
		unsigned int A_B_n = A->n + B->n;
		TMATRIX<vtype>* A_B = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		A_B->m = A_B_m;
		A_B->n = A_B_n;
		A_B->vectors = (vtype*)malloc(sizeof(vtype) * size_A_B);
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
template<typename vtype>
TMATRIX<vtype>* GAUSSELI(TMATRIX<vtype>* A, int limit) {
	//Initializing
	TMATRIX<vtype>* G = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
	G->m = A->m;
	G->n = A->n;
	int size_G = G->m * G->n;
	G->vectors = (vtype*)malloc(sizeof(vtype) * size_G);
	memcpy(G->vectors, A->vectors, sizeof(vtype) * size_G);

	int n = 0;
	int m = 0;
	for (int n = 0; n < limit; n++)
	{
		vtype current = G->vectors[n + m * G->n];
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
template<typename vtype>
TMATRIX<vtype>* COPY_ROW(TMATRIX<vtype>* A, unsigned int start, unsigned int howmany)
{
	if (start >= A->n)
	{
		perror("TMATRIX<COPY_ROW> : The start is out of the Row of the matrix");
	}
	else
	{
		TMATRIX<vtype>* A_B = (TMATRIX<vtype>*)malloc(sizeof(TMATRIX<vtype>));
		A_B->m = A->m;
		A_B->n = howmany;

		int size_A_B = A_B->m * A_B->n;
		A_B->vectors = (vtype*)malloc(size_A_B * sizeof(vtype));
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
template<typename vtype>
extern void swap_rows(TMATRIX<vtype>* A, int r1, int r2)
{
	if (r1 != r2)
		for (int k = 0; k < A->n; k++)
		{
			vtype temp = A->vectors[k + r1 * A->n];
			A->vectors[k + r1 * A->n] = A->vectors[k + r2 * A->n];
			A->vectors[k + r2 * A->n] = temp;
		}
}
template<typename vtype>
extern void Div_rows(TMATRIX<vtype>* A, int r1, vtype num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] / num;
	}
}
template<typename vtype>
extern void Multi_Add_rows(TMATRIX<vtype>* A, int r1, int r2, vtype num)
{
	for (int k = 0; k < A->n; k++)
	{
		A->vectors[k + r1 * A->n] = A->vectors[k + r1 * A->n] + A->vectors[k + r2 * A->n] * num;
	}
}

