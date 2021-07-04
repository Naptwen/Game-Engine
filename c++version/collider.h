#pragma once
#include "matrix.h"
vector<float> set_collision(matrix A) {
    vector<vector<float>> temp_vertices = A.list;
    float x_max = temp_vertices[0][0];
    float x_min = temp_vertices[0][0];
    float y_max = temp_vertices[0][1];
    float y_min = temp_vertices[0][1];
    float z_max = temp_vertices[0][2];
    float z_min = temp_vertices[0][2];
    for (const vector<float> vec : temp_vertices) {
        if (vec[0] > x_max)
            x_max = vec[0];
         if (vec[0] < x_min)
             x_min = vec[0];
         if (vec[1] > y_max)
             y_max = vec[1];
         if (vec[1] < y_min)
             y_min = vec[1];
         if (vec[2] > z_max)
             z_max = vec[2];
         if (vec[2] < z_min)
             z_min = vec[2];
    }
    return { x_max, x_min, y_max, y_min, z_max, z_min };
}
// transformaation matrix A is reuqired (4x4)
// set_collision vertices vector is required
matrix convert_AABB(vector<float> collision, matrix A) {
    matrix B, C, D, E;
    vector<float> ver_0 = {collision[1], collision[3], collision[5], 1 };
    vector<float> ver_1 = {collision[0], collision[3], collision[5], 1 };
    vector<float> ver_2 = {collision[1], collision[2], collision[5], 1 };
    vector<float> ver_3 = {collision[1], collision[3], collision[4], 1 };
    vector<float> ver_4 = {collision[0], collision[2], collision[5], 1 };
    vector<float> ver_5 = {collision[1], collision[2], collision[4], 1 };
    vector<float> ver_6 = {collision[0], collision[3], collision[4], 1 };
    vector<float> ver_7 = {collision[0], collision[2], collision[4], 1 };
    B + ver_0;
    B + ver_1;
    B + ver_2;
    B + ver_3;
    B + ver_4;
    B + ver_5;
    B + ver_6;
    B + ver_7;
    C = B.T();
    D = A * C;
    E = D.T();
    return E;
}
//         [5]        max
//          /--------/
//         / |      / |
//   [3]  / _|__[6]/__| [4]
//        | [2]    |  /
//        |/       | /
//    min / -------- / [1]
// A is the collision matrix which includes convert_AABB
// B is the transform matrix 4 x 4
bool OBB_collision(matrix A, matrix B) {
    vector<float> vec_x_coor = unit3(dir3(A.list[1], A.list[0]));
    vector<float> vec_y_coor = unit3(dir3(A.list[3], A.list[0]));
    vector<float> vec_z_coor = unit3(dir3(A.list[2], A.list[0]));
    float max_ver_x = len3(multi3(dot3(A.list[0], vec_x_coor) / dot3(vec_x_coor, vec_x_coor), vec_x_coor));
    float min_ver_x = len3(multi3(dot3(A.list[0], vec_x_coor) / dot3(vec_x_coor, vec_x_coor), vec_x_coor));
    for (const vector<float> vertex : A.list) {
        float temp_dis_x = len3(multi3(dot3(vertex, vec_x_coor) / dot3(vec_x_coor, vec_x_coor), vec_x_coor));
        if (temp_dis_x > max_ver_x) {
            max_ver_x = temp_dis_x;
        }
        if (temp_dis_x < min_ver_x) {
            min_ver_x = temp_dis_x;
        }
    }
    float max_ver_y = len3(multi3(dot3(A.list[0], vec_y_coor) / dot3(vec_y_coor, vec_y_coor), vec_y_coor));
    float min_ver_y = len3(multi3(dot3(A.list[0], vec_y_coor) / dot3(vec_y_coor, vec_y_coor), vec_y_coor));
    for (const vector<float> vertex : A.list) {
        float temp_dis_y = len3(multi3(dot3(vertex, vec_y_coor) / dot3(vec_y_coor, vec_y_coor), vec_y_coor));
        if (temp_dis_y > max_ver_y) {
            max_ver_y = temp_dis_y;
        }
        if (temp_dis_y < min_ver_y) {
            min_ver_y = temp_dis_y;
        }
    }    
    float max_ver_z = len3(multi3(dot3(A.list[0], vec_z_coor) / dot3(vec_z_coor, vec_z_coor), vec_z_coor));
    float min_ver_z = len3(multi3(dot3(A.list[0], vec_z_coor) / dot3(vec_z_coor, vec_z_coor), vec_z_coor));
    for (const vector<float> vertex : A.list) {
        float temp_dis_z = len3(multi3(dot3(vertex, vec_z_coor) / dot3(vec_z_coor, vec_z_coor), vec_z_coor));
        if (temp_dis_z > max_ver_z) {
            max_ver_z = temp_dis_z;
        }
        if (temp_dis_z < min_ver_z) {
            min_ver_z = temp_dis_z;
        }
    }
}

matrix transfrom_matrix(vector<float>T, vector<float>R, vector<float>S) {
    //translation matrix
    matrix A;
    vector<float> a1 = { 1,0,0,T[0] };
    vector<float> a2 = { 0,1,0,T[1] };
    vector<float> a3 = { 0,0,1,T[2] };
    vector<float> a4 = { 0,0,0,1 };
    A + a1;
    A + a2;
    A + a3;
    A + a4;
    //scale matrix
    matrix B;
    vector<float> b1 = { S[0],0,0,0 };
    vector<float> b2 = { 0,S[1],0,0 };
    vector<float> b3 = { 0,0,S[2],0 };
    vector<float> b4 = { 0,0,0,1 };
    A + a1;
    A + a2;
    A + a3;
    A + a4;

    matrix Rx;
    vector<float> rx1 = { 1,0,0,0 };
    vector<float> rx2 = { 0,cosf(R[0]), -sinf(R[0]),0 };
    vector<float> rx3 = { 0,sinf(R[0]), cosf(R[0]),0 };
    vector<float> rx4 = { 0,0,0,1 };
    Rx + rx1;
    Rx + rx2;
    Rx + rx3;
    Rx + rx4;

    matrix Ry;
    vector<float> ry1 = { cosf(R[1]),0,sinf(R[1]),0 };
    vector<float> ry2 = { 0,1, 0,0 };
    vector<float> ry3 = { -sinf(R[1]),0, cosf(R[1]),0 };
    vector<float> ry4 = { 0,0,0,1 };
    Ry + ry1;
    Ry + ry2;
    Ry + ry3;
    Ry + ry4;

    matrix Rz;
    vector<float> rz1 = { cosf(R[2]),-sinf(R[2]),0,0 };
    vector<float> rz2 = { sinf(R[2]),cosf(R[2]), 0,0 };
    vector<float> rz3 = { 0,0, 1,0 };
    vector<float> rz4 = { 0,0,0,1 };
    Rz + rz1;
    Rz + rz2;
    Rz + rz3;
    Rz + rz4;

    return A * (Rx * (Ry * (Rz * B)));
}
