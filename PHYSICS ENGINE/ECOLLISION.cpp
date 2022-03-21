#include "ECOLLISION.h"
#include <GLFW/glfw3.h>
namespace ECOLLISION
{
	bool Rectangle::body_set(float* vectors, unsigned int size)
	{
		float max[3] = { 0,0,0 };
		float min[3] = { 0,0,0 };
		vector3DmaxminFinder(vectors, size/3, max, min);
		float a = max[0] - min[0];
		float b = max[1] - min[1];
		float c = max[2] - min[2];

		center[0] = min[0] + a / 2;
		center[1] = min[1] + b / 2;
		center[2] = min[2] + c / 2;

		//bottom ccw
		float xyz0[3] = { min[0], min[1], min[2] };
		float xyz1[3] = { min[0]+ a, min[1], min[2] };
		float xyz2[3] = { min[0]+ a, min[1], min[2] + c };
		float xyz3[3] = { min[0], min[1], min[2] + c };
		//top ccw			 
		float xyz4[3] = { min[0], min[1] + b, min[2] };
		float xyz5[3] = { min[0]+ a, min[1] + b, min[2] };
		float xyz6[3] = { min[0]+ a, min[1] + b, min[2] + c };
		float xyz7[3] = { min[0], min[1] + b, min[2] + c };
		//___________VEC___________ static value
		collision_vec[0] = xyz0[0];
		collision_vec[1] = xyz0[1];
		collision_vec[2] = xyz0[2];

		collision_vec[3] = xyz1[0];
		collision_vec[4] = xyz1[1];
		collision_vec[5] = xyz1[2];

		collision_vec[6] = xyz2[0];
		collision_vec[7] = xyz2[1];
		collision_vec[8] = xyz2[2];

		collision_vec[9] = xyz3[0];
		collision_vec[10] = xyz3[1];
		collision_vec[11] = xyz3[2];

		collision_vec[12] = xyz4[0];
		collision_vec[13] = xyz4[1];
		collision_vec[14] = xyz4[2];

		collision_vec[15] = xyz5[0];
		collision_vec[16] = xyz5[1];
		collision_vec[17] = xyz5[2];

		collision_vec[18] = xyz6[0];
		collision_vec[19] = xyz6[1];
		collision_vec[20] = xyz6[2];

		collision_vec[21] = xyz7[0];
		collision_vec[22] = xyz7[1];
		collision_vec[23] = xyz7[2];
		//___________POS___________
		collision_pos[0] = xyz0[0];
		collision_pos[1] = xyz0[1];
		collision_pos[2] = xyz0[2];

		collision_pos[3] = xyz1[0];
		collision_pos[4] = xyz1[1];
		collision_pos[5] = xyz1[2];

		collision_pos[6] = xyz2[0];
		collision_pos[7] = xyz2[1];
		collision_pos[8] = xyz2[2];

		collision_pos[9] = xyz3[0];
		collision_pos[10] = xyz3[1];
		collision_pos[11] = xyz3[2];

		collision_pos[12] = xyz4[0];
		collision_pos[13] = xyz4[1];
		collision_pos[14] = xyz4[2];

		collision_pos[15] = xyz5[0];
		collision_pos[16] = xyz5[1];
		collision_pos[17] = xyz5[2];

		collision_pos[18] = xyz6[0];
		collision_pos[19] = xyz6[1];
		collision_pos[20] = xyz6[2];

		collision_pos[21] = xyz7[0];
		collision_pos[22] = xyz7[1];
		collision_pos[23] = xyz7[2];

		this->center_pos[0] = center[0];
		this->center_pos[1] = center[1];
		this->center_pos[2] = center[2];

		return true;
	}
	bool Rectangle::body_pos(float* Trans, float* Scale, float* Rotation)
	{
			float xyz[24];
			float xyz8[3];// center
			memcpy(xyz, collision_vec, sizeof(float) * 24);
			memcpy(xyz8, center, sizeof(float) * 3);
			Translation3D_CPU(xyz, Trans, Scale, Rotation, 8);
			Translation3D_CPU(xyz8, Trans, Scale, Rotation, 1);
			memcpy(collision_pos, xyz, sizeof(float) * 24);
			memcpy(center_pos, xyz8,sizeof(float) * 3);
			return true;
	}
	bool Rectangle::is_in_Rectangle(Rectangle* trg)
	{
		float p[3] = { collision_pos[0],			collision_pos[1],				collision_pos[2] };
		float a[3] = { trg->collision_pos[0],		trg->collision_pos[1],			trg->collision_pos[2] };
		float b[3] = { trg->collision_pos[3],		trg->collision_pos[4],			trg->collision_pos[5] };
		float d[3] = { trg->collision_pos[9],		trg->collision_pos[10],			trg->collision_pos[11] };
		float e[3] = { trg->collision_pos[12],		trg->collision_pos[13],			trg->collision_pos[14] };
		if (ECOLLISION::point_in_R3(p, a, b, d, e))return true;
		p[0] = collision_pos[3];
		p[1] = collision_pos[4];
		p[2] = collision_pos[5];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[6];
		p[1] = collision_pos[7];
		p[2] = collision_pos[8];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[9];
		p[1] = collision_pos[10];
		p[2] = collision_pos[11];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[12];
		p[1] = collision_pos[13];
		p[2] = collision_pos[14];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[15];
		p[1] = collision_pos[16];
		p[2] = collision_pos[17];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[18];
		p[1] = collision_pos[19];
		p[2] = collision_pos[20];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		p[0] = collision_pos[21];
		p[1] = collision_pos[22];
		p[2] = collision_pos[23];
		if (ECOLLISION::point_in_R3(p, a, b, d, e)) return true;
		return false;
	}
	bool Rectangle::OBB_detecting(Rectangle* trg)
	{

		float a[12] = {
			collision_pos[0], collision_pos[1], collision_pos[2],
			collision_pos[3], collision_pos[4], collision_pos[5],
			collision_pos[9], collision_pos[10], collision_pos[11],
			collision_pos[12],collision_pos[13], collision_pos[14]
		};
		float b[12] = {
			trg->collision_pos[0], trg->collision_pos[1], trg->collision_pos[2],
			trg->collision_pos[3], trg->collision_pos[4], trg->collision_pos[5],
			trg->collision_pos[9], trg->collision_pos[10], trg->collision_pos[11],
			trg->collision_pos[12], trg->collision_pos[13], trg->collision_pos[14]
		};

		float* ver_a = collision_pos;
		float* ver_b = trg->collision_pos;
		//axis 
		float A[9] = {
		 (a[3] - a[0]), (a[4] - a[1]) , (a[5] - a[2]),
		 (a[6] - a[0]), (a[7] - a[1]) , (a[8] - a[2]),
		 (a[9] - a[0]), (a[10] - a[1]), (a[11] - a[2]) };
		float B[9] = {
		(b[3] - b[0]) , (b[4] - b[1])  , (b[5] - b[2]) ,
		(b[6] - b[0]) , (b[7] - b[1])  , (b[8] - b[2]) ,
		(b[9] - b[0]) , (b[10] - b[1]) , (b[11] - b[2])  };

		for (int k = 0; k < 15; k++)
		{
			float pro_a_list[24];
			float pro_b_list[24];
			float a_vec[6];
			float b_vec[6];
			float C[3];

			if (k == 0)
				C[0] = A[0], C[1] = A[0], C[2] = A[2];
			else if (k == 1)
				C[0] = A[3], C[1] = A[4], C[2] = A[5];
			else if (k == 2)
				C[0] = A[6], C[1] = A[7], C[2] = A[8];
			else if (k == 3)
				C[0] = B[0], C[1] = B[0], C[2] = B[2];
			else if (k == 4)
				C[0] = B[3], C[1] = B[4], C[2] = B[5];
			else if (k == 5)
				C[0] = B[6], C[1] = B[7], C[2] = B[8];
			else
			{
				int  i = (k - 6) / 3;
				int  j = (k - 6) % 3;
				vector3DCrossProduct(&A[3 * i], &B[3 * j], C);
			}
			//each project for all pointers
			for (int i = 0; i < 8; i++)
			{
				float temp1[3] = { ver_a[3 * i] , ver_a[3 * i + 1] , ver_a[3 * i + 2] };
				float temp2[3] = { ver_b[3 * i] , ver_b[3 * i + 1] , ver_b[3 * i + 2] };
				vector3Dprojection(C, temp1, &pro_a_list[3 * i]);
				vector3Dprojection(C, temp2, &pro_b_list[3 * i]);
			}
			//max min detection for a
			vector3DmaxminFinder(pro_a_list, 8, &a_vec[3], &a_vec[0]);
			//max min detection for b
			vector3DmaxminFinder(pro_b_list, 8, &b_vec[3], &b_vec[0]);
			//check line intersecting
			if (!collinear_line_intersect(a_vec, b_vec))
				return false;
		}
		return true;
	}

