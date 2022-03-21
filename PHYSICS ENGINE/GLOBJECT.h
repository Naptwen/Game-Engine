#ifndef GLOBJECT_H_
#define GLOBJECT_H_

#pragma once
#include <gl/glew.h>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include "EBMP.h"
#include "EOBJ.h"
#include "ESTL.h"

#include "ECOLLISION.h"
#include "EPHYSICS.h"

#define TRI 2
#define QUAD 3
#define POLY 5
#define LINE 7
#define FORWARD 11
#define BACKWARD 13
#define LEFT 15
#define RIGHT 17
#define UP 19
#define DOWN 23

#define RECTANGLE 29
#define SPHERE 37

class LIGHT {
public:
	float ambient[4] = { 1.0, 1.0, 1.0, 0.0 };
	float diffuse[4] = { 1.0, 1.0, 1.0, 0.0 };
	float specular[4] = { 1.0, 1.0, 1.0, 0.0 };
	float position[4] = { 6.0, 6.0, 5.0, 0.0 };
	void LIGHT_ON() 
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	void LIGHT_OFF()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
};
class CAMERA {
public:
	float eye[3] = { 2,2,2 };
	float at[3] = { 0,0,0 };
	float w[3] = { 0,1,0 };
	float rot[3] = {0,0,0};

	void LOOKAT() {
		gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], w[0], w[1], w[2]);
	}
	void CAMERA_1ST_MOVE(const char MODE, float dis)
	{
		float d = sqrtf((at[0] - eye[0]) * (at[0] - eye[0]) + (at[1] - eye[1]) * (at[1] - eye[1]) + (at[2] - eye[2]) * (at[2] - eye[2]));
		float u[3] = { (eye[0] - at[0]) / d, (eye[1] - at[1]) / d, (eye[2] - at[2]) / d }; // front
		float v[3] = { u[1] * w[2] - u[2] * w[1],-(u[0] * w[2] - u[2] * w[0]), u[0] * w[1] - u[1] * w[0] }; //left
		switch (MODE) {
		case FORWARD:
			eye[0] -= u[0] * dis;
			eye[1] -= u[1] * dis;
			eye[2] -= u[2] * dis;
			at[0] -= u[0] * dis;
			at[1] -= u[1] * dis;
			at[2] -= u[2] * dis;
			break;
		case BACKWARD:
			eye[0] += u[0] * dis;
			eye[1] += u[1] * dis;
			eye[2] += u[2] * dis;
			at[0] += u[0] * dis;
			at[1] += u[1] * dis;
			at[2] += u[2] * dis;
			break;
		case LEFT:
			eye[0] += v[0] * dis;
			eye[1] += v[1] * dis;
			eye[2] += v[2] * dis;
			at[0] += v[0] * dis;
			at[1] += v[1] * dis;
			at[2] += v[2] * dis;
			break;
		case RIGHT:
			eye[0] -= v[0] * dis;
			eye[1] -= v[1] * dis;
			eye[2] -= v[2] * dis;
			at[0] -= v[0] * dis;
			at[1] -= v[1] * dis;
			at[2] -= v[2] * dis;
			break;
		}
	}
	void CAMERA_1ST_ROTATION(const char MODE, float dis)
	{
		float d = sqrtf((at[0] - eye[0]) * (at[0] - eye[0]) + (at[1] - eye[1]) * (at[1] - eye[1]) + (at[2] - eye[2]) * (at[2] - eye[2]));
		float u[3] = { (eye[0] - at[0]) / d, (eye[1] - at[1]) / d, (eye[2] - at[2]) / d }; // front
		float v[3] = { u[1] * w[2] - u[2] * w[1],-(u[0] * w[2] - u[2] * w[0]), u[0] * w[1] - u[1] * w[0] }; //left
		float t[3] = { v[1] * u[2] - v[2] * u[1],-(v[0] * u[2] - v[2] * u[0]), v[0] * u[1] - v[1] * u[0] }; //up
		switch (MODE) {
		case UP:
			at[0] -= t[0] * dis;
			at[1] -= t[1] * dis;
			at[2] -= t[2] * dis;
			break;
		case DOWN:
			at[0] += t[0] * dis;
			at[1] += t[1] * dis;
			at[2] += t[2] * dis;
			break;
		case LEFT:
			at[0] -= v[0] * dis;
			at[1] -= v[1] * dis;
			at[2] -= v[2] * dis;
			break;
		case RIGHT:
			at[0] += v[0] * d * dis;
			at[1] += v[1] * d * dis;
			at[2] += v[2] * d * dis;
			break;
		}
	}

};
class GLOBJECT {
private:
	GLuint vbo_vertex = 0;
	GLuint vbo_color = 0;
	GLuint vbo_normal = 0;
	GLuint vbo_uv = 0;
	GLuint vbo_texture = 0;
	GLuint vbo_ind = 0;

	unsigned char* color = nullptr;
	float* vertex = nullptr;
	float* normal = nullptr;
	float* uv = nullptr;
	unsigned int* ind = nullptr;
	unsigned char* image = nullptr;

	unsigned long ver_size = 0;
	unsigned long norm_size = 0;
	unsigned long uv_size = 0;
	unsigned long ind_size = 0;
	//image file
	unsigned long image_X = 0;
	unsigned long image_Y = 0;


	//collision
	//square
	ECOLLISION::Rectangle rect;
	ECOLLISION::Sphere sph;
	char ridge_type = 0;
	//Physics
	EPHYSICS::PHYSICS phys;
public:
	const char tag[4] = { 0,0,0,0 };
	float origin[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
	float Rotation[3] = { 0,0,0 };

	void GLOBJECT::MEMORYCLEAR();

	GLOBJECT::GLOBJECT();
	GLOBJECT::GLOBJECT(const GLOBJECT& to_copy);
	GLOBJECT::~GLOBJECT();

	void GLOBJECT::LOAD_STL(string filename);
	void GLOBJECT::LOAD_OBJ(string filename);
	void GLOBJECT::LOAD_BMP(string filename, const char alpha);
	void GLOBJECT::PUSHARRAY(const unsigned char* color, const float* vertex, const float* normal, const  float* uv, const unsigned int* ind,
							const unsigned int ver_size, const unsigned norm_size, const unsigned int uv_size, const unsigned int ind_size);
	void GLOBJECT::DRAW();
	//vertices, color, indices are required
	void GLOBJECT::CREATE_IBO();
	void GLOBJECT::DRAW_IBO();

	void GLOBJECT::pinfo();
	//immagdately locating the object [r] is rotation, [s] is scale
	void GLOBJECT::LOCATION(float x, float y, float z, float r1, float r2, float r3, float s1, float s2, float s3);
	//add the pos and rotation and scale
	void GLOBJECT::MOVE(float x, float y, float z, float r1, float r2, float r3, float s1, float s2, float s3);
	//RECTANGLE 
	//SPHERE 
	void GLOBJECT::RIDGEBODY_SET(const char type);
	void GLOBJECT::RIDGEBODY_POS();
	void GLOBJECT::RIDGEBODY_DRAW();
	bool GLOBJECT::RIDGEBODY_COLLISION(GLOBJECT* trg);
};
#endif