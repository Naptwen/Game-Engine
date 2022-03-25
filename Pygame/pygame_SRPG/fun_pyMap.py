import fun_pyCell
import fun_pyMap
import numpy as np
import fun_pyCharacter
import copy
import os

def army_txt_import(txt):
    file = open(txt, "r")
    temp = file.read().splitlines()
    army = []
    money = int(temp[0])
    for i in range(1, len(temp)):
        army.append(str(temp[i]))
    file.close()
    army.sort()
    return [money, army]

def army_txt_export(txt, army, money):
    file = open(txt, "w")
    file.write(str(money) + "\n")
    for temp in army:
        file.write(str(temp) + "\n")
    file.close()

def map_load_for_game(map_name):
    tile_map = []
    tile_img_map = []
    obj_map = []
    ob_img_map = []
    enemy_map = []
    etc_map = []
    [tile_map_size, tile_map, tile_img_map, obj_map,
    ob_img_map, enemy_map, etc_map] = fun_pyMap.map_txt_import(map_name)
    table = []
    table = fun_pyMap.map2cell(tile_map_size[0], tile_map_size[1], tile_map, obj_map, enemy_map, etc_map)
    return tile_img_map, ob_img_map, table

def map2cell(cols, rows, tile_map, obj_map, enemy_map, etc_map):
    table = fun_pyCell.Cell_Table(cols, rows)
    for j in range(0,table.rows):
        for i in range(0,table.cols):
            index = table.cols * j + i
            if tile_map[i][j] == 0 or obj_map[i][j] != 0:
                table.node_table[index].obstacle = True
                if obj_map[i][j] == 1:
                    table.node_table[index].block = True
            if etc_map[i][j] == 1:
                table.node_table[index].status = "deploy"
            elif etc_map[i][j] == 2:
                table.node_table[index].status = "enemy_deploy"
            temp = enemy_map[i][j]
            if temp > 0:
                table.node_table[index].type = copy.deepcopy(fun_pyCharacter.enemy[temp])
                table.node_table[index].block = True
    table2 = copy.deepcopy(table)
    del table
    return table2

def world_txt_export(txt, world_cell):
    cur_path = os.path.dirname(__file__)
    source_path = os.path.join(cur_path, txt)
    file = open(source_path, "w")
    file.write(str(world_cell.cols) + str(",") + str(world_cell.rows)+ "\n") 
    for j in range(0, world_cell.rows):
        for i in range(0, world_cell.cols):
            temp = world_cell.node_table[world_cell.cols * j + i].type
            if temp is not None:
                txt = str(i) + "," + str(j) + "," + str(temp.name) +\
                    "," + str(temp.text) + "," + str(temp.tag)
                for val in temp.buildings.values():
                    txt += "," + str(val)
                txt += "\n"
                file.write(txt) 
    file.close()
#call by object since receive class object
def world_txt_import(txt, world_cell):
    cur_path = os.path.dirname(__file__)
    source_path = os.path.join(cur_path, txt)
    try:
        file = open(source_path, "r")
        line = file.readline()
        data = line.split(",")
        world_size = [int(data[0]), int(data[1])]
        j = 0
        while True:
            line = file.readline()
            if not line:
                break
            elif line == "\n":
                pass
            else:
                sub_data = line.split(",")
                pos = [int(sub_data[0]), int(sub_data[1])]  
                cell = world_cell.giveCell(pos)
                if cell.type is not None:
                    temp = cell.type
                    temp.name = sub_data[2]
                    temp.text = sub_data[3]
                    temp.tag = sub_data[4]
                    for k in range(5, len(sub_data)):
                        temp.buildings[list(temp.buildings)[k-5]] = int(sub_data[k])  
        file.close()
    except FileNotFoundError:
        print("world map save file load error")

def map_txt_import(txt):
    cur_path = os.path.dirname(__file__)
    source_path = os.path.join(cur_path, txt)
    try:
        file = open(source_path, "r")
        line = file.readline()
        data = line.split(",")
        tile_size = [int(data[0]), int(data[1])]
        tile_map = np.zeros(tile_size)
        tile_img_map = np.zeros(tile_size)
        obj_map = np.zeros(tile_size)
        obj_img_map = np.zeros(tile_size)
        enemy_map = np.zeros(tile_size)
        etc_map = np.zeros(tile_size)
        j = 0
        while True:
            line = file.readline()
            data = line.split(",")
            if line == "":
                break
            for i, txt in enumerate(data):
                data2 = txt.split(":")
                tile_map[i][j] = int(data2[0])
                tile_img_map[i][j] = int(data2[1])
                obj_map[i][j] = int(data2[2])
                obj_img_map[i][j] = int(data2[3])
                enemy_map[i][j] = int(data2[4])
                etc_map[i][j] = int(data2[5])
            j += 1
            if not line:
                break
        file.close()
        return [tile_size, tile_map, tile_img_map, obj_map, obj_img_map, enemy_map, etc_map]
    except FileNotFoundError:
        return [ [0,0], [0],[0],[0],[0],[0],[0]]

def map_txt_export(txt, tile_map_size, tile_map, tile_img_map, obj_map, obj_img_map, enemy_map, etc_map):
    cur_path = os.path.dirname(__file__)
    source_path = os.path.join(cur_path, txt)
    file = open(source_path, "w")
    file.write(str(tile_map_size[0]) + str(",") + str(tile_map_size[1]) + str("\n"))
    for j in range(0, tile_map_size[1]):
        for i in range(0, tile_map_size[0]):
            txt = str(int(tile_map[i][j])) + ":" + str(int(tile_img_map[i][j])) + ":" + str(int(obj_map[i][j])) \
            + ":" + str(int(obj_img_map[i][j]))+ ":" + str(int(enemy_map[i][j])) + ":" + str(int(etc_map[i][j]))
            file.write(txt)
            if i is not tile_map_size[0]-1:
                file.write(",")
        if j is not tile_map_size[1]-1:
            file.write("\n")
    file.close()