	bool Sphere::body_set(float* vectors, unsigned int size)
	{
		float max[3];
		float min[3];
		vector3DmaxminFinder(vectors, size / 3, max, min);
		float a = max[0] - min[0];
		float b = max[1] - min[1];
		float c = max[2] - min[2];
		radius = sqrtf(a * a + b  * b  + c  * c);
		radius = radius * 0.28867513459; // I assume this is a cube so calculate the height of the cube then dividing by 2
		collision_vec[0] = min[0] + a/ 2;
		collision_vec[1] = min[1] + b/ 2;
		collision_vec[2] = min[2] + c/ 2;
		return true;
	}
	bool Sphere::body_pos(float* Trans, float* Scale, float* Rotation)
	{
		float xyz[3] = { 0,0,0 };
		memcpy(xyz, collision_vec, sizeof(float) * 3);
		ECOLLISION::Translation3D_CPU(xyz, Trans, Scale, Rotation,1);
		memcpy(collision_vec, xyz,  sizeof(float) * 3);
		new_radius = radius * (Scale[0] +Scale[1] + Scale[2])/3;
		return true;
	}
	bool Sphere::collision_two_sphere(Sphere* trg)
	{
		float dis = sqrtf(
			(trg->collision_pos[0] - this->collision_pos[0]) * (trg->collision_pos[0] - this->collision_pos[0])
			+ (trg->collision_pos[1] - this->collision_pos[1]) * (trg->collision_pos[1] - this->collision_pos[1])
			+ (trg->collision_pos[2] - this->collision_pos[2]) * (trg->collision_pos[2] - this->collision_pos[2]));
		if (dis <= this->new_radius + trg->new_radius)
			return true;
		return false;
	}

