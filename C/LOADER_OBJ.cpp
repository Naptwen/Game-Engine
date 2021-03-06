#include "pch.h"
#include "LOADER_OBJ.h"


using namespace std;

unsigned int counting(char* text, const char* Delimiter) {
    char* copy_txt = (char*)malloc(255 * sizeof(char));
    strcpy(copy_txt, text);
    char* token = strtok(copy_txt, Delimiter);
    float i = 0;
    while (token != NULL) {
        printf("token = %s\n", token);
        token = strtok(NULL, Delimiter);
        i++;
    }
    free(token);
    free(copy_txt);
    return i - 1;
}

DLLEXPORT C_obj* CHello_new() {
    C_obj* chello = (C_obj*)malloc(sizeof(C_obj));
    std::printf("C++ : C_obj file created\n");;
    return chello;
}

template <typename T>
void doubleFree(T** ptr, int size) {
    if (ptr != nullptr) {
        for (int i = 0; i < size; i++)
            std::free(ptr[i]);
        std::free(ptr);
    }
}

DLLEXPORT void CHello_free(C_obj* temp) {
    doubleFree(temp->vertices_list, temp->sub_count);
    if (temp->vertices_size != nullptr) 
        free(temp->vertices_size);
    doubleFree(temp->normals_list, temp->sub_count);
    if (temp->normals_size != nullptr)
        free(temp->normals_size);
    doubleFree(temp->text_coor_list, temp->sub_count);
    if (temp->text_coor_size != nullptr)
        free(temp->text_coor_size);
    doubleFree(temp->ind_v_list, temp->sub_count);
    if (temp->ind_v_size != nullptr)
        free(temp->ind_v_size);    
    doubleFree(temp->ind_n_list, temp->sub_count);
    if (temp->ind_n_size != nullptr)
        free(temp->ind_n_size);
    doubleFree(temp->ind_t_list, temp->sub_count);
    if (temp->ind_t_size != nullptr)
        free(temp->ind_t_size);
}
DLLEXPORT void CHello_loading(C_obj* temp, const char* file_name) {
    std::printf("C++ : c_obj file try to load file %s\n", file_name);;
    char line[255];
    FILE* myfile;
    myfile = fopen(file_name, "r");
    std::vector<std::vector<double>> vertices_list = {};
    std::vector<std::vector<double>> normals_list = {};
    std::vector<std::vector<double>> text_coor_list = {};
    std::vector<std::vector<unsigned int>> ind_v_list = {};
    std::vector<std::vector<unsigned int>> ind_n_list = {};
    std::vector<std::vector<unsigned int>> ind_t_list = {};
    std::vector<double> vertices;
    std::vector<double> normals;
    std::vector<double> text_coor;
    std::vector<unsigned int> ind_v;
    std::vector<unsigned int> ind_n;
    std::vector<unsigned int> ind_t;
    unsigned int obj_counts = 0;
    while (fgets(line, 255, myfile)) {
        printf("text = %s\n", line);
        char* data = strtok(line, " ");
        if (!(strcmp(data, "v"))) {
            char* data1 = strtok(NULL, " ");
            char* data2 = strtok(NULL, " ");
            char* data3 = strtok(NULL, " ");
            double ptr1 = atof(data1);
            double ptr2 = atof(data2);
            double ptr3 = atof(data3);
            vertices.push_back(ptr1);
            vertices.push_back(ptr2);
            vertices.push_back(ptr3);
        }
        else if (!(strcmp(data, "vt"))) {
            char* data1 = strtok(NULL, " ");
            char* data2 = strtok(NULL, " ");
            double ptr1 = atof(data1);
            double ptr2 = atof(data2);
            text_coor.push_back(ptr1);
            text_coor.push_back(ptr2);
        }
        else if (!(strcmp(data, "vn"))) {
            char* data1 = strtok(NULL, " ");
            char* data2 = strtok(NULL, " ");
            char* data3 = strtok(NULL, " ");
            double ptr1 = atof(data1);
            double ptr2 = atof(data2);
            double ptr3 = atof(data3);
            normals.push_back(ptr1);
            normals.push_back(ptr2);
            normals.push_back(ptr3);
        }
        else if (!(strcmp(data, "f"))) {
            char* left_txt = strtok(NULL, "");
            if (strstr(left_txt, "//") != NULL) {
                char* data2 = strtok(left_txt, " ");
                while (data2 != NULL) {
                    char* data3 = strtok(NULL, "");
                    char* token = strtok(data2, "//");
                    int ptr1 = { atoi(token) - 1 };
                    token = strtok(NULL, "//");
                    int ptr2 = { atoi(token) - 1 };
                    ind_v.push_back(ptr1);
                    ind_n.push_back(ptr2);;
                    data2 = strtok(data3, " ");
                }
            }
            else if (strstr(left_txt, "/") != NULL){
                char* data2 = strtok(left_txt, " ");
                while (data2 != NULL) {
                    char* data3 = strtok(NULL, "");
                    char* token = strtok(data2, "/");
                    int ptr1 = atoi(token) - 1;
                    token = strtok(NULL, "/");
                    int ptr2 = atoi(token) - 1;
                    token = strtok(NULL, "/");
                    int ptr3 = atoi(token) - 1;
                    ind_v.push_back(ptr1);
                    ind_t.push_back(ptr2);
                    ind_n.push_back(ptr3);
                    data2 = strtok(data3, " ");
                }
            }
            else {
                char* data1 = strtok(left_txt, " ");
                char* data2 = strtok(NULL, " ");
                char* data3 = strtok(NULL, " ");
                int ptr1 = { atoi(data1) - 1 };
                int ptr2 = { atoi(data2) - 1 };
                int ptr3 = { atoi(data3) - 1 };
                ind_v.push_back(ptr1);
                ind_v.push_back(ptr2);
                ind_v.push_back(ptr3);
            }
        }
        else if (!(strcmp(data, "mtllib"))) {
            const char* data1 = (const char*) strtok(NULL, " ");
            temp->mtllib = data1;
        }
        else if (!(strcmp(data, "o")) || !(strcmp(data, "g"))) {
            if (vertices.size() > 0) {
                vertices_list.push_back(vertices);
                normals_list.push_back(normals);
                text_coor_list.push_back(text_coor);
                ind_v_list.push_back(ind_v);
                ind_n_list.push_back(ind_n);
                ind_t_list.push_back(ind_t);
                vertices.clear();
                normals.clear();
                text_coor.clear();
                ind_v.clear();
                ind_n.clear();
                ind_t.clear();
                obj_counts++;
            }
        }
    }
    vertices_list.push_back(vertices);
    normals_list.push_back(normals);
    text_coor_list.push_back(text_coor); 
    ind_v_list.push_back(ind_v); 
    ind_n_list.push_back(ind_n); 
    ind_t_list.push_back(ind_t); 
    obj_counts++;

    temp->sub_count = obj_counts;

    temp->vertices_list = (double**)malloc(vertices_list.size() * sizeof(double*));
    for (unsigned int i = 0; i < vertices_list.size(); i++)
        temp->vertices_list[i] = (double*)malloc(vertices_list[i].size() * sizeof(double));
    temp->vertices_size = (unsigned int*)malloc(vertices_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < vertices_list.size(); i++)
    {
        temp->vertices_size[i] = vertices_list[i].size();
        for (int j = 0; j < vertices_list[i].size(); j++)
            temp->vertices_list[i][j] = vertices_list[i][j];
    }
    
    temp->normals_list = (double**)malloc(normals_list.size() * sizeof(double*));
    for (unsigned int i = 0; i < normals_list.size(); i++)
        temp->normals_list[i] = (double*)malloc(normals_list[i].size() * sizeof(double));
    temp->normals_size = (unsigned int*)malloc(normals_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < normals_list.size(); i++)
    {
        temp->normals_size[i] = normals_list[i].size();
        for (int j = 0; j < normals_list[i].size(); j++)
            temp->normals_list[i][j] = normals_list[i][j];
    }

    temp->text_coor_list = (double**)malloc(text_coor_list.size() * sizeof(double*));
    for (unsigned int i = 0; i < text_coor_list.size(); i++)
        temp->text_coor_list[i] = (double*)malloc(text_coor_list[i].size() * sizeof(double));
    temp->text_coor_size = (unsigned int*)malloc(text_coor_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < text_coor_list.size(); i++)
    {
        temp->text_coor_size[i] = text_coor_list[i].size();
        for (int j = 0; j < text_coor_list[i].size(); j++)
            temp->text_coor_list[i][j] = text_coor_list[i][j];
    }

    temp->ind_v_list = (unsigned int**)malloc(ind_v_list.size() * sizeof(unsigned int*));
    for (unsigned int i = 0; i < ind_v_list.size(); i++)
        temp->ind_v_list[i] = (unsigned int*)malloc(ind_v_list[i].size() * sizeof(unsigned int));
    temp->ind_v_size = (unsigned int*)malloc(ind_v_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < ind_v_list.size(); i++)
    {
        temp->ind_v_size[i] = ind_v_list[i].size();
        for (int j = 0; j < ind_v_list[i].size(); j++)
            temp->ind_v_list[i][j] = ind_v_list[i][j];
            
    }

    temp->ind_n_list = (unsigned int**)malloc(ind_n_list.size() * sizeof(unsigned int*));
    for (unsigned int i = 0; i < ind_v_list.size(); i++)
        temp->ind_n_list[i] = (unsigned int*)malloc(ind_n_list[i].size() * sizeof(unsigned int));
    temp->ind_n_size = (unsigned int*)malloc(ind_n_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < ind_n_list.size(); i++)
    {
        temp->ind_n_size[i] = ind_n_list[i].size();
        for (int j = 0; j < ind_n_list[i].size(); j++)
            temp->ind_n_list[i][j] = ind_n_list[i][j];
    }

    temp->ind_t_list = (unsigned int**)malloc(ind_t_list.size() * sizeof(unsigned int*));
    for (unsigned int i = 0; i < ind_t_list.size(); i++)
        temp->ind_t_list[i] = (unsigned int*)malloc(ind_t_list[i].size() * sizeof(unsigned int));
    temp->ind_t_size = (unsigned int*)malloc(ind_t_list.size() * sizeof(unsigned int));
    for (unsigned int i = 0; i < ind_t_list.size(); i++)
    {
        temp->ind_t_size[i] = ind_t_list[i].size();
        for (int j = 0; j < ind_t_list[i].size(); j++)
            temp->ind_t_list[i][j] = ind_t_list[i][j];
    }

    fclose(myfile);
    std::printf("C++ : c_obj file load completed %s\n", file_name);
    return ;
}

