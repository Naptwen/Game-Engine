import numpy as np
import os.path
from ctypes import *



class CHello(Structure):
    pass


CHello._fields_ = [
        ("object_size", c_int),
        ("vertices_list", POINTER(POINTER(c_double))),
        ("vertices_size", POINTER(c_uint)),
        ("normals_list", POINTER(POINTER(c_double))),
        ("normals_size", POINTER(c_uint)),
        ("text_coor_list", POINTER(POINTER(c_double))),
        ("text_coor_size", POINTER(c_uint)),
        ("ind_v_list", POINTER(POINTER(c_double))),
        ("ind_v_size", POINTER(c_uint)),
        ("ind_n_list", POINTER(POINTER(c_double))),
        ("ind_n_size", POINTER(c_uint)),
        ("ind_t_list", POINTER(POINTER(c_double))),
        ("ind_t_size", POINTER(c_uint)),
        ("mtllib", c_char_p,)
    ]

dll_name = "LOADER_OBJ.dll"
dllpath = os.path.dirname(os.path.abspath(__file__)) + os.path.sep + dll_name
mydll = cdll.LoadLibrary(dllpath)

mydll.CHello_new.argtypes = [c_void_p]
mydll.CHello_new.restype = POINTER(CHello)

mydll.CHello_loading.argtypes = [POINTER(CHello), c_char_p]
mydll.CHello_loading.restype = c_void_p


class cello():
    obj = POINTER(CHello)

    def __init__(self):
        self.obj = mydll.CHello_new(1)

    def loading(self, file_name):
        file_name = create_string_buffer(file_name.encode('utf-8'))
        mydll.CHello_loading(self.obj, file_name)
        for obj in range(self.obj.object_size):
            vec_size = self.obj[0].vertices_size[0]
            print(self.obj[0].vertices_list[0][0:vec_size])
            
