#pragma once
#pragma warning(disable: 4996)
#define DLLEXPORT extern "C" __declspec(dllexport)
#define UNITYPE char
#define uniI 2
#define uniC 3
#define uniF 5
#define uniF2 7
#define uniF3 11
#define uniS 13
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <malloc.h>


//reference from https://stackoverflow.com/questions/18780570/passing-a-c-stdvector-to-numpy-array-in-python
using namespace std;

float counting(char* text, const char* Delimiter);
extern "C" {
	struct C_obj {
		unsigned int sub_count = 0;
		double** vertices_list = {};
		unsigned int* vertices_size;
		double** normals_list = {};
		unsigned int* normals_size;
		double** text_coor_list = {};
		unsigned int* text_coor_size;
		unsigned int** ind_v_list = {};
		unsigned int* ind_v_size;
		unsigned int** ind_n_list = {};
		unsigned int* ind_n_size;
		unsigned int** ind_t_list = {};
		unsigned int* ind_t_size;
		const char* mtllib = "";
	};
}

DLLEXPORT C_obj* CHello_new();

DLLEXPORT void CHello_loading(C_obj* temp, const  char* file_name);




