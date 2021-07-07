#pragma once
#define DLLEXPORT extern "C" __declspec(dllexport)
#define UNITYPE char
#define uniI 2
#define uniC 3
#define uniF 5
#define uniF2 7
#define uniF3 11
#define uniS 13
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <malloc.h>

union __declspec(dllexport) element {
    char c;
    int i;
    unsigned int ui;
    float f;
};

class vec {
        element* data;
        char type = 0;
        unsigned int size = 0;
        unsigned int pos = 0;
    public:
        // uniC | uniI | uniF | uniF2 | uniF3 | uniS
        vec(UNITYPE type);
        void add(element item);
        void print();
        void operator = (const vec temp) {
            type = temp.type;
            size = temp.size;
            pos = temp.pos;
            if (data != nullptr)
                free(data);
            data = (element*)std::malloc(sizeof(element) * size);
            memcpy(data, temp.data, sizeof(element) * size);

        }
        ~vec();
    };

vec::vec(UNITYPE type) {
        this->type = type;
        data = (element*)malloc(sizeof(element));
        size = 1;
    }
void vec::add(element item) {
        if (pos >= size - 1) {
            data = (element*)realloc(data, 2 * size * sizeof(element));
            size *= 2;
        }
        data[pos] = item;
        pos++;
    }
void vec::print() {
        if (type == uniF) {
            std::printf("float %u \n", pos);
            for (int i = 0; i < pos; i++)
                std::printf("[%f]\n", data[i].f);
        }
        else if (type == uniI) {
            std::printf("Int %u \n", pos);
            for (int i = 0; i < pos; i++)
                std::printf("[%u]\n", data[i].ui);
        }

    }
vec::~vec() {
    }

struct __declspec(dllexport) objData {
    vec vertices = vec(uniF);
    vec text_coor = vec(uniF);
    vec normals = vec(uniF);
    vec ind_v = vec(uniI);
    vec ind_n = vec(uniI);
    vec ind_t = vec(uniI);
    void operator = (const objData obj) {
        vec vertices = obj.vertices;
        vec text_coor = obj.text_coor;
        vec normals = obj.normals;
        vec ind_v = obj.ind_v;
        vec ind_n = obj.ind_n;
        vec ind_t = obj.ind_t;
    }
};
DLLEXPORT bool loading(std::vector<objData>*, const char* const);

DLLEXPORT unsigned int count(char* text, const char* Delimiter);

DLLEXPORT unsigned int count(char* text, const char* Delimiter) {
    char* copy_txt = (char*)malloc(255 * sizeof(char));
    strcpy(copy_txt, text);
    char* token = strtok(copy_txt, Delimiter);
    unsigned int i = 0;
    while (token != NULL) {
        token = strtok(NULL, Delimiter);
        i++;
    }
    free(token);
    free(copy_txt);
    return i - 1;
}
// the only left c++ is the vector<objData> part to make pure c, instead of vector using dyanmaic memory by malloc.
DLLEXPORT bool loading(std::vector<objData>* list, const char* const file_name) {
    std::printf("loading\n");
    char line[255];
    FILE* myfile;
    myfile = fopen(file_name, "r");
    objData temp;
    while (fgets(line, 255, myfile)) {
            char* data = strtok(line, " ");
            if (!(strcmp(data, "v"))) {
                char* data1 = strtok(NULL, " ");
                char* data2 = strtok(NULL, " ");
                char* data3 = strtok(NULL, " ");
                union element ptr1;
                ptr1.f = atof(data1);
                union element ptr2;
                ptr2.f = atof(data2);
                union element ptr3;
                ptr3.f = atof(data3);
                temp.vertices.add(ptr1);
                temp.vertices.add(ptr2);
                temp.vertices.add(ptr3);
            }
            else if (!(strcmp(data, "vt"))) {
                char* data1 = strtok(NULL, " ");
                char* data2 = strtok(NULL, " ");
                union element ptr1;
                ptr1.f = atof(data1);
                union element ptr2;
                ptr2.f = atof(data2);
                temp.text_coor.add(ptr1);
                temp.text_coor.add(ptr2);
            }
            else if (!(strcmp(data, "vn"))) {
                char* data1 = strtok(NULL, " ");
                char* data2 = strtok(NULL, " ");
                char* data3 = strtok(NULL, " ");
                union element ptr1;
                ptr1.f = atof(data1);
                union element ptr2;
                ptr2.f = atof(data2);
                union element ptr3;
                ptr3.f = atof(data3);
                temp.normals.add(ptr1);
                temp.normals.add(ptr2);
                temp.normals.add(ptr3);
            }
            else if (!(strcmp(data, "f"))) {
                char* left_txt = strtok(NULL, "");
                unsigned int dslash = 0;
                unsigned int slash = count(left_txt, "/");
                if (dslash == 0 && slash == 0) {
                    char* data1 = strtok(NULL, " ");
                    char* data2 = strtok(NULL, " ");
                    char* data3 = strtok(NULL, " ");
                    union element ptr1 = { atoi(data1) - 1 };
                    union element ptr2 = { atoi(data2) - 1 };
                    union element ptr3 = { atoi(data3) - 1 };
                    temp.ind_v.add(ptr1);
                    temp.ind_v.add(ptr2);
                    temp.ind_v.add(ptr3);
                }
                else if (dslash > 0) {
                    char* data2 = strtok(left_txt, " ");
                    char* token = strtok(data2, "//");
                    while (data != NULL) {
                        union element ptr1 = { atoi(token) - 1 };
                        token = strtok(NULL, "//");
                        union element ptr2 = { atoi(token) - 1 };
                        temp.ind_v.add(ptr1);
                        temp.ind_n.add(ptr2);;
                    }
                    free(token);
                }
                else {
                    char* data2 = strtok(left_txt, " ");
                    while (data2 != NULL) {
                        char* data3 = strtok(NULL, "");
                        char* token = strtok(data2, "/");
                        union element ptr1;
                        ptr1.ui = atoi(token) - 1;
                        token = strtok(NULL, "/");
                        union element ptr2;
                        ptr2.ui = atoi(token) - 1;
                        token = strtok(NULL, "/");
                        union element ptr3;
                        ptr3.ui = atoi(token) - 1;
                        temp.ind_v.add(ptr1);
                        temp.ind_t.add(ptr2);
                        temp.ind_n.add(ptr3);
                        data2 = strtok(data3, " ");
                    }
                }
            }
            else if (!(strcmp(data, "mtllib"))) {
                std::printf("mtllib\n");
            }
            else if (!(strcmp(data, "o"))) {
                std::printf("add object\n");
            }
        }
    fclose(myfile);
    list->push_back(temp);
    std::printf("\nfinishing\n");
    return 1;
}

