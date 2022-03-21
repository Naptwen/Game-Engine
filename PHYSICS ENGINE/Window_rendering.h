#pragma once
#include <Windows.h>
#include <string>

using namespace std;

wstring s2w(string str);
void DrawBitBlt(int x, int y, string name);
void gotoxy(int x, int y);
void WindowWH(int* w, int* h);
void WindowSize(string title, int w, int h);
void textcolor(int foreground, int background);
void VIRTUAL_KEY(unsigned char KEY_CODE);