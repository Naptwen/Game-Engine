#pragma once
#include <cmath>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "TMATRIX_CUDA.cuh"
#define DEG2RAD  0.01745329251
static float testv = 0;
namespace ECOLLISION
{
	struct Rectangle
	{
		float collision_vec[24] = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0 };
		float center[3] = { 0,0,0 };
		float collision_pos[24] = { 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0 };
		float center_pos[3] = { 0,0,0 };
		Rectangle::Rectangle() 
		{
		};
		//OMP PARALLER
		bool Rectangle::body_set(float* vectors, unsigned int size);
		bool Rectangle::body_pos(float* Trans, float* Scale, float* Rotation);
		bool Rectangle::is_in_Rectangle(Rectangle* trg);
		bool Rectangle::OBB_detecting(Rectangle* trg);
	};
	struct Sphere
	{
		float collision_vec[3] = { 0,0,0 };
		float collision_pos[3] = { 0,0,0 };
		float radius = 0;
		float new_radius = 0;
		Sphere::Sphere() 
		{
		};
		//OMP PARALLER
		bool Sphere::body_set(float* vectors, unsigned int size);
		bool Sphere::body_pos(float* Trans, float* Scale, float* Rotation);
		bool Sphere::collision_two_sphere(Sphere* trg);
	};

	//pointer inside the space (the space vectors must be orthogonal)
	bool point_in_R3(float p[3], float a[3], float b[3], float d[3], float e[3]);
	//USING USG MATRIX MUMUTIPLICATION CPU
	//size is the # of vertices (not sizeof data)
	bool Translation3D_CPU(float* vectors, float* Trans, float* Scale, float* Rot, unsigned int size);
	//
	bool vector3DCrossProduct(float* A, float* B, float* C);
	//
	bool collinear_line_intersect(float a[6], float b[6]);
	//the size is the # of vec, not the size of array
	bool vector3DmaxminFinder(float* vec, unsigned int size, float* max, float* min);
	//
	bool vector3Dprojection(float* trg, float* src, float* result);
}
