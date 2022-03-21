#ifndef GLFW_INCLUDE_VULKAN_H__
#define GLFW_INCLUDE_VULKAN_H__
#pragma once
#include "GLOBJECT.h"

class GLFW{
private:
	int w = 0;
	int h = 0;
	GLFWwindow* window = nullptr;

	LIGHT myLight;
	double xpos = 0, ypos = 0;//mouse pos
	bool mouse_bool = false;
	float T[3] = { 0,0,0 }; //screen pos
	float S[2] = { 2,1 }; //screen scale
	float R = 0; //world rotating from 010
	void GLFW::init_Window();
	void GLFW::screen_init();
	void GLFW::screen_reshpae();
	void GLFW::screen_llight();
	void GLFW::screen_clear();
	void GLFW::mainLoop();
	void GLFW::window_destroy();
public:
	CAMERA mycamera;
	vector<GLOBJECT>* obj_list;
	GLFW::GLFW(int w, int h);
	GLFW::~GLFW();
	void GLFW::run();
	void GLFW::windowEvent(int width, int height);
	void GLFW::scrollEvent(double x, double y);
	void GLFW::keyEvent(int key, int scancode, int action, int mods);
	void GLFW::mouseEvent(int button, int action, int mods);
	void GLFW::MyHandler();
};
//size is # of the vertices not the size of vertices array
void DRAW_RIDGE_BODY(float* vertices, float* center, unsigned int size);
void DRAW_COORDINATE();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
#endif