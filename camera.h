#pragma once
#include "matrix.h"

void camera_mov(vector<float>* focus, vector<float>* eye, float hor, float ver) {
	vector<float> vec = { eye->at(0) - focus->at(0), eye->at(1) - focus->at(1), eye->at(2) - focus->at(2) };
	vector<float> up = { 0, 0, 1 };
	vector<float> vec_hor = cross3(vec, up);
	vector<float> front = cross3(up, vec_hor);
	eye->at(0) = eye->at(0) + front[0] * hor;
	eye->at(1) = eye->at(1) + front[1] * hor;
	eye->at(2) = eye->at(2) + front[2] * hor;

	focus->at(0) = focus->at(0) + front[0] * hor;
	focus->at(1) = focus->at(1) + front[1] * hor;
	focus->at(2) = focus->at(2) + front[2] * hor;
	eye->at(0) = eye->at(0) + vec_hor[0] * ver;
	eye->at(0) = eye->at(1) + vec_hor[1] * ver;
	eye->at(0) = eye->at(2) + vec_hor[2] * ver;
	focus->at(0) = focus->at(0) + vec_hor[0] * ver;
	focus->at(1) = focus->at(1) + vec_hor[1] * ver;
	focus->at(2) = focus->at(2) + vec_hor[2] * ver;
}

void camera_rot_hor(vector<float>* focus, vector<float>* eye, float degree) {
	vector<float> vec = { focus->at(0) - eye->at(0), focus->at(1) - eye->at(1) , focus->at(2) - eye->at(2) };
	float r = degree * (3.141592653589793238463 / 180.0);
	float _x = cos(r) * vec[0] - sin(r) * vec[1];
	float _y = sin(r) * vec[0] - cos(r) * vec[1];
	focus->at(0) = eye->at(0) + _x;
	focus->at(1) = eye->at(1) + _y;
}
