from sys import prefix
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt
import random
import numpy as np

class GScene(QtWidgets.QGraphicsScene):
    map_data = np.empty([0,0], dtype = int)
    obstacle_data = np.empty([0,0], dtype = int)

    def __init__(self, window, parent = None):
        super().__init__(parent)
        self.window = window
        self.background_item = QtWidgets.QGraphicsPixmapItem()
        self.addItem(self.background_item )

    def init_map(self, w, h):
        for i in range(0,w):
            for j in range(0,h):
                pix = self.water()
                self.drawing(pix, i*64, j*64)
        self.map_data = np.zeros((h, w), dtype = int)      
        self.obstacle_data = np.zeros((h,w), dtype = int)
        print("Scene setup finish")

    def water(self):
        v = random.randint(0,2)
        pix = QtGui.QPixmap('picture/water1.jpeg')
        if v == 1:
            pix = QtGui.QPixmap('picture/water2.jpeg')
        elif v == 2:
            pix = QtGui.QPixmap('picture/water3.jpeg')
        return pix

    def grass(self, L, R, U ,D):
        pix = QtGui.QPixmap('picture/grass13.png')
        if not L and not  R and not U and D:
            pix = QtGui.QPixmap('picture/grass1.png') 
        elif not L and not R and U and D:
            pix = QtGui.QPixmap('picture/grass2.png') 
        elif not L and not R and U and not D:
            pix = QtGui.QPixmap('picture/grass3.png') 
        elif L and R and not U and D:
            pix = QtGui.QPixmap('picture/grass4.png')  
        elif L and R and U and not D:
            pix = QtGui.QPixmap('picture/grass5.png')
        elif not L and R and U and D:
            pix = QtGui.QPixmap('picture/grass6.png')
        elif L and not R and U and D:
            pix = QtGui.QPixmap('picture/grass7.png')
        elif not L and R and not U and D:
            pix = QtGui.QPixmap('picture/grass8.png')                            
        elif L and not R and not U and D:
            pix = QtGui.QPixmap('picture/grass9.png')
        elif not L and R and U and not D:
            pix = QtGui.QPixmap('picture/grass10.png')
        elif L and not R and U and not D:
            pix = QtGui.QPixmap('picture/grass11.png')
        elif not L and not R and not U and not D:
            pix = QtGui.QPixmap('picture/grass12.png')
        elif L and R and U and D:
            pix = QtGui.QPixmap('picture/grass13.png')
        elif not L and R and not U and not D:
            pix = QtGui.QPixmap('picture/grass14.png')
        elif L and R and not U and not D:
            pix = QtGui.QPixmap('picture/grass15.png')
        elif L and not R and not U and not D:
            pix = QtGui.QPixmap('picture/grass16.png')
        return pix        

    def egg(self):
        v = random.randint(0,2)
        pix = QtGui.QPixmap('picture/egg1.png')
        if v == 1:
            pix = QtGui.QPixmap('picture/egg2.png')
        elif v == 2:
            pix = QtGui.QPixmap('picture/egg3.png')
        return pix       

    def sapphire(self):
        pix = QtGui.QPixmap('picture/sapphire.png')
        return pix

    def monster(self):
        pix = QtGui.QPixmap('picture/monster.png')
        return pix

    def mapRounding(self):
        rows, cols = self.map_data.shape 
        self.removeItem(self.background_item)
        self.addItem(self.background_item)
        for j in range(0, rows):
            for i in range(0, cols):
                pix = self.water()
                self.drawing(pix,i * 64,j * 64)
                if self.map_data[j,i] == 1:
                    U,D,L,R = 0, 0, 0, 0
                    if i > 0 and i < cols-1:
                        if self.map_data[j, i - 1] == 1:
                            L = 1
                        if self.map_data[j, i + 1] == 1:
                            R = 1
                    elif i == 0 :
                        R = 1
                    elif i == cols -1:
                        L = 1
                    if j > 0 and j < rows-1:
                        if self.map_data[j - 1, i] == 1:
                            U = 1    
                        if self.map_data[j + 1, i] == 1:
                            D = 1       
                    elif j == 0 :
                        D = 1
                    elif j == rows - 1:
                        U = 1
                    pix = self.grass(L, R, U, D)
                    self.drawing(pix,i * 64,j * 64)
                if self.obstacle_data[j,i] == 1:
                    pix = self.egg()
                    self.drawing(pix,i * 64,j * 64)
                elif self.obstacle_data[j,i] == 2:
                    pix = self.sapphire()
                    self.drawing(pix,i * 64,j * 64)
                elif self.obstacle_data[j,i] == 3:
                    pix = self.monster()
                    self.drawing(pix,i * 64,j * 64)


    def drawing(self, pix, x, y):
        pixmap_item = QtWidgets.QGraphicsPixmapItem(pix)
        pixmap_item.setPos(x,y)
        pixmap_item.setParentItem(self.background_item)


    def mousePressEvent(self, event):
        item = self.window.treeWidget.currentItem()
        if item is not None and event.button() == Qt.LeftButton:
                x = event.scenePos().x() - event.scenePos().x()%64
                y = event.scenePos().y() - event.scenePos().y()%64
                if x < self.width() and y < self.height():
                    if item.text(0) == 'water':
                        self.map_data[int(y/64), int(x/64)]  = 0
                    elif item.text(0) == 'grass':
                        self.map_data[int(y/64), int(x/64)]  = 1
                    elif item.text(0) == 'egg':
                        self.obstacle_data[int(y/64), int(x/64)]  = 1
                    elif item.text(0) == 'sapphire':
                        self.obstacle_data[int(y/64), int(x/64)]  = 2
                    elif item.text(0) == 'monster':
                        self.obstacle_data[int(y/64), int(x/64)]  = 3
                    self.mapRounding()   
    
    def export(self):
        file = open("map_data.txt", "w+")
        row, col = self.map_data.shape
        for i in range(0, row):
            for j in range(0, col):
                file.write(self.map_data[i,j])
            file.write("\n")
        file.write("**obstacle**")
        for i in range(0, row):
            for j in range(0, col):
                file.write(self.obstacle_data[i,j])
            file.write("\n")
              