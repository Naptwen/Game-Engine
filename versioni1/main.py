import sys
import csv
import time
import copy
from pyOBJECT import *
from pyPathFInder import *
from pyCamera import *
from pyObj import *
from os.path import *

from OpenGL.GL import *
from OpenGL.GL import shaders
from OpenGL.GLU import *
from OpenGL.arrays import vbo

from PyQt5 import QtCore
from PyQt5 import uic
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtOpenGL import *

form_class = uic.loadUiType("py_ui.ui")[0]


class MyWindow(QMainWindow, form_class):
    eye = np.array([0, 0, 0])
    camera = np.array([10, 10, 10])
    zoom = 0.8
    finish = True
    cell = []
    vertex_list = []

    object_list = []
    obstacle_list = []
    way_list = []
    path_list = []

    mouse = False
    click_1 = False #obstacle
    click_2 = False #moving
    click_3 = False #object

    loaded_obj = None
    loaded_script = None

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setupUi(self)
        self.openGLWidget.initializeGL = self.initializeGL
        self.openGLWidget.resizeGL = self.resizeGL
        self.openGLWidget.paintGL = self.paintGL

        self.moveButton.setCheckable(True)
        self.moveButton.setStyleSheet("background-color : lightgreen")
        self.obstacleButton.setCheckable(True)
        self.obstacleButton.setStyleSheet("background-color : lightgrey")
        self.objectButton.setCheckable(True)
        self.objectButton.setStyleSheet("background-color : lightgrey")

        self.Show_Ray.setChecked(False)
        self.Show_Obstacle.setChecked(False)
        self.Show_Objects.setChecked(True)

        print('Initializing finished')

    def initializeGL(self):
        glClearColor(0.2, 0.2, 0.2, 1)
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        # glEnable(GL_LIGHTING);
        # glEnable(GL_LIGHT0);
        # glLightfv(GL_LIGHT0, GL_POSITION, [0,0,10,1]);
        # self.object2 = OBJECT('model/teddy.obj')w
        self.cell = Cell_Table(100, 100)
        print('GL initialized compelete')

    def resizeGL(self, w, h):
        glClearColor(0.8, 0.8, 0.8, 1)
        glViewport(0, 0,  w,  h)
        glMatrixMode(GL_PROJECTION)
        glLoadIdentity()
        gluPerspective(45, w/h, 0.1, 1000)
        glDepthFunc(GL_LESS)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
        glDepthFunc(GL_ALWAYS)

    def paintGL(self):

        w = self.openGLWidget.width()
        h = self.openGLWidget.height()
        self.openGLWidget.resizeGL(w, h)

        glMatrixMode(GL_MODELVIEW)
        glEnable(GL_DEPTH_TEST)
        glLoadIdentity()

        gluLookAt(
            self.camera[0], self.camera[1], self.camera[2],
            self.eye[0], self.eye[1], self.eye[2],
            0.0, 0.0, 1.0)

        self.draw_base()
        self.draw_objects()
        glFlush()

        self.mouseChecking()
        self.openGLWidget.update()

    def keyPressEvent(self, e):
        KEY = e.key()
        if KEY == 87:  # w
            [self.camera, self.eye] = camera_mov(self.camera, self.eye, 0, 0.4)
        elif KEY == 65:  # a
            [self.camera, self.eye] = camera_mov(self.camera, self.eye, -0.4, 0)
        elif KEY == 83:  # s
            [self.camera, self.eye] = camera_mov(self.camera, self.eye, 0, -0.4)
        elif KEY == 68:  # d
            [self.camera, self.eye] = camera_mov(self.camera, self.eye, 0.4, 0)
        elif KEY == 81:  # d
            self.camera = camera_rot_xy(self.camera, self.eye, 5)
        elif KEY == 69:  # d
            self.camera = camera_rot_xy(self.camera, self.eye, -5)

    def mousePressEvent(self, event):
        if event.buttons() == QtCore.Qt.RightButton:
            event.accept()
            self.mouseX = event.pos().x()
            self.mouseY = event.pos().y()
            self.mouse = True
        elif event.buttons() == QtCore.Qt.MiddleButton:
            self.mouseX = event.pos().x()
            self.mouseY = event.pos().y()
            event.accept()

    def wheelEvent(self, e):
        vec = self.eye - self.camera
        if e.angleDelta().y() < 0:
            self.camera = self.camera + 0.1* vec
            self.eye = self.eye + 0.1 * vec
        else:
            self.camera = self.camera - 0.1 * vec
            self.eye = self.eye - 0.1 * vec

    def load_slot(self):
        file_name = QFileDialog.getOpenFileName(self,"Open obj", "./model/", "Image Files (*.obj)")
        self.loadFiletext.setText(file_name[0].split('/')[-1])
        self.loaded_obj = file_name[0]

    def move_slot(self):
        print('moving clicked')
        self.click_2 = ~self.click_2
        if self.click_2 == 0:
            self.path_list.clear()

    def obstacle_slot(self):
        print('creat obstacle clicked')
        self.click_1 = ~self.click_1

    def object_slot(self):
        print('create object clicked')
        self.click_3 = ~self.click_3

    def list_item_click_slot(self):
        print('List item clicked')
        curItem = self.object_listWidget.currentItem().text()
        index = self.searchOBJECT(curItem)
        self.loadFiletext.setText(self.object_list[index].obj_file.split('/')[-1])
        self.colorR.setValue(self.object_list[index].color[0])
        self.colorG.setValue(self.object_list[index].color[1])
        self.colorB.setValue(self.object_list[index].color[2])
        self.colorA.setValue(self.object_list[index].color[3])
        self.sizex.setValue(self.object_list[index].size[0])
        self.sizey.setValue(self.object_list[index].size[1])
        self.sizez.setValue(self.object_list[index].size[2])
        self.originX.setValue(self.object_list[index].origin[0])
        self.originY.setValue(self.object_list[index].origin[1])
        self.originZ.setValue(self.object_list[index].origin[2])
        self.rotX.setValue(self.object_list[index].rotX)
        self.rotY.setValue(self.object_list[index].rotY)
        self.rotZ.setValue(self.object_list[index].rotZ)

        self.scriptListWidget.clear()
        for script in self.object_list[index].script_list:
            item = QListWidgetItem(str(script))
            self.scriptListWidget.addItem(item)

        if self.object_list[index].collision_state is False:
            self.collider.setChecked(False)
        else:
            self.collider.setChecked(True)

    def delete_slot(self):
        print('List Item delete')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            self.object_list.pop(index)
            print(self.object_listWidget.currentRow())
            self.object_listWidget.takeItem(self.object_listWidget.currentRow())

    def property_slot(self):
        print('SET PROPERTY')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            self.object_list[index].set_OBJ(self.loaded_obj)
            self.object_list[index].load_OBJ()
            self.object_list[index].size = [float(self.sizex.text()), float(self.sizey.text()), float(self.sizez.text())]
            self.object_list[index].origin = [float(self.originX.text()), float(self.originY.text()),
                             float(self.originZ.text())]
            self.object_list[index].setRot([float(self.rotX.text()), float(self.rotY.text()), float(self.rotZ.text())])
            self.object_list[index].color = [float(self.colorR.text()), float(self.colorG.text()),
                            float(self.colorB.text()), float(self.colorA.text())]
            print(self.object_list[index].color)
            print('OBJECT PROPERTY SETTING COMPLETED')

    def load_script_slot(self):
        print('LOAD SCRIPT')
        file_name = QFileDialog.getOpenFileName(self,"Open script", "./", "Script Files (*.cpp)")
        self.loadScriptText.setText(file_name[0].split('/')[-1])
        self.loaded_script = file_name[0]

    def script_add_slot(self):
        print('ADD SCRIPT')
        item = QListWidgetItem(str(self.loaded_script))
        self.scriptListWidget.addItem(item)

    def script_delete_slot(self):
        print('DELETE SCRIPT')
        self.scriptListWidget.takeItem(self.scriptListWidget.currentRow())

    def script_set_slot(self):
        print('SET SCRIPT')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            temp_list = list()
            for x in range(self.scriptListWidget.count()):
                temp_list.append(self.scriptListWidget.item(x).text())
            self.object_list[index].script_list = copy.deepcopy(temp_list)

    def AABB_slot(self):
        print('SET AABB')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            self.object_list[index].set_Collision()
            self.object_list[index].collision_state = True
            self.collider.setChecked(True)
            self.object_list[index].collision_Type = 'AABB'

    def sphere_slot(self):
        print('SET Sphere')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            self.object_list[index].set_Collision()
            self.object_list[index].collision_state = True
            self.collider.setChecked(True)
            self.object_list[index].collision_Type = 'Sphere'

    def collider_slot(self):
        print('COLLIDER CHECK')
        curItem = self.object_listWidget.currentItem()
        if curItem is not None:
            curItem_text = curItem.text()
            index = self.searchOBJECT(curItem_text)
            self.object_list[index].collision_state = self.collider.checkState()

    def view2world(self, x, y):
        model = glGetDoublev(GL_MODELVIEW_MATRIX)
        proj = glGetDoublev(GL_PROJECTION_MATRIX)
        view = glGetIntegerv(GL_VIEWPORT)
        winx = x
        winy = view[3] - y
        winz = glReadPixels(winx, winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT)
        world = gluUnProject(winx, winy, winz, model, proj, view)
        return world

    def mouseChecking(self):
        if self.mouse is True:
            world = self.view2world(self.mouseX, self.mouseY)
            self.vertex_list.append(world)
            vec = world - self.camera
            vec = unit_vec(vec)
            ray = np.array(world)
            while ray[2] > 0:
                ray = ray + vec * 0.1
            self.vertex_list.append(ray)

            if self.click_1:
                if int(ray[0]) >= 0 and int(ray[0]) < self.cell.rows\
                        and int(ray[1]) >= 0 and int(ray[1]) < self.cell.cols:
                    if self.cell.checkObstacle(int(ray[0]), int(ray[1])) is False:
                        self.cell.setObstacle(True, int(ray[0]), int(ray[1]))
                        object = OBJECT()
                        object.set_OBJ('model/cube.obj')
                        object.load_OBJ()
                        object.origin = [int(ray[0]), int(ray[1]), 0]
                        object.color = [0, 0, 0, 0.6]
                        object.object_ID = int(ray[0]) * self.cell.cols + int(ray[1])
                        self.obstacle_list.append(object)
                        print('OBSTACLE CREATED')
                    else:
                        print('OBSTACLE REMOVING')
                        self.cell.setObstacle(False, int(ray[0]), int(ray[1]))
                        for i in range(len(self.obstacle_list)):
                            print('ID', self.obstacle_list[i].object_ID, 'finding ', int(ray[0]) * self.cell.cols + int(ray[1]))
                            if self.obstacle_list[i].object_ID == int(ray[0]) * self.cell.cols + int(ray[1]):
                                self.obstacle_list.pop(i)
                                break
            elif self.click_2:
                self.cell.clearCell()
                self.path_list.clear()
                curItem = self.object_listWidget.currentItem()
                if curItem is not None:
                    curItem_text = curItem.text()
                    index = self.searchOBJECT(curItem_text)
                    start = [self.object_list[index].origin[0], self.object_list[index].origin[1]]
                    goal = [int(ray[0]), int(ray[1])]
                    path = self.cell.Dijkstra(start, goal)
                    print('PATH : ', start, '->', goal)
                    for k in path:
                        object = OBJECT()
                        object.set_OBJ('model/cube.obj')
                        object.load_OBJ()
                        object.color = [0, 0, 0, 0.5]
                        object.origin = [k[0], k[1], 0]
                        object.size = [1, 1, 0]
                        self.path_list.append(object)
            elif self.click_3:
                if int(ray[0]) >= 0 and int(ray[0]) < self.cell.rows and int(ray[1]) >= 0\
                        and int(ray[1]) < self.cell.cols and self.loaded_obj is not None:
                    object = OBJECT()
                    object.set_OBJ(self.loaded_obj)
                    object.load_OBJ()
                    object.size = [float(self.sizex.text()), float(self.sizey.text()), float(self.sizez.text())]
                    object.origin = [int(ray[0]), int(ray[1]), float(self.originZ.text())]
                    object.setRot([float(self.rotX.text()), float(self.rotY.text()), float(self.rotZ.text())])
                    object.color = [float(self.colorR.text()),float(self.colorG.text()),
                                            float(self.colorB.text()), float(self.colorA.text())]
                    object.object_ID = self.loaded_obj.split('/')[-1] + '-ID-' + str(time.perf_counter())
                    self.object_list.append(object)
                    item = QListWidgetItem(str(object.object_ID))
                    self.object_listWidget.addItem(item)
                    print('OBJECT CREATED')
            self.mouse = False

    def searchOBJECT(self, ID):
        for i in range(len(self.object_list)):
            obj = self.object_list[i]
            if str(obj.object_ID) == str(ID):
                return i
        return None

    def draw_base(self):
        glBegin(GL_LINES)
        glColor3f(0.4, 0.4, 0.4)
        for i in range(0, 100):
            glVertex3f(i, 0, 0);
            glVertex3f(i, 100, 0);
        for j in range(0, 100):
            glVertex3f(0, j, 0);
            glVertex3f(100, j, 0);
        glEnd()

        glBegin(GL_LINES);
        glColor3f(1, 0, 0)
        glVertex3f(0, 0, 0);
        glVertex3f(20, 0, 0);

        glColor3f(0, 1, 0)
        glVertex3f(0, 0, 0);
        glVertex3f(0, 20, 0);

        glColor3f(0, 0, 1)
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 20);
        glEnd()

    def draw_objects(self):
        if self.Show_Objects.checkState() == 2:
            for obj in self.object_list:
                obj.vbo_draw()
                if self.Show_Collision.checkState() == 2 and len(obj.collision) > 0 and obj.collision_state == 2:
                    if obj.collision_Type == 'AABB':
                        obj.draw_AABB_box()
                    elif obj.collision_Type == 'Sphere':
                        obj.draw_Sphere()
                    if obj.collision_state is True:
                        if obj.collision_Type == 'AABB':
                            temp = obj.convert_AABB()
                            for obj2 in self.object_list:
                                if len(obj2.collision) > 0 and obj2 is not obj and obj2.collision_state is True:
                                    temp2 = obj2.convert_AABB()
                                    if obj.OBB_collision(temp, temp2):
                                        print(obj.object_ID, '-collistion with', obj2.object_ID)
                        elif obj.collision_Type == 'Sphere':
                            for obj2 in self.object_list:
                                if len(obj2.collision) > 0 and obj2 is not obj and obj2.collision_state is True:
                                    obj.S2S_collision(obj2)

        for obj in self.path_list:
            obj.vbo_draw()

        if self.Show_Obstacle.checkState() == 2:
            for obj in self.obstacle_list:
                obj.vbo_draw()

        if self.Show_Ray.checkState() == 2:
            glBegin(GL_LINES)
            glColor3f(1, 0, 0)
            for ver in self.vertex_list:
                glVertex3f(ver[0], ver[1], ver[2])
            glEnd()

def main():
    print('PY START')
    app = QApplication(sys.argv)
    myWindow = MyWindow()
    myWindow.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
