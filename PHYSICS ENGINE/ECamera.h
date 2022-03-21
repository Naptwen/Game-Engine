#include <cmath>
#include "TMATRIX.h"
class ECamera {
public:
    float e[3] = { 0,0,0 };
    float c[3] = { 1,1,1 };
    float w[3] = { 0,1,0 };
    int camera_w = 0;
    int camera_h = 0;
    float zoom = 1;
    float d = 0;
    float pov = 1000;
    void Point_in_tri(int p[2], int v1[2], int v2[2], int v3[2]) {
        int AB[2] = { v2[0] - v1[0], v2[1] - v1[1] };
        int BC[2] = { v3[0] - v2[0], v3[1] - v2[1] };
        int CA[2] = { v1[0] - v3[0], v1[1] - v3[1] };
        printf("[%d, %d], [%d, %d], [%d, %d]\n", AB[0],AB[1], BC[0],BC[1],CA[0],CA[1]);
        int crossB = AB[0] * (v2[1] - p[1]) - (v2[0] - p[0]) * AB[1];
        int crossC = BC[0] * (v3[1] - p[1]) - (v3[0] - p[0]) * BC[1];
        int crossA = CA[0] * (v1[1] - p[1]) - (v1[0] - p[0]) * CA[1];
        printf("%d, %d, %d\n", crossB, crossC, crossA);
        if (crossB <= 0 && crossC <= 0 && crossA <= 0)
            printf("inside");
        else
            printf("outside");
    }
    float POV_CAL(float screen_z) {
        return sqrt((screen_z * screen_z + pov)) - sqrt(pov);
    }
    void Camera_pos(float vector[], float new_vector[]) {
        d = sqrt((e[0] - c[0]) * (e[0] - c[0]) + (e[1] - c[1]) * (e[1] - c[1]) + (e[2] - c[2]) * (e[2] - c[2]));
        float u[3] = { (c[0] - e[0]) / d, (c[1] - e[1]) / d, (c[2] - e[2]) / d };
        float v[3] = { u[1] * w[2] - u[2] * w[1],-(u[0] * w[2] - u[2] * w[0]), u[0] * w[1] - u[1] * w[0] };
        float t[3] = { v[1] * u[2] - v[2] * u[1],-(v[0] * u[2] - v[2] * u[0]), v[0] * u[1] - v[1] * u[0] };
        float k[3] = { u[0],u[1],u[2] };

        float screen_x = v[0] * vector[0] + v[1] * vector[1] + v[2] * vector[2];
        float screen_y = t[0] * vector[0] + t[1] * vector[1] + t[2] * vector[2];
        float screen_z = k[0] * vector[0] + k[1] * vector[1] + k[2] * vector[2];
        // using 10y^2 = x^2 + 100 euqation make smooth curve of the pov;
        float pov = POV_CAL(screen_z);
        float pov_x = screen_x * zoom * pov ;
        float pov_y = screen_y * zoom * pov ;
        new_vector[0] = pov_x;
        new_vector[1] = -pov_y;
        new_vector[2] = screen_z;
    }
    void Camera_matrix(float* vectors, float* new_vectors, int size) {
        #pragma omp parallel for //collapse(4)
        for (int i = 0; i < size; i+=3) {
            float ptr[] = { vectors[i], vectors[i + 1],vectors[i + 2] };
            float new_ptr[3];
            Camera_pos(ptr, new_ptr);
            new_vectors[i] = new_ptr[0];
            new_vectors[i + 1] = new_ptr[1];
            new_vectors[i + 2] = new_ptr[2];
        }
    }
    TMATRIX<float>* Camera_Tmatrix(TMATRIX<float>* vectors)
    {
        d = sqrt((e[0] - c[0]) * (e[0] - c[0]) + (e[1] - c[1]) * (e[1] - c[1]) + (e[2] - c[2]) * (e[2] - c[2]));
        float u[3] = { (c[0] - e[0]) / d, (c[1] - e[1]) / d, (c[2] - e[2]) / d };
        float v[3] = { u[1] * w[2] - u[2] * w[1],-(u[0] * w[2] - u[2] * w[0]), u[0] * w[1] - u[1] * w[0] };
        float t[3] = { v[1] * u[2] - v[2] * u[1],-(v[0] * u[2] - v[2] * u[0]), v[0] * u[1] - v[1] * u[0] };
        float k[3] = { u[0],u[1],u[2] };
        float matrix[9] = {v[0],v[1],v[2],t[0],t[1],t[2],k[0],k[1],k[2]};
        TMATRIX<float> tmatrix(3,3);
        tmatrix = matrix;
        TMATRIX<float>* result_matrix = tmatrix * (*vectors);
        int n = result_matrix->n;
        for (int i = 0; i < n; i++)
        {
            float screen_x = result_matrix->vectors[i];
            float screen_y = result_matrix->vectors[i + n];
            float screen_z = result_matrix->vectors[i + 2 * n];
            float pov = POV_CAL(screen_z);
            float pov_x = screen_x * zoom * pov;
            float pov_y = screen_y * zoom * pov;
            result_matrix->vectors[i] = pov_x;
            result_matrix->vectors[i + n] = -pov_y;
            result_matrix->vectors[i + 2 * n] = 0;
        }
        return result_matrix;
    }
};