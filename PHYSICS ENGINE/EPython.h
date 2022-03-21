#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif
#include <Python.h>
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif
#pragma once
#include <stdio.h>
#include <string>

using namespace std;
/////////////////////////////////////////////////////////////////////
//[s] - string, [i] -int, [f] -float [d]- double
//-example: RUN_PYTHON("foo", "main", "sif", "Hello world", 1, 1.2345)
//-It returns string type
//-If def_name is [main] it returns [NULL] and with no param
/////////////////////////////////////////////////////////////////////
PyObject* RUN_PYTHON(string python_name, string def_name, const char* types, ...);
string PyObj2str(PyObject* obj);
int PyObjint(PyObject* obj);