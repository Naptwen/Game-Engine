#pragma once
#include <vector>
#include <math.h>
using namespace std;

class matrix {
public:
	vector<vector<float>> list;

	void operator =(const matrix temp) {
		this->list = temp.list;
	}
	matrix operator * (const matrix temp) {
		try {
			if (this->list[0].size() == temp.list.size())
			{
				vector<vector<float>> temp_list;
				for (unsigned int i = 0; i < this->list.size(); i++) {
					vector<float> rows;
					for (unsigned int j = 0; j < temp.list[0].size(); j++) {
						float sum = 0;
						for (unsigned int k = 0; k < this->list[0].size(); k++) {
							sum += this->list[i][k] * temp.list[k][j];
						}
						rows.push_back(sum);
					}
					temp_list.push_back(rows);
				}
				matrix C;
				C.list = temp_list;
				return C;
			}
		}
		catch (exception e)
		{
			printf("Size missmatching");
			return temp;
		}
	}
	void operator + (const vector<float> temp) {
		if (this->list.size() == 0)
			this->list.push_back(temp);
		else
			if (temp.size() != this->list[0].size())
				printf("Vector size miss matching");
			else
				this->list.push_back(temp);
	}
	matrix operator + (const matrix temp) {
		matrix C;
		if(this->list.size() == temp.list.size() && this->list[0].size() == temp.list[0].size()){}
			for (unsigned int i = 0; i < this->list.size(); i++) {
				vector<float> vec;
				for (unsigned int j = 0; j < this->list[0].size(); j++) {
					vec.push_back(this->list[i][j] + temp.list[i][j]);
				}
				C.list.push_back(vec);
			}
			return C;
	}
	matrix operator - (const matrix temp) {
		matrix C;
		if (this->list.size() == temp.list.size() && this->list[0].size() == temp.list[0].size()) {}
		for (unsigned int i = 0; i < this->list.size(); i++) {
			vector<float> vec;
			for (unsigned int j = 0; j < this->list[0].size(); j++) {
				vec.push_back(this->list[i][j] - temp.list[i][j]);
			}
			C.list.push_back(vec);
		}
		return C;
	}
	matrix dot(const matrix temp) {
		matrix A = this->T();
		matrix B;
		B = A * temp;
		return B;
	}
	matrix T(){
		vector<vector<float>> temp_list;
		for (unsigned int i = 0; i < this->list[0].size(); i++) {
			vector<float> rows;
			for (unsigned int j = 0; j < this->list.size(); j++) {
				rows.push_back(this->list[j][i]);
			}
			temp_list.push_back(rows);
		}
		matrix C;
		C.list = temp_list;
		return C;
	}
	void flatten() {
		vector<vector<float>> temp_list = this->list;
		this->list.clear();
		vector<float> save;
		for (vector<float> rows : temp_list) {
			for (int i = 0; i < rows.size(); i++){
				save.push_back(rows[i]);
			}
		}
		this->list.push_back(save);
	}
	void show() {
		for (unsigned int i = 0; i < this->list.size(); i++) {
			printf("[");
			for (unsigned int j = 0; j < this->list[0].size(); j++) {
				printf("%f, ", this->list[i][j]);
			}
			printf("]\n");
		}
	}
};

vector<float> unit3(vector<float>A){
	float len = sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
	vector<float> nor;
	try {
		if (len != 0) {
			nor = { A[0] / len, A[1] / len, A[2] / len };
			return nor;
		}
	}
	catch (exception e){
		printf("Vector is divided by 0\n");
	}
	return A;
}

vector<float> cross3(vector<float> A, vector<float> B) {
	vector<float> C = { 0, 0, 0 };
	C[0] = A[1] * B[2] - A[2] * B[1];
	C[1] = A[2] * B[0] - A[0] * B[2];
	C[2] = A[0] * B[1] - A[1] * B[0];
	return C;
}

vector<float> dir3(vector<float> end, vector<float> start) {
	return { end[0] - start[0],end[1] - start[1],end[2] - start[2] };
}

vector<float> multi3(float val, vector<float> A) {
	return { A[0] * val, A[1] * val, A[2] * val };
}

float len3(vector<float> A){
	return sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
}

float dot3(vector<float> A, vector<float> B) {
	return A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
}


