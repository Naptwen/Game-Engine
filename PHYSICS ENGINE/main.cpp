
#pragma once
//--------------------------
#include <stdio.h>
#include <crtdbg.h>
#include <conio.h>
#include <vector>
#include <chrono> // for time
#include <thread>
#include <mutex>
#include <ctype.h>

//---------------------------
#include "Window_rendering.h"
#include "EPython.h"
#include "ECSV.h"
//External header
//#include "ConsoleLogger.h"
//--------------------------



#include "ECamera.h"
/*VULKAN*/
#include "GLFW.h"

#include "UNEURON_CUDAF.cuh"



using namespace std;

static vector<string> data_set;
static vector<int> result_set;

constexpr unsigned int str2int(const char* str, int h)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}


int main()
{
	printf("%f test", 0 / 1);
	GLFW screen1(800, 400);
	screen1.mycamera.eye[0] = 5;
	screen1.mycamera.eye[1] = 5;
	screen1.mycamera.eye[2] = 5;
	screen1.mycamera.at[0] = 3;
	screen1.mycamera.at[1] = 0;
	screen1.mycamera.at[2] = 3;

	GLOBJECT test1;
	test1.LOAD_OBJ("Cube.obj");
	test1.LOAD_BMP("Texture/Dirt.bmp", 255);
	test1.LOCATION(0, 0, 0, 0, 45, 0, 1,1,1);
	test1.RIDGEBODY_SET(RECTANGLE);
	printf("SET OBJ1\n");

	GLOBJECT test2;
	test2.LOAD_OBJ("Cube.obj");
	test2.LOAD_BMP("Texture/Dirt.bmp", 255);
	test2.LOCATION(2, 0.2, 2, 10, 5, 0, 1,1,1);
	test2.RIDGEBODY_SET(RECTANGLE);
	printf("SET OBJ2\n");

	vector<GLOBJECT> obj_list;
	obj_list.push_back(test1);
	obj_list.push_back(test2);
	printf("SET OBJ LIST\n");

	screen1.obj_list = &obj_list;
	std::thread t(&GLFW::run, &screen1);
	t.join();
	

	/*
	GLOBJECT test2;
	test2.PUSHARRAY(test2_colors, test2_ver, test2_norm, test2_texCoords, test2_indices, 72, 48, 36);
	test2.LOCATION(0, 0, 0, 0, 0, 0, 10, 0.1, 10);
	test2.LOAD_BMP("Texture/Dirt.bmp", 255);
	screen1.PUSH_OBJECT(&test2);
	*/

	printf("GLFW CLOSED\n");
	
	//FILE_READ_TXT("emotions.txt");
	//TEST();
	_getch();

	//_getch();
	//PyObject* python_handle;
	//PyObject* run = RUN_PYTHON("pyGUI", "TEST", "o", &python_handle);
	//PyObject* result = PyUnicode_AsEncodedString(PyObject_Str(run), "UTF-8", "strict");
	//cout << strdup(PyBytes_AS_STRING(result));

	//mutex m;
	//----------------Making Test value
	return 0;
}
