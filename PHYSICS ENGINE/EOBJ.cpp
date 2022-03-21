#include "EOBJ.h"

EOBJ::EOBJ(std::string file_name)
{

	FILE* obj = fopen(file_name.c_str(), "r");
	if (obj != NULL)
	{
		while (true)
		{
			char data[128];
			int res = fscanf(obj, "%s",data);
			if (res == EOF)
				break;
			if (strcmp(data, "v") == 0)
			{
				vec3 temp;
				fscanf(obj, "%f %f %f\n", &temp.x, &temp.y, &temp.z);
				temp_ver.push_back(temp);
			}
			else if (strcmp(data, "vt") == 0) 
			{
				vec2 temp;
				fscanf(obj, "%f %f\n", &temp.x, &temp.y);
				temp_uv.push_back(temp);
			}
			else if (strcmp(data, "vn") == 0)
			{
				vec3 temp;
				fscanf(obj, "%f %f %f\n", &temp.x, &temp.y, &temp.z);
				temp_normals.push_back(temp);
			}
			else if (strcmp(data, "f") == 0)
			{
				if (temp_uv.size() > 0 && temp_normals.size() > 0)
				{
					unsigned int vertex[3];
					unsigned int uv[3];
					unsigned int normal[3];
					fscanf(obj, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
						&vertex[0], &uv[0], &normal[0],
						&vertex[1], &uv[1], &normal[1],
						&vertex[2], &uv[2], &normal[2]);

					ver_ind.push_back(vertex[0] - 1);
					ver_ind.push_back(vertex[1] - 1);
					ver_ind.push_back(vertex[2] - 1);

					uv_ind.push_back(uv[0] - 1);
					uv_ind.push_back(uv[1] - 1);
					uv_ind.push_back(uv[2] - 1);

					norm_ind.push_back(normal[0] - 1);
					norm_ind.push_back(normal[1] - 1);
					norm_ind.push_back(normal[2] - 1);
				}
				else
				{
					unsigned int vertex[3];
					fscanf(obj, "%d %d %d\n", &vertex[0], &vertex[1], &vertex[2]);
					ver_ind.push_back(vertex[0] - 1);
					ver_ind.push_back(vertex[1] - 1);
					ver_ind.push_back(vertex[2] - 1);
				}

			}
		}
		for (int i = 0; i < ver_ind.size(); i++)
		{
			unsigned int index = ver_ind[i];
			vertices.push_back(temp_ver[index].x);
			vertices.push_back(temp_ver[index].y);
			vertices.push_back(temp_ver[index].z);
			ind.push_back(i);
		}
		for (int i = 0; i < norm_ind.size(); i++)
		{
			unsigned int index = norm_ind[i];
			normals.push_back(temp_normals[index].x);
			normals.push_back(temp_normals[index].y);
			normals.push_back(temp_normals[index].z);
		}
		for (int i = 0; i < uv_ind.size(); i++)
		{
			unsigned int index = uv_ind[i];
			text_coordinates.push_back(temp_uv[index].x);
			text_coordinates.push_back(temp_uv[index].y);
		}
	}
	fclose(obj);
}