	/*
	bool Rotationf(float* xyz, float angle, float x, float y, float z)
	{
		float c = cos(angle * DEG2RAD);
		float s = sin(angle * DEG2RAD);
		float matrix[9] =
		{
			x* x* (1 - c) + c,		x* y* (1 - c) - z * s,	x* z* (1 - c) + y * s,
			y* x* (1 - c) + z * s,	y* y* (1 - c) + c,		y* z* (1 - c) - x * s,
			z* x* (1 - c) - y * s,	z* y* (1 - c) + x * s,	z* z* (1 - c) + c
		};
		float a = matrix[0] * xyz[0] + matrix[1] * xyz[1] + matrix[2] * xyz[2];
		float b = matrix[3] * xyz[0] + matrix[4] * xyz[1] + matrix[5] * xyz[2];
		float t = matrix[6] * xyz[0] + matrix[7] * xyz[1] + matrix[8] * xyz[2];

		xyz[0] = a;
		xyz[1] = b;
		xyz[2] = t;

		return true;
	}
	bool Trans_Scale_Rot(float* xyz, float* Trans, float* Scale, float* Rot)
	{
		xyz[0] *= Scale[0];
		xyz[1] *= Scale[1];
		xyz[2] *= Scale[2];
		//rotation
		Rotationf(xyz, Rot[2], 0, 1, 0);
		Rotationf(xyz, Rot[1], 0, 0, 1);
		Rotationf(xyz, Rot[0], 1, 0, 0);

		xyz[0] += Trans[0];
		xyz[1] += Trans[1];
		xyz[2] += Trans[2];
			
		return true;
	}
	*/
	bool Translation3D_CPU(float* vectors, float* Trans, float* Scale, float* Rot, unsigned int size)
	{
		TMATRIX<float> trans(4, 4);
		TMATRIX<float> rot1(4, 4);
		TMATRIX<float> rot2(4, 4);
		TMATRIX<float> rot3(4, 4);
		TMATRIX<float> scale(4, 4);
		TMATRIX<float> temp(4, 1);

		float trans_mat[16] = {
			1,0,0,Trans[0],
			0,1,0,Trans[1],
			0,0,1,Trans[2],
			0,0,0,1
		};
		float scale_mat[16] ={
			Scale[0],0,0,0,
			0,Scale[1],0,0,
			0,0,Scale[2],0,
			0,0,0,1
		};
		float c = cos(Rot[0] * DEG2RAD);
		float s = sin(Rot[0] * DEG2RAD);
		float x = 1, y = 0, z = 0;
		float matrix1[16] =	{
			x * x * (1 - c) + c,		x * y * (1 - c) - z * s,	x * z * (1 - c) + y * s,	0,
			y * x * (1 - c) + z * s,	y * y * (1 - c) + c,		y * z * (1 - c) - x * s,	0,
			z * x * (1 - c) - y * s,	z * y * (1 - c) + x * s,	z * z * (1 - c) + c,		0,
			0,							0,							0,							1
		};
		c = cos(Rot[1] * DEG2RAD);
		s = sin(Rot[1] * DEG2RAD);
		x = 0, y = 0, z = 1;
		float matrix2[16] =	{
			x * x * (1 - c) + c,		x * y * (1 - c) - z * s,	x * z * (1 - c) + y * s,	0,
			y * x * (1 - c) + z * s,	y * y * (1 - c) + c,		y * z * (1 - c) - x * s,	0,
			z * x * (1 - c) - y * s,	z * y * (1 - c) + x * s,	z * z * (1 - c) + c,		0,
			0,							0,							0,							1
		};
		c = cos(Rot[2] * DEG2RAD);
		s = sin(Rot[2] * DEG2RAD);
		x = 0, y = 1, z = 0;
		float matrix3[16] =	{
			x * x * (1 - c) + c,		x * y * (1 - c) - z * s,	x * z * (1 - c) + y * s,	0,
			y * x * (1 - c) + z * s,	y * y * (1 - c) + c,		y * z * (1 - c) - x * s,	0,
			z * x * (1 - c) - y * s,	z * y * (1 - c) + x * s,	z * z * (1 - c) + c,		0,
			0,							0,							0,							1
		};
		
		trans = trans_mat;
		scale = scale_mat;
		rot1 = matrix1;
		rot2 = matrix2;
		rot3 = matrix3;
#pragma omp parallel for shared(trans, scale, rot1, rot2, rot3)
		for (int i = 0; i < size; i++)
		{
			float vec4[4] = { vectors[3 * i], vectors[3 * i + 1], vectors[3 * i + 2], 1 };
			temp = vec4;
			TMATRIX<float>* temp1 = (scale * temp);
			TMATRIX<float>* temp2 = (rot3 * *temp1);
			TMATRIX<float>* temp3 = (rot2 * *temp2);
			TMATRIX<float>* temp4 = (rot1 * *temp3);
			TMATRIX<float>* temp5 = (trans * *temp4);

			vectors[3 * i] = temp5->vectors[0];
			vectors[3 * i + 1] = temp5->vectors[1];
			vectors[3 * i + 2] = temp5->vectors[2];

			free(temp1->vectors);
			free(temp2->vectors);
			free(temp3->vectors);
			free(temp4->vectors);
			free(temp5->vectors);
			free(temp1);
			free(temp2);
			free(temp3);
			free(temp4);
			free(temp5);
		}
		return true;
	}
	bool vector3DCrossProduct(float* A, float* B, float* C)
	{
		//Skew-symmetric matrix
		float a[9] = {
			0,-A[2],A[1],
			A[2],0,-A[0],
			-A[1],A[0],0
		};
		float b[3] = {
			B[0],B[1],B[2]
		};
		TMATRIX<float> tempA(3, 3);
		TMATRIX<float> tempB(3, 1);
		tempA = a;
		tempB = b;
		TMATRIX<float>* tempC = tempA * tempB;
		C[0] = tempC->vectors[0];
		C[1] = tempC->vectors[1];
		C[2] = tempC->vectors[2];
		free(tempC->vectors);
		free(tempC);
		return true;
	}
	bool point_in_R3(float p[3], float a[3], float b[3], float d[3], float e[3])
	{
		float ap[3] = { (p[0] - a[0]) , (p[1] - a[1])  , (p[2] - a[2]) };
		if (ap[0] > 0 && ap[1] > 0 && ap[2])
		{
			//they are orthogonal
			//unit vector
			float dab = sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]) + (b[2] - a[2]) * (b[2] - a[2]));
			float dad = sqrtf((d[0] - a[0]) * (d[0] - a[0]) + (d[1] - a[1]) * (d[1] - a[1]) + (d[2] - a[2]) * (d[2] - a[2]));
			float dae = sqrtf((e[0] - a[0]) * (e[0] - a[0]) + (e[1] - a[1]) * (e[1] - a[1]) + (e[2] - a[2]) * (e[2] - a[2]));
			float dap = sqrtf((p[0] - a[0]) * (p[0] - a[0]) + (p[1] - a[1]) * (p[1] - a[1]) + (p[2] - a[2]) * (p[2] - a[2]));
			float ab[3] = { (b[0] - a[0]) / dab , (b[1] - a[1]) / dab , (b[2] - a[2]) / dab };
			float ad[3] = { (d[0] - a[0]) / dad , (d[1] - a[1]) / dad , (d[2] - a[2]) / dad };
			float ae[3] = { (e[0] - a[0]) / dae , (e[1] - a[1]) / dae , (e[2] - a[2]) / dae };
			float ap[3] = { (p[0] - a[0]) , (p[1] - a[1])  , (p[2] - a[2]) };
			//inner product
			float abp[3] = { ab[0] * ap[0], ab[1] * ap[1], ab[2] * ap[2] };
			float adp[3] = { ad[0] * ap[0], ad[1] * ap[1], ad[2] * ap[2] };
			float aep[3] = { ae[0] * ap[0], ae[1] * ap[1], ae[2] * ap[2] };
			float in_abp = abp[0] + abp[1] + abp[2];
			float in_adp = adp[0] + adp[1] + adp[2];
			float in_aep = aep[0] + aep[1] + aep[2];
			//projection
			float pabp[3] = { in_abp * ab[0], in_abp * ab[1], in_abp * ab[2] };
			float padp[3] = { in_adp * ad[0], in_adp * ad[1], in_adp * ad[2] };
			float paep[3] = { in_aep * ae[0], in_aep * ae[1], in_aep * ae[2] };
			//length
			float l_pabp = sqrtf(pabp[0] * pabp[0] + pabp[1] * pabp[1] + pabp[2] * pabp[2]);
			float l_padp = sqrtf(padp[0] * padp[0] + padp[1] * padp[1] + padp[2] * padp[2]);
			float l_paep = sqrtf(paep[0] * paep[0] + paep[1] * paep[1] + paep[2] * paep[2]);
			if (l_pabp < dab && l_padp < dad && l_paep < dae)
				return true;
		}
		return false;
	}
	bool collinear_line_intersect(float A[6], float B[6])
	{
		if ((A[3] >= B[0] && A[4] >= B[1] && A[5] >= B[2])
			&& (B[3] >= A[0] && B[4] >= A[1] && B[5] >= A[2]))
			return true;
		/*
		else if (A[3] == B[0] && A[4] == B[1] && A[5] == B[2])
			return true;
		else if (B[3] == A[0] && B[4] == A[1] && B[5] == A[2])
			return true;
			*/
		else
			return false;
	}
	bool vector3DmaxminFinder(float* vec, unsigned int size, float* max, float* min)
	{
		float max_x = vec[0];
		float max_y = vec[1];
		float max_z = vec[2];
		float min_x = vec[0];
		float min_y = vec[1];
		float min_z = vec[2];
		for (int i = 1; i < size; i++)
		{
			max_x = (vec[3 * i] > max_x) ?		 vec[3 * i] : max_x;
			max_y = (vec[3 * i + 1] > max_y) ?	 vec[3 * i + 1] : max_y;
			max_z = (vec[3 * i + 2] > max_z) ?	 vec[3 * i + 2] : max_z;
			min_x = (vec[3 * i] < min_x) ?		 vec[3 * i] : min_x;
			min_y = (vec[3 * i + 1] < min_y) ?	 vec[3 * i + 1] : min_y;
			min_z = (vec[3 * i + 2] < min_z) ?	 vec[3 * i + 2] : min_z;

		}
		max[0] = max_x;
		max[1] = max_y;
		max[2] = max_z;

		min[0] = min_x;
		min[1] = min_y;
		min[2] = min_z;
		return true;
	}
	bool vector3Dprojection(float* trg, float* src, float* result)
	{
		float dis = sqrtf(trg[0] * trg[0] + trg[1] * trg[1] + trg[2] * trg[2]);
		dis = (dis <=  0) ? 1 : dis;
		float unit[3] = { trg[0] / dis, trg[1] / dis , trg[2] / dis };
		float dot = unit[0] * src[0] + unit[1] * src[1] + unit[2] * src[2];
		result[0] = dot * unit[0];
		result[1] = dot * unit[1];
		result[2] = dot * unit[2];
		return true;
	}
}