#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include <windows.h>  // Header File For Windows - has structures for BMP format
#include <stdio.h>      // Header File For Standard Input/Output
#include <stdlib.h>


using namespace std;

//Reference the wikipidea Bitmap information
#pragma pack(2) //for making byte 2
struct BMPHEADER{
	unsigned short bhmagic = 0;
	unsigned int   bhsize = 0;
	unsigned short bhrev1 = 0;
	unsigned short bhrev2 = 0;
	unsigned int   bhoffset = 0;
} ;

struct  BMPINFO {
	unsigned int   biSize = 0;
	unsigned int   biWidth = 0;
	unsigned int   biHeight = 0;
	unsigned short biPlanes = 0;
	unsigned short biBitCount = 0;
	unsigned int   biCompression = 0;
	unsigned int   biSizeImage = 0;
	unsigned int   biXPelsPerMeter = 0;
	unsigned int   biYPelsPerMeter = 0;
	unsigned int   biClrUsed = 0;
	unsigned int   biClrImportant = 0;
};
//the x y position is based on lower left corner of bmp
#pragma pack()
struct BMPRGB{
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char rev = 0;
	unsigned int x = 0;
	unsigned int y = 0;
};
class EBMP {
public:
	BMPHEADER bh;
	BMPINFO bi;
	BMPRGB* brgb;
	string file_name;
	int Gap;
	int size;
	void EBMP::INFORMATION();
	void EBMP::ROTATION(int degree);
	void EBMP::CUT(int x, int y, int w, int h);
	void EBMP::RESET();
	EBMP::EBMP(string file_name);
	EBMP::EBMP();
	EBMP::~EBMP();
private:
	void EBMP::LOADBMP();
};
