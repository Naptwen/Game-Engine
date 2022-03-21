
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
/*
void FILE_READ_TXT(string name)
{
	std::ifstream fin;
	fin.open(name);
	string data;
	string result;
	string data_1;
	string data_2;
	int count = 0;
	while (!fin.eof())
	{
		std::getline(fin, data);
		data_1 = slicer(&data, ";");
		data_2 = data;
		data_set.push_back(data_1);

		if (data_2 == "fear")
			result_set.push_back(0);
		else if (data_2 == "love")
			result_set.push_back(1);
		else if (data_2 == "anger")
			result_set.push_back(2);
		else if (data_2 == "joy")
			result_set.push_back(3);
		else if (data_2 == "sadness")
			result_set.push_back(4);
		else if (data_2 == "surprise")
			result_set.push_back(5);
		count++;
		if (count > 8000)
			break;
	}
	//cout << data_set.size() << endl;
	//cout << result_set.size() << endl;
	//cout << max << endl;
	fin.close();
}
void FILE_LOG(streambuf* backup)
{
	//backup = std::cout.rdbuf();
	ofstream file;
	file.open("log.txt");
	cout.rdbuf(file.rdbuf());
}
void FILE_READ(streambuf* backup)
{
	std::ifstream fin;
	fin.open("log.txt");
	string line;
	cout.rdbuf(backup);
	while (!fin.eof())
	{
		getline(fin, line);
		cout << line << endl;
	}
	fin.close();
}
void printer(const char* txt)
{
	printf("\r%s", txt);
}

void TEST()
{
	NEURON_BLOCKF test;
	test.ADD_LAYER(300, NONE);
	test.ADD_LAYER(300, Soft_ReLU);
	test.ADD_LAYER(100, Soft_ReLU);
	test.ADD_LAYER(6, NONE);
	test.W_INIT(HENORM);
	test.LEARN_SET(0.9, 0.0005, MEAN, NEST);
	float data[300];
	float trg[6];

	int count = 0;
	while (count < 100)
	{
		srand(time(NULL));
		//MINIBATCH
		int index = rand() % 8000 + 0;
		for (int i = 0; i < 300; i++)
		{
			data[i] = (data_set[index].c_str())[i];
		}
		for (int i = 0; i < 6; i++)
		{
			trg[i] = 0.5;
		}
		//DATA SCAILING
		Mean_Normalize(data, 300); //FOR SCAILING THE INPUT DATA
		for (int i = 0; i < 6; i++)
		{
			if (i == result_set[index])
			{
				trg[i] = 0.9;
				break;
			}
		}
		cout << "LEARN FOR [" << index << "] \n";
		test.INPUT(data, 300);

		test.NEURON_SAVE("NEW TYPE2.csv");
		cout << "DONE FOR [" << index << "] \n";
	}

}
*/


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
