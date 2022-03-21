#ifndef DLL_IFACE
#ifdef DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllexport )
#else  // !DLL_IFACE_EXPORT
#define DLL_IFACE _declspec( dllimport )
#endif // !DLL_IFACE_EXPORT
#endif // !DLL_IFACE

#ifndef TMATRIX_H__
#define TMATRIX_H__

#ifdef USE_EXPORT_KEYWORD
export
#endif

#include <stdio.h>
#include <fstream>

template<typename vtype>
struct TMATRIX//AVOIDING SHARING PTR TO PROTECT DATA
{
public:
	vtype* vectors = NULL;
	unsigned int m = 0; //row
	unsigned int n = 0; //column

	TMATRIX();
	//[m] column number
	//[n] row number
	//The matrix coordinate is start from upper left to right bleow
	TMATRIX(unsigned int m, unsigned int n);
	~TMATRIX();
	//This Matrix receive the same type of data
	//The other data which out of the size is ignored
	void operator = (const vtype* matrix);
	void operator = (const TMATRIX& matrix);
	TMATRIX<vtype>* operator * (const TMATRIX& B);
	TMATRIX<vtype>* operator + (const TMATRIX& B);
	TMATRIX<vtype>* operator - (const TMATRIX& B);
	TMATRIX<vtype>* operator * (const vtype f);
	TMATRIX<vtype>* operator + (const  vtype f);
	TMATRIX<vtype>* T();

	void pinfo();
	void m2txt(const char* name);
	void ZEROS();
	void IDENTITY();
	void ONES();
};
template<typename vtype>
TMATRIX<vtype>* ROWCUT(TMATRIX<vtype>* A, unsigned int st, unsigned int ed);
template<typename vtype>
TMATRIX<vtype>* COLUMCUT(TMATRIX<vtype>* A, unsigned int st, unsigned int ed);
template<typename vtype>
TMATRIX<vtype>* MATRIXADDCOL(TMATRIX<vtype>* A, TMATRIX<vtype>* B);
template<typename vtype>
TMATRIX<vtype>* MATRIXADDROW(TMATRIX<vtype>* A, TMATRIX<vtype>* B);
template<typename vtype>
TMATRIX<vtype>* GAUSSELI(TMATRIX<vtype>* A, int limit);
template<typename vtype>
TMATRIX<vtype>* COPY_ROW(TMATRIX<vtype>* A, unsigned int start, unsigned int howmany);
template<typename vtype>
void swap_rows(TMATRIX<vtype>* A, int r1, int r2);
template<typename vtype>
void Div_rows(TMATRIX<vtype>* A, int r1, vtype num);
template<typename vtype>
void Multi_Add_rows(TMATRIX<vtype>* A, int r1, int r2, vtype num);

#ifndef USE_EXPORT_KEYWORD
#include "TMATRIX.cpp"
#endif

#endif