#include "ESTL.h"

void byte2vertext(char* data, int i, float* vertex, bool z_up)
{
	char f1[4] = { data[i + 0],data[i + 1],data[i + 2],data[i + 3] };
	char f2[4] = { data[i + 4],data[i + 5],data[i + 6],data[i + 7] };
	char f3[4] = { data[i + 8],data[i + 9],data[i + 10],data[i + 11] };
	vertex[0] = *((float*)f1);
	if (z_up)
	{
		vertex[2] = *((float*)f2);
		vertex[1] = *((float*)f3);
	}
	else
	{
		vertex[1] = *((float*)f2);
		vertex[2] = *((float*)f3);
	}

}

ESTL::~ESTL() {
	MEMORYCLEAR();
}
ESTL::ESTL(std::string file_name, bool z_up)
{
	MEMORYCLEAR();
	std::ifstream stl;
	stl.open(file_name, std::ios::in | std::ios::binary);
	if (stl.good())
	{
		//READING HEADER
		stl.seekg(0, std::ios::beg);
		char header[80];
		stl.read(header, 80);
		//THE NUMBER OF TRIANGLE		
		char num_tri_data[4];
		stl.read(num_tri_data, 4);
		//The C++ casting type is the little endian!
		unsigned long num_tri_long = *(unsigned long*)num_tri_data;
		this->num = (unsigned long)num_tri_long * 9;
		this->vertices = (float*)malloc(sizeof(float) * num_tri_long * 9);
		this->normal = (float*)malloc(sizeof(float) * num_tri_long * 9);
		this->indice = (unsigned int*)malloc(sizeof(unsigned int) * num_tri_long * 9);
		char data[50];

#pragma omp parallel for
		for (unsigned int i = 0; i < num_tri_long * 9; i += 9)
		{
			stl.read(data, 50);
			float vertex[3];

			byte2vertext(data, 0, vertex, z_up);
			memcpy(&normal[i], &vertex, sizeof(float) * 3);
			memcpy(&normal[i + 3], &vertex, sizeof(float) * 3);
			memcpy(&normal[i + 6], &vertex, sizeof(float) * 3);
			byte2vertext(data, 12, vertex, z_up);
			memcpy(&vertices[i], &vertex, sizeof(float) * 3);
			byte2vertext(data, 24, vertex, z_up);
			memcpy(&vertices[i + 3], &vertex, sizeof(float) * 3);
			byte2vertext(data, 36, vertex, z_up);
			memcpy(&vertices[i + 6], &vertex, sizeof(float) * 3);

			this->indice[i] = i;
			this->indice[i + 1] = i + 1;
			this->indice[i + 2] = i + 2;
			this->indice[i + 3] = i + 3;
			this->indice[i + 4] = i + 4;
			this->indice[i + 5] = i + 5;
			this->indice[i + 6] = i + 6;
			this->indice[i + 7] = i + 7;
			this->indice[i + 8] = i + 8;
		}
		stl.close();
	}
	else
	{
		printf(" failed to read from file\n");
	}
}
void ESTL::MEMORYCLEAR() {
	this->num = 0;
	if (this->vertices != nullptr)
		free(this->vertices);
	if (this->normal != nullptr)
		free(this->normal);
	if (this->indice != nullptr)
		free(this->indice);
}

