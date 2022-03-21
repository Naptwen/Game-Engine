#pragma once
#include "Window_rendering.h"


void WindowSize(string title, int w, int h)
{
	HWND console = GetConsoleWindow();
	RECT r;
	string tt = "Title : " + title;
	system(tt.c_str());
	GetWindowRect(console, &r);
	MoveWindow(console, 50, 50, w, h, SWP_SHOWWINDOW | SWP_NOMOVE);
}

void WindowWH(int* w, int* h) 
{
	RECT rect;
	HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC(myconsole);
	GetWindowRect(myconsole, &rect);

	*w = rect.right - rect.left;
	*h = rect.bottom - rect.top;
}
//str2wstr
wstring s2w(string str)
{
	wstring stemp = wstring(str.begin(), str.end());
	copy(str.begin(), str.end(), stemp.begin());
	return stemp;
}
//x and y are the position of pixel
//Draw Bitmap
void DrawBitBlt(int x, int y, string name)
{
	HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC(myconsole);
	LPCSTR str = name.c_str();
	HBITMAP hBit = (HBITMAP)LoadImage(NULL, str,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	ReleaseDC(myconsole, hdc);
	DeleteDC(MemDC);
}
void gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void textcolor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


void VIRTUAL_KEY(unsigned char KEY_CODE)
{
	INPUT ip;
	Sleep(1);
	//Ste up a key board event
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	//Press virtual key
	ip.ki.wVk = KEY_CODE; //Set virtual key code
	ip.ki.dwFlags = 0; //Press
	SendInput(1, &ip, sizeof(INPUT));

	//Release the virtual key
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}