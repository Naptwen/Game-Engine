import numpy as np
import os.path
from ctypes import *



class CHello(Structure):
    pass


CHello._fields_ = [
        ("sub_count", c_uint),
        ("vertices_list", POINTER(POINTER(c_double))),
        ("vertices_size", POINTER(c_uint)),
        ("normals_list", POINTER(POINTER(c_double))),
        ("normals_size", POINTER(c_uint)),
        ("text_coor_list", POINTER(POINTER(c_double))),
        ("text_coor_size", POINTER(c_uint)),
        ("ind_v_list", POINTER(POINTER(c_uint))),
        ("ind_v_size", POINTER(c_uint)),
        ("ind_n_list", POINTER(POINTER(c_uint))),
        ("ind_n_size", POINTER(c_uint)),
        ("ind_t_list", POINTER(POINTER(c_uint))),
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


class Loader:
    obj = POINTER(CHello)
    sub_count = 0
    vertices = []
    normals = []
    ind_v = []


    def __init__(self):
        self.obj = mydll.CHello_new(1)


    def loading(self, file_name):
        file_name = create_string_buffer(file_name.encode('utf-8'))
        mydll.CHello_loading(self.obj, file_name)
        self.sub_count = self.obj[0].sub_count
        for index in range(self.obj[0].sub_count):
            vec_size = self.obj[0].vertices_size[index]
            new_vec = self.obj[0].vertices_list[index][0:vec_size]
            self.vertices.append(new_vec)

        for index in range(self.obj[0].sub_count):
            norm_size = self.obj[0].normals_size[index]
            new_norm = self.obj[0].normals_list[index][0:norm_size]
            self.normals.append(new_norm)

        for index in range(self.obj[0].sub_count):
            ind_v_size = self.obj[0].ind_v_size[index]
            new_ind_v = self.obj[0].ind_v_list[index][0:ind_v_size]
            self.ind_v.append(new_ind_v)

