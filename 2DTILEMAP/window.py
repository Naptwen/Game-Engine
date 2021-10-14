from typing import Any
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtGui import QIcon, QPixmap, QImage
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QLineF
import sys
from GUI import Ui_MainWindow
from mapeditor import *


class GMainWindow(QMainWindow):
    def __init__(self, parent = None):
        super().__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.SetUp.clicked.connect(self.viewSet)
        self.treeSet()

    def viewSet(self):
        self.scene = GScene(self.ui)
        self.scene.setSceneRect(0, 0, self.ui.map_w.value() * 64, self.ui.map_h.value() * 64)
        self.scene.init_map(self.ui.map_w.value(), self.ui.map_h.value())
        self.ui.graphicsView.setScene(self.scene)
        self.ui.graphicsView.setAlignment(Qt.AlignTop | Qt.AlignLeft)
        self.ui.pushButton_2.clicked.connect(self.export)
    
    def treeSet(self):
        map = QtWidgets.QTreeWidgetItem(self.ui.treeWidget)
        map.setText(0, "map")
        water = QtWidgets.QTreeWidgetItem(map)
        water.setText(0, "water")
        grass = QtWidgets.QTreeWidgetItem(map)
        grass.setText(0, "grass")
        obstacle = QtWidgets.QTreeWidgetItem(self.ui.treeWidget)
        obstacle.setText(0, "obstacle")
        egg = QtWidgets.QTreeWidgetItem(obstacle)
        egg.setText(0, "egg")
        sapphire = QtWidgets.QTreeWidgetItem(obstacle)
        sapphire.setText(0, "sapphire")
        character = QtWidgets.QTreeWidgetItem(self.ui.treeWidget)
        character.setText(0, "character")
        monster = QtWidgets.QTreeWidgetItem(character)
        monster.setText(0, "monster")

    def export(self):
        file = open("map_data.txt", "w")
        row, col = self.scene.map_data.shape
        map = self.scene.map_data.astype(str)
        for i in range(0, row):
            for j in range(0, col):
                file.write(map[i,j])
            file.write("\n")
        file.write("**obstacle**")
        for i in range(0, row):
            for j in range(0, col):
                file.write(map[i,j])
            file.write("\n")
        file.close()
        print("FIle EXPORTED")
        
if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    qt = GMainWindow()
    qt.show()
    sys.exit(app.exec_())
