#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#define CHARCSV 3
#define INTCSV 7
#define FLOATCSV 11
#define DOUBLECSV 23
#define STRINGCSV 29

#define WRITE 29
#define EDIT 47


//TYPE : CHARCSV, INTCSV, FLOATCSV, DOUBLECSV
void CSV_to_vector(std::string file_name, void* rec_data, char TYPE);
//MODE : WRITE, EDIT
void vector_to_CSV(std::vector<double>& decitions, std::string name, char MODE);