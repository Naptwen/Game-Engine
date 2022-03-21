#ifndef EOBJ_H_
#define EOBJ_H_

#pragma once
#include <vector>
#include <fstream>
#include <string>


struct EOBJ
{
private:
	struct vec3
	{
		float x, y, z;
	};
	struct vec2
	{
		float x, y;
	};
	std::vector<vec3> temp_ver;
	std::vector<vec3> temp_normals;
	std::vector<vec2> temp_uv;
	std::vector<unsigned int> norm_ind;
	std::vector<unsigned int> uv_ind;
	std::vector<unsigned int> ver_ind;
public:
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<float> text_coordinates;
	std::vector<unsigned int> ind;

	explicit EOBJ::EOBJ(std::string file_name);
};

#endif