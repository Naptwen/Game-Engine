#include "EPython.h"

using namespace std;

int PyObjint(PyObject* obj)
{
    return _PyLong_AsInt(obj);
}
string PyObj2str(PyObject* obj)
{
    return strdup(PyBytes_AS_STRING(PyUnicode_AsEncodedString(PyObject_Str(obj), "UTF-8", "strict")));
}
PyObject* RUN_PYTHON(string python_name, string def_name, const char* types, ...)
{
    Py_Initialize();
    PyRun_SimpleString("import sys\nimport os \nsys.path.append(os.getcwd())"); // Editing the path
    //register the path
    if (def_name.c_str() != NULL)
    {        
        PyObject* pName = PyUnicode_FromString(python_name.c_str());//find name
        if (pName == NULL)
        {
            Py_DECREF(pName);
            printf("pN\n");
        }
        PyObject* pModule = PyImport_Import(pName);//find module by name
        if (pModule == NULL)
        {
            Py_DECREF(pModule);
            printf("pM\n");
        }
        if (pModule != NULL)
        {
            PyObject* pFunc = PyObject_GetAttrString(pModule, def_name.c_str());//find function name
            Py_DECREF(pModule);
            if (pFunc != NULL && PyCallable_Check(pFunc))//if function is working
            {
                PyObject* pArgs = PyList_New(0);//Tuple size n
                va_list str_list;
                va_start(str_list, types);
                int count = 0;
                while (types[count] != NULL)
                {
                    PyObject* pValue = NULL;
                    switch (types[count])
                    {
                    case 's':
                    {
                        const char* value = va_arg(str_list, const char*);
                        pValue = Py_BuildValue("s", value);
                    }
                    break;
                    case 'i':
                    {
                        int value = va_arg(str_list, int);
                        pValue = Py_BuildValue("i", value);
                    }
                    break;
                    case 'f':
                    {
                        float value = va_arg(str_list, float);
                        pValue = Py_BuildValue("f", value);
                    }
                    break;
                    case 'd':
                    {
                        double value = va_arg(str_list, double);
                        pValue = Py_BuildValue("D", value);
                    }
                    break;
                    case 'o':
                    {
                        PyObject* value = va_arg(str_list, PyObject*);
                        pValue = Py_BuildValue("O", value);
                    }
                    }
                    PyList_Append(pArgs, pValue);
                    Py_DECREF(pValue);
                    count++;
                }
                va_end(str_list);
                pArgs = PyList_AsTuple(pArgs);
                PyObject* result = PyObject_CallObject(pFunc, pArgs);//pfunction with parg values
                Py_DECREF(pArgs);                
                if (result != NULL)
                {
                    return result;
                    //PyObject* uni = PyUnicode_AsEncodedString(PyObject_Str(result), "UTF-8", "strict");//convert to unicode str then conver str to unicode
                    //Py_DECREF(uni);                   
                    //return strdup(PyBytes_AS_STRING(uni));//convert unicode to byte then conver to str
                }
            }
            else
            {
                Py_DECREF(pFunc);
                printf("pF\n");
            }
        }
        Py_Finalize();
        return NULL;
    }
    else
    {
        string name = python_name + ".py";    
        PyObject* obj = Py_BuildValue("s", name);
        FILE* file = _Py_fopen_obj(obj, "r+");
        if (file != NULL) {
            PyRun_SimpleFile(file, name.c_str());
        }
        Py_Finalize();
        return NULL;
    }
}