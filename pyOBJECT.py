import copy
import math
from OpenGL.GL import *
from OpenGL.GLU import *
from pyObj import *
import numpy as np
from math import *
import sys

class OBJECT:
    obj_file = None
    object_ID = None
    color = [1, 1, 1, 1]
    origin = [0, 0, 0]
    rotX = 0
    rotY = 0
    rotZ = 0
    size = [1, 1, 1]
    script_list = list()
    vertices = []
    normals = []
    indices = []
    textures = []


    collision = []
    collision_Type = None
    collision_state = False

    def __init__(self):
        self.origin = np.array([0, 0, 0], dtype=np.float32)
        self.size = np.array([1, 1, 1], dtype=np.float32)

    def set_OBJ(self, file_name):
        self.obj_file = file_name

    def load_OBJ(self):
        if self.obj_file is not None:
            temp = OBJ(self.obj_file)
            self.vertices = np.array(temp.vertices, dtype = np.float32)
            self.normals = np.array(temp.normals, dtype = np.float32)
            self.textures = np.array(temp.textures, dtype = np.float32)
            self.indices = np.array(temp.ind_v, dtype = np.uint32)
        else:
            print('Need SET the object')

    def resize(self, vertices, vector3):
        size = np.array(vector3, dtype=np.float32)
        return vertices * size

    def mov(self, vertices, vector3):
        translate = np.array(vector3, dtype = np.float32)
        return vertices + translate

    def rot(self, vertices, vector3):
        tX = vector3[0]
        tY = vector3[1]
        tZ = vector3[2]
        temp_vertices= []
        for vertex in vertices:
            Rx = np.array([[1, 0, 0],
                 [0, cos(tX), -sin(tX)],
                 [0, sin(tX), cos(tX)]])
            Ry = np.array([[cos(tY), 0, sin(tY)],
                 [0, 1, 0],
                 [-sin(tY), 0, cos(tY)]])
            Rz = np.array([[cos(tZ), -sin(tZ), 0],
                 [sin(tZ), cos(tZ), 0],
                 [0, 0, 1]])
            temp_vertex = np.dot(Rx, np.dot(Ry, np.dot(Rz, np.array(vertex))))
            temp_vertices.append(temp_vertex)
        return temp_vertices

    def setRot(self, vector3):
        self.rotX = vector3[0]
        self.rotY = vector3[1]
        self.rotZ = vector3[2]

    def setColor4(self, vector4):
        self.color = vector4

    def vbo_draw(self):
        # VBO setup
        vbo_ver = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ver)
        glBufferData(GL_ARRAY_BUFFER, sys.getsizeof(self.vertices), self.vertices, GL_STATIC_DRAW)
        glBufferSubData(GL_ARRAY_BUFFER, 0, sys.getsizeof(self.vertices), self.vertices)
        glBindBuffer(GL_ARRAY_BUFFER, 0)
        # EBO setup
        vbo_ind = glGenBuffers(1)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind)
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sys.getsizeof(self.indices), self.indices, GL_STATIC_DRAW)
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sys.getsizeof(self.indices), self.indices)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

        #drawing
        glBindBuffer(GL_ARRAY_BUFFER, vbo_ver)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ind)
        glEnableClientState(GL_VERTEX_ARRAY)

        glPushMatrix()
        glColor4fv(self.color)
        glVertexPointer(3, GL_FLOAT, 0, None)
        glTranslatef(self.origin[0], self.origin[1], self.origin[2])
        glRotatef(self.rotX, 1, 0, 0)
        glRotatef(self.rotY, 0, 1, 0)
        glRotatef(self.rotZ, 0, 0, 1)
        glScalef(self.size[0], self.size[1], self.size[2])
        glDrawElements(GL_TRIANGLES, len(self.indices), GL_UNSIGNED_INT, None)
        glPopMatrix()

        glDisableClientState(GL_VERTEX_ARRAY)

        glBindBuffer(GL_ARRAY_BUFFER, 0)
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)

    def set_Collision(self):
        print('Collision SET')
        temp_vertices = copy.deepcopy(self.vertices)
        x_max = temp_vertices[0][0]
        x_min = temp_vertices[0][0]
        y_max = temp_vertices[0][1]
        y_min = temp_vertices[0][1]
        z_max = temp_vertices[0][2]
        z_min = temp_vertices[0][2]
        for vec in temp_vertices:
            if vec[0] > x_max:
                x_max = vec[0]
            if vec[0] < x_min:
                x_min = vec[0]
            if vec[1] > y_max:
                y_max = vec[1]
            if vec[1] < y_min:
                y_min = vec[1]
            if vec[2] > z_max:
                z_max = vec[2]
            if vec[2] < z_min:
                z_min = vec[2]
        self.collision = [x_max, x_min, y_max, y_min, z_max, z_min]

    def set_Collider_type(self,type):
        self.collision_Type = type
    #         [5]        max
    #          /--------/
    #         / |      / |
    #   [3]  / _|__[6]/__| [4]
    #        | [2]    |  /
    #        |/       | /
    #    min /--------/[1]
    def draw_AABB_box(self):
        if len(self.collision) >= 6:
            ver_min = [self.collision[1],self.collision[3],self.collision[5]]
            ver_max = [self.collision[0],self.collision[2],self.collision[4]]
            ver1 = [self.collision[0],self.collision[3],self.collision[5]]
            ver2 = [self.collision[1],self.collision[2],self.collision[5]]
            ver3 = [self.collision[1],self.collision[3],self.collision[4]]
            ver4 = [self.collision[0],self.collision[2],self.collision[5]]
            ver5 = [self.collision[1], self.collision[2], self.collision[4]]
            ver6 = [self.collision[0], self.collision[3], self.collision[4]]
            cube_line_vertices = [ver_min, ver1, ver_min, ver2, ver_min, ver3, ver_max, ver4, ver_max, ver5, ver_max, ver6,
                             ver3, ver6,ver3,ver5,ver2,ver4,ver1,ver4,ver1,ver6,ver2,ver5]
            glPushMatrix()
            glTranslatef(self.origin[0], self.origin[1], self.origin[2])
            glRotatef(self.rotX, 1, 0, 0)
            glRotatef(self.rotY, 0, 1, 0)
            glRotatef(self.rotZ, 0, 0, 1)
            glScalef(self.size[0], self.size[1], self.size[2])
            glBegin(GL_LINES)
            glColor3f(1, 0, 0)
            for ver in cube_line_vertices:
                glVertex3f(ver[0], ver[1], ver[2])
            glEnd()
            glPopMatrix()
    #converting the vertices by translate, rotation and scale
    def convert_AABB(self):
        glPushMatrix()
        glTranslatef(self.origin[0], self.origin[1], self.origin[2])
        glRotatef(self.rotX, 1, 0, 0)
        glRotatef(self.rotY, 0, 1, 0)
        glRotatef(self.rotZ, 0, 0, 1)
        glScalef(self.size[0], self.size[1], self.size[2])
        matrixf = glGetFloatv(GL_MODELVIEW_MATRIX)
        glPopMatrix()

        ver_0 = [self.collision[1], self.collision[3], self.collision[5]]
        ver_1 = [self.collision[0], self.collision[3], self.collision[5]]
        ver_2 = [self.collision[1], self.collision[2], self.collision[5]]
        ver_3 = [self.collision[1], self.collision[3], self.collision[4]]
        ver_4 = [self.collision[0], self.collision[2], self.collision[5]]
        ver_5 = [self.collision[1], self.collision[2], self.collision[4]]
        ver_6 = [self.collision[0], self.collision[3], self.collision[4]]
        ver_7 = [self.collision[0], self.collision[2], self.collision[4]]

        vertices_list = [ver_0, ver_1, ver_2,ver_3,ver_4,ver_5,ver_6,ver_7]
        new_vertices = []
        for vertex in vertices_list:
            verX = matrixf[0][0] * vertex[0] + matrixf[1][0] * vertex[1] + matrixf[2][0] * vertex[2] + matrixf[3][0]
            verY = matrixf[0][1] * vertex[0] + matrixf[1][1] * vertex[1] + matrixf[2][1] * vertex[2] + matrixf[3][1]
            verZ = matrixf[0][2] * vertex[0] + matrixf[1][2] * vertex[1] + matrixf[2][2] * vertex[2] + matrixf[3][2]
            newVer = [verX, verY, verZ]
            new_vertices.append(newVer)
        return new_vertices
    #converted vertices required
    def OBB_collision(self, vertices = list(), target_vertices = list()):
        vec_x_coor = np.array(self.unit_vec(np.array(vertices[1]) - np.array(vertices[0])))
        vec_y_coor = np.array(self.unit_vec(np.array(vertices[3]) - np.array(vertices[0])))
        vec_z_coor = np.array(self.unit_vec(np.array(vertices[2]) - np.array(vertices[0])))
        #projection on to the bases
        max_ver_x = np.dot(np.dot(vertices[0], vec_x_coor) / np.dot(vec_x_coor,vec_x_coor), (vec_x_coor))
        min_ver_x = np.dot(np.dot(vertices[0], vec_x_coor) / np.dot(vec_x_coor, vec_x_coor), (vec_x_coor))
        for vertex in vertices:
            temp_ver = np.dot(np.dot(vertex,vec_x_coor)/np.dot(vec_x_coor,vec_x_coor), (vec_x_coor))
            if self.vec_dis(temp_ver) > self.vec_dis(max_ver_x):
                max_ver_x = temp_ver
            if self.vec_dis(temp_ver) < self.vec_dis(min_ver_x):
                min_ver_x = temp_ver
        max_ver_y = np.dot(np.dot(vertices[0], vec_y_coor) / np.dot(vec_y_coor, vec_y_coor), (vec_y_coor))
        min_ver_y = np.dot(np.dot(vertices[0], vec_y_coor) / np.dot(vec_y_coor, vec_y_coor), (vec_y_coor))
        for vertex in vertices:
            temp_ver = np.dot(np.dot(vertex,vec_y_coor)/np.dot(vec_y_coor,vec_y_coor), (vec_y_coor))
            if self.vec_dis(temp_ver) > self.vec_dis(max_ver_y):
                max_ver_y = temp_ver
            if self.vec_dis(temp_ver) < self.vec_dis(min_ver_y):
                min_ver_y = temp_ver
        max_ver_z = np.dot(np.dot(vertices[0], vec_z_coor) / np.dot(vec_z_coor, vec_z_coor), (vec_z_coor))
        min_ver_z = np.dot(np.dot(vertices[0], vec_z_coor) / np.dot(vec_z_coor, vec_z_coor), (vec_z_coor))
        for vertex in vertices:
            temp_ver = np.dot(np.dot(vertex,vec_z_coor)/np.dot(vec_z_coor, vec_z_coor), (vec_z_coor))
            if self.vec_dis(temp_ver) > self.vec_dis(max_ver_z):
                max_ver_z = temp_ver
            if self.vec_dis(temp_ver) < self.vec_dis(min_ver_z):
                min_ver_z = temp_ver
        #comparing the target vertices are in projected line
        for vertex in target_vertices:
            project_x = np.dot(np.dot(vertex, vec_x_coor) / np.dot(vec_x_coor, vec_x_coor), (vec_x_coor))
            if np.linalg.norm(project_x) > np.linalg.norm(min_ver_x) and np.linalg.norm(project_x) < np.linalg.norm(max_ver_x):
                project_y = np.dot(np.dot(vertex, vec_y_coor) / np.dot(vec_y_coor, vec_y_coor), (vec_y_coor))
                if np.linalg.norm(project_y) > np.linalg.norm(min_ver_y) and np.linalg.norm(project_y) < np.linalg.norm(max_ver_y):
                    project_z = np.dot(np.dot(vertex, vec_z_coor) / np.dot(vec_z_coor, vec_z_coor), (vec_z_coor))
                    if np.linalg.norm(project_z) > np.linalg.norm(min_ver_z) and np.linalg.norm(project_z) < np.linalg.norm(max_ver_z):
                        return True
        return False

    def draw_Sphere(self):
        if len(self.collision) >= 2:
            ver_min = np.array([self.collision[1], self.collision[3], self.collision[5]])
            ver_max = np.array([self.collision[0], self.collision[2], self.collision[4]])
            radius = np.linalg.norm(ver_max - ver_min) / 2
            glPushMatrix()
            glTranslatef(self.origin[0], self.origin[1], self.origin[2])
            glRotatef(self.rotX, 1, 0, 0)
            glRotatef(self.rotY, 0, 1, 0)
            glRotatef(self.rotZ, 0, 0, 1)
            glScalef(self.size[0], self.size[1], self.size[2])
            glColor3f(1, 0, 0)
            sphere= gluNewQuadric()
            gluSphere(sphere, radius, 10, 10)
            glPopMatrix()

    def S2S_collision(self, opponent_obj):
        ver_min = np.array([self.collision[1], self.collision[3], self.collision[5]])
        ver_max = np.array([self.collision[0], self.collision[2], self.collision[4]])
        radius = np.linalg.norm(ver_max - ver_min) / 2
        ver_min2 = np.array([opponent_obj.collision[1], opponent_obj.collision[3], opponent_obj.collision[5]])
        ver_max2 = np.array([opponent_obj.collision[0], opponent_obj.collision[2], opponent_obj.collision[4]])
        radius2 = np.linalg.norm(ver_max2 - ver_min2) / 2
        if np.linalg.norm(self.origin - opponent_obj.origin) < radius + radius2:
            return True
        return False

    def vec_dis(self, vector3):
        return math.sqrt(vector3[0]**2 + vector3[1]**2 + vector3[2]**2)

    def unit_vec(self, vec):
        len = math.sqrt(vec[0] ** 2 + vec[1] ** 2 + vec[2] ** 2)
        if len != 0:
            nor = np.array([vec[0] / len, vec[1] / len, vec[2] / len])
        else:
            nor = np.array([0, 0, 0])
        return nor

    def openGL_draw(self):
        vertices = self.vertices * self.size
        vertices = vertices + self.origin

        glPushMatrix()

        glEnableClientState(GL_VERTEX_ARRAY)
        glEnableClientState(GL_NORMAL_ARRAY)

        glColor4fv([1,1,1,1])
        glVertexPointerf(vertices)
        glNormalPointerf(self.normals)

        glDrawElements(GL_TRIANGLES, len(self.indices), GL_UNSIGNED_INT, self.indices)
        glDisableClientState(GL_VERTEX_ARRAY)
        glDisableClientState(GL_NORMAL_ARRAY)

        glPopMatrix()
