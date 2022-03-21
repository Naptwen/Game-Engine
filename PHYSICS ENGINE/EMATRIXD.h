#pragma once
#include <string>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ELOG.h"


class EMATRIXD//AVOIDING SHARING PTR TO PROTECT DATA
{
public:
	double* vectors;
	unsigned int m; //row
	unsigned int n; //column
	EMATRIXD::EMATRIXD();
	EMATRIXD::EMATRIXD(unsigned int m, unsigned int n);
	EMATRIXD::~EMATRIXD();
	void EMATRIXD::operator = (const double* matrix);
	void EMATRIXD::operator = (const EMATRIXD& matrix);
	EMATRIXD* EMATRIXD:: operator * (const EMATRIXD& B);
	EMATRIXD* EMATRIXD:: operator * (const double f);
	EMATRIXD* EMATRIXD::operator + (const  double f);
	EMATRIXD* EMATRIXD::T();
	void EMATRIXD::pinfo();
	void EMATRIXD::info();
	void EMATRIXD::m2txt(std::string name);
	void EMATRIXD::COLUMCUT(unsigned int st, unsigned int ed);
	void EMATRIXD::ROWCUT(unsigned int st, unsigned int ed);
	void EMATRIXD::ZEROS();
	void EMATRIXD::ONES();
	void EMATRIXD::IDENTITY();
};
void MATRIXCOPY_COL(EMATRIXD* A, int start, int howmany);
EMATRIXD* MATRIXADDROW(EMATRIXD* A, EMATRIXD* B);
EMATRIXD* MATRIXADDCOL(EMATRIXD* A, EMATRIXD* B);
EMATRIXD* GAUSSELI(EMATRIXD* A, int limit);
void swap_rows(EMATRIXD* A, int r1, int r2);
void Div_rows(EMATRIXD* A, int r1, double num);
void Multi_Add_rows(EMATRIXD* A, int r1, int r2, double num);