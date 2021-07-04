import numpy as np


class MTL:

    amc = []
    dmc = []
    spec = []
    speE = []
    trans = []
    tranF = []
    opd = []

    def loading(self, file_name):
        for line in open(file_name, "r"):
            data = line.split()
            if data[0] == 'mtllib':
                mtl_name = data[1]
            elif data[0] == 'newmtl':  # name
                name = data[1]
                for line in open(file_name, "r"):
                    data = line.split()
                    if data[0] == 'Ka':  # ambient color
                        self.amc = map(float, data[1:3])
                    elif data[0] == 'Kd':  # diffuse color
                        self.dmc = map(float, data[1:3])
                    elif data[0] == 'Ks':  # specular color
                        self.spec = map(float, data[1:3])
                    elif data[0] == 'Ns':  # specular exponent
                        self.specE = data[1]
                    elif data[0] == 'd' or data[0] == 'Tr':  # transpaent
                        self.trans = data[1]
                    elif data[0] == 'Tf':  # Transmission Filter Color
                        if data[1] == 'xyz':
                            self.tranF = map(float, data[2:4])
                        elif data[1] == 'spectral':
                            pass
                    elif data[0] == 'Ni':  # optical density
                        self.opd = data[1]
                    elif data[0] == 'illum':
                        pass
                    elif data[0] == 'map_Ka':  # ambient color
                        _tamc = data[1]
                    elif data[0] == 'map_Kd':  # diffuse color
                        _tdmc = data[1]
                    elif data[0] == 'map_Ks':  # specular color
                        _tspec = data[1]
                    elif data[0] == 'map_Ns':  # specular exponent
                        _tspecE = data[1]
                    elif data[0] == 'map_d' or data[0] == 'Tr':  # transpaent
                        _ttrans = data[1]
                    elif data[0] == 'map_bump':  # Transmission Filter Color
                        _tbump = data[1]
                    elif data[0] == 'disp':  # optical density
                        _tdisp = data[1]
                    elif data[0] == 'decal':
                        pass


class OBJ:

    vertices = []
    normals = []
    texture = []
    ind_v = []
    ind_n = []
    ind_t = []
    _obj = []
    _gorup = []
    _mtl = []
    _obj = []
    _group = []
    _mtllib = []
    _mtl = []

    def __init__(self, file_name):
        self.loading(file_name)

    def loading(self, file_name):
        self.vertices = []
        self.normals = []
        self.textures = []
        self.ind_v = []
        self.ind_n = []
        self.ind_t = []
        self._obj = []
        self._gorup = []
        self._mtllib = []
        try:
            for line in open(file_name, "r"):
                data = line.split()
                if len(data) > 1:
                    if data[0] == 'v':
                        vertex = [data[1], data[2], data[3]]
                        self.vertices.append(vertex)
                    elif data[0] == 'vt':
                        vertex = [data[1], data[2]]
                        self.textures.append(vertex)
                    elif data[0] == 'vn':
                        vertex = [data[1], data[2], data[3]]
                        self.normals.append(vertex)
                    elif data[0] == 'f':
                        if data[1].count('//'):
                            for i in range(1, 4):
                                __data = data[i].split('//')
                                self.ind_v.append(int(__data[0])-1)
                                self.ind_n.append(int(__data[1])-1)
                        else:
                            count = data[1].count('/')
                            if count == 0:
                                self.ind_v.append(int(data[1])-1)
                                self.ind_v.append(int(data[2])-1)
                                self.ind_v.append(int(data[3])-1)
                            elif count == 1:
                                for i in range(1, 4):
                                    __data = data[i].split('/')
                                    self.ind_v.append(int(__data[0])-1)
                                    self.ind_t.append(int(__data[1])-1)
                            elif count == 2:
                                for i in range(1, 4):
                                    __data = data[i].split('/')
                                    self.ind_v.append(int(data[1])-1)
                                    self.ind_t.append(int(data[2])-1)
                                    self.ind_n.append(int(data[3])-1)
                    elif data[0] == 'mtllib':
                        self._mtllib.append(data[1])
                    elif data[0] == 'usemtl':
                        self._mtl.append(data[1])
                    elif data[0] == 'o':
                        self._obj.append(data[1])
                    elif data[0] == 'g':
                        self._group.append(data[1])
            return True
        except ValueError:
            print('file loading failed')
            return False
