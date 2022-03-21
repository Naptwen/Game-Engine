#ifndef ESTL_H_
#define ESTL_H_

#pragma once
#include <fstream>
#include <string>
#include <vector>

void byte2vertext(char* data, int i, float* vertex, bool z_up);
struct ESTL
{
public:
	float* vertices = nullptr;
	float* normal = nullptr;
	unsigned int* indice = nullptr;
	unsigned long num = 0;
	ESTL::ESTL(std::string file_name, bool z_up);
	ESTL::~ESTL();
	void ESTL::MEMORYCLEAR();
};
#endif