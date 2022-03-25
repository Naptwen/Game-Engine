import pygame
import os
import fun_pyImgTxt
import fun_pyImgTxtBtn
import fun_pyMap
import fun_pytxt
from fun_pyMap import *
import numpy as np
import copy
import fun_pyCell

building    = {"LUMBER" : 0, "FARM" : 0, "MINE" : 0, "BLACK" : 0, 
                "HOUSE" : 0, "RANCH" : 0, "POPULATION" : 0}
price       = {"LUMBER" : 100, "FARM" : 100, "MINE" : 100, "BLACK" : 100, 
                "HOUSE" : 100, "RANCH" : 100}

class Building(object):
    name = None
    text = None
    tag = None
    buildings = building
    def __init__(self, name, text, tag):
        self.name = name
        self.text = text
        self.tag = tag
        self.buildings = copy.deepcopy(building)

class main_menu(object):
    screen = None
    screen_size = []
    phase = None
    build = False
    sound = 1
    mouse_pos = [0,0]
    cell_pos = [0,0]
    current_cell_pos = None
    resource = [100,100,100,100,100,100, 100] #wood, bread, stone, iron, meat, gold, skull
    unit = ["Footman", "Footman", "Footman", "Archer"]

    btn_size = [100,100]
    btn_list = []
    btn_list_txt = [] ##building names in heere
    btn_list2 = []
    btn_list_txt2 = [] ##building names in heere

    world_map_pos = [0,0]
    world_cell_map = None
    world_map_size = []
    world_map_screen_size = []
    world_cell_size = 40

    world_img_list = {}
    world_terrain_list = {}
    world_etc_list = {}

    world_img_txt_list = ["map_tile/world1.png"]
    world_terrain_txt_list = ["map_tile/mountain1.png", "map_tile/forest1.png"]
    world_etc_txt_list = ["map_tile/etc1.png"]

    world_terrain_map = []
    world_etc_img_map = []
    world_obj_map = []
    world_img_map = []
    world_etc_map = []

    ui_frame = None
    frame = None
    resource_frame = None

    def __init__(self, screen):        
        cur_path = os.path.dirname(__file__)
        source_path = os.path.join(cur_path, "images")
        self.screen = screen
        info = pygame.display.Info()
        self.screen_size = [info.current_w, info.current_h]
        self.phase = None
        self.mouse_pos = [0,0]
        self.cell_pos = [0,0]
        self.build = False

        self.btn_size = [100,100]
        self.btn_list = []
        self.btn_list_txt = []
        self.btn_list2 = []
        self.btn_list_txt2 = []

        self.world_cell_map = fun_pyCell.Cell_Table(20, 20)
        self.world_map_size = [self.world_cell_map.cols, self.world_cell_map.rows]
        self.world_map_screen_size = [self.screen_size[0], self.screen_size[1] - self.btn_size[1]]
        self.world_map_pos = [0,0]

        self.world_img_list = {}
        self.world_terrain_list = {}
        self.world_etc_list = {}
        #----------set sprite map image--------------
        for i, img in enumerate(self.world_img_txt_list):
            temp = pygame.image.load(os.path.join(source_path, img)).convert()
            for j in range(0, 16):
                sprite_img = temp.subsurface([52 * j, 0, 52, 52])
                sprite_img = pygame.transform.scale(
                    sprite_img, (self.world_cell_size, self.world_cell_size))
                self.world_img_list[16 * i + j + 1] = sprite_img
        for i, img in enumerate(self.world_terrain_txt_list):
            temp = pygame.image.load(os.path.join(source_path, img)).convert()
            for j in range(0, 16):
                sprite_img = temp.subsurface([52 * j, 0, 52, 52])
                sprite_img = pygame.transform.scale(
                    sprite_img, (self.world_cell_size, self.world_cell_size))
                self.world_terrain_list[16 * i + j + 1] = sprite_img
        for i, img in enumerate(self.world_etc_txt_list):
            temp = pygame.image.load(os.path.join(source_path, img)).convert()
            for j in range(0, 16):
                sprite_img = temp.subsurface([52 * j, 0, 52, 52])
                sprite_img = pygame.transform.scale(
                    sprite_img, (self.world_cell_size, self.world_cell_size))
                self.world_etc_list[16 * i + j + 1] = sprite_img
        #---------------------------------------------
        self.world_obj_map = np.zeros([self.world_cell_map.cols, self.world_cell_map.rows])
        self.world_terrain_map = copy.deepcopy(self.world_obj_map)
        self.world_img_map = copy.deepcopy(self.world_obj_map)
        self.world_etc_img_map = copy.deepcopy(self.world_obj_map)
        #-----------set merge map image------------------
        for obj in range(0, 2):
            indices = range(16 * obj, 16 * obj + 16)
            self.world_img_map = fun_pyCell.merge(
                self.world_obj_map, self.world_map_size, obj + 1, indices, self.world_img_map)
        #-------------------map setting------------------
        [self.world_map_size, self.world_obj_map , self.world_img_map, self.world_terrain_map,
            self.world_terrain_img_map, enemy_map, self.world_etc_map] = copy.deepcopy(fun_pyMap.map_txt_import("maps/world_map.txt"))
        self.world_cell_map = fun_pyCell.Cell_Table(self.world_map_size[0], self.world_map_size[1])
        self.map_setting()
        #--------------------fame-&--UI btn-----------------------
        self.btn_list_txt = list(Building.buildings.keys())
        self.btn_list_txt2 = []
        for key in list(Building.buildings.keys()):
            self.btn_list_txt2.append(str(key) + "_delete")
        self.btn0 = fun_pyImgTxtBtn.pyImgTxtBtn(
            [self.btn_size[0], self.world_map_screen_size[1]], "BUILD", 15, 
            [0,0,0], [190,215,200], self.btn_size, 
            "main_menu/banner.png", self.screen, "BUILD")
        self.btn1 = fun_pyImgTxtBtn.pyImgTxtBtn(
            [self.world_map_screen_size[0] - self.btn_size[0]*3 * 0.5, self.world_map_screen_size[1]], "EXIT", 15, 
            [0,0,0], [190,215,200], self.btn_size, 
            "main_menu/banner.png", self.screen, "EXIT")
        self.ui_frame = fun_pyImgTxt.pyImgTxt("",  [0,self.world_map_screen_size[1]], 
                                                    15, [0,0,0], [0,0], "main_title/Frame1.png", 
                                                    [self.world_map_screen_size[0], self.btn_size[1]], self.screen)     
        center = [self.screen_size[0]*1/6, self.screen_size[1]*1/6]
        self.mini_frame = fun_pyImgTxt.pyImgTxt("",  [0,0], 15, [0,0,0], [15,15], 
                                "main_title/Frame.png", [150,200], self.screen)    
        #---------resource-----------------------------------------
        self.resource_frame = fun_pyImgTxt.pyImgTxt("",  [0,0], 20, [0,0,0], [25,5], "main_title/Frame.png",
                             [self.btn_size[0] * 2.5, self.btn_size[1]*1.5], self.screen)    
        #-----------------------other UI frame---------------------
        self.frame1 = fun_pyImgTxt.pyImgTxt("",  center, 15, [0,0,0], [15,15], "main_title/Frame.png", 
                                                    [self.screen_size[0] * 0.7, self.screen_size[1] * 0.7], self.screen)   
        for i in range(0,6): 
            btn = fun_pyImgTxtBtn.pyImgTxtBtn(
                [center[0] + self.btn_size[0] * (i + 1), center[1] + self.btn_size[1]], 
                self.btn_list_txt[i], 12, [0,0,0], [190,215,200], 
                [self.btn_size[0], self.btn_size[1] * 0.25], 
                "main_menu/banner.png", self.screen, self.btn_list_txt[i])
            self.btn_list.append(btn)
        for i in range(0,6): 
            btn = fun_pyImgTxtBtn.pyImgTxtBtn(
                [center[0] + self.btn_size[0] * (i + 1), center[1] + self.btn_size[1]*5 * 0.25], 
                "DESTORY", 12, [0,0,0], [190,215,200], 
                [self.btn_size[0], self.btn_size[1] * 0.25], 
                "main_menu/banner.png", self.screen, self.btn_list_txt2[i])
            self.btn_list2.append(btn)

    def phase_check(self):
        pass

    def screen_move(self):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:
            self.world_map_pos[0] += 10
        elif keys[pygame.K_d]:
            self.world_map_pos[0] -= 10
        elif keys[pygame.K_w]:
            self.world_map_pos[1] += 10
        elif keys[pygame.K_s]:
            self.world_map_pos[1] -= 10
        mouse_pos = pygame.mouse.get_pos()
        speed = 40
        if mouse_pos[0] == self.screen_size[0] - 1:
            self.world_map_pos[0] -= speed
            if mouse_pos[1] == self.screen_size[1] - 1:
                self.world_map_pos[1] -= speed
            elif mouse_pos[1] == 0:
                self.world_map_pos[1] += speed
        elif mouse_pos[0] == 0:
            self.world_map_pos[0] += speed
            if mouse_pos[1] == self.screen_size[1] - 1:
                self.world_map_pos[1] -= speed
            elif mouse_pos[1] == 0:
                self.world_map_pos[1] += speed
        elif mouse_pos[1] == self.screen_size[1] - 1:
            self.world_map_pos[1] -= speed
            if mouse_pos[0] == self.screen_size[0] - 1:
                self.world_map_pos[0] -= speed
            elif mouse_pos[0] == 0:
                self.world_map_pos[0] += speed
        elif mouse_pos[1] == 0:
            self.world_map_pos[1] += speed
            if mouse_pos[0] == self.screen_size[0] - 1:
                self.world_map_pos[0] -= speed
            elif mouse_pos[0] == 0:
                self.world_map_pos[0] += speed
        w = self.world_map_size[0] * self.world_cell_size
        h = self.world_map_size[1] * self.world_cell_size
        if w >= self.world_map_screen_size[0]:
            if  self.world_map_pos[0] <= -w + self.world_map_screen_size[0] :
                self.world_map_pos[0] = -w + self.world_map_screen_size[0]
            elif self.world_map_pos[0] >= 0:
                self.world_map_pos[0] = 0
        else:
            if self.world_map_pos[0] >= self.world_map_screen_size[0] - w :
                self.world_map_pos[0] = self.world_map_screen_size[0] - w
            elif self.world_map_pos[0] <= 0:
                self.world_map_pos[0] = 0
        if h >= self.world_map_screen_size[1]:
            if  self.world_map_pos[1] <= -h + self.world_map_screen_size[1] :
                self.world_map_pos[1] = -h + self.world_map_screen_size[1]
            elif self.world_map_pos[1] >= 0:
                self.world_map_pos[1] = 0
        else:
            if self.world_map_pos[1] >= self.world_map_screen_size[1] - h :
                self.world_map_pos[1] = self.world_map_screen_size[1] - h
            elif self.world_map_pos[1] <= 0:
                self.world_map_pos[1] = 0

    def map_setting(self):
        for j in range(0, self.world_map_size[1]):
            for i in range(0, self.world_map_size[0]):    
                paint = self.world_img_map[i][j]
                if paint == 0:
                    self.world_cell_map.node_table[self.world_map_size[0] * j + i].obstacle = True
                temp = Building("Nothing","None", "None")
                temp.name = "<CASTLE>"
                temp.text = "[1 Turn per]"
                paint = self.world_terrain_img_map[i][j] 
                if paint >= 33:   
                    self.world_cell_map.node_table[self.world_map_size[0] * j + i].type = copy.deepcopy(temp)
                paint = self.world_etc_map[i][j]
                if paint == 1:
                    temp.tag = "alliance"
                    self.world_cell_map.node_table[self.world_map_size[0]  * j + i].type = copy.deepcopy(temp)
                if paint == 2:
                    temp.tag = "enemy"
                    self.world_cell_map.node_table[self.world_map_size[0] * j + i].type = copy.deepcopy(temp)
        # fun_pyMap.world_txt_import("world_save.txt", self.world_cell_map)
    def draw(self): 
        for j in range(0, self.world_map_size[1]):
            for i in range(0, self.world_map_size[0]):
                #-----------tile draw-----------------------------
                paint = self.world_img_map[i][j]
                if paint > 0:
                    self.screen.blit(self.world_img_list[paint], 
                    [self.world_map_pos[0] + i * self.world_cell_size,
                     self.world_map_pos[1] + j * self.world_cell_size])
                paint = self.world_terrain_img_map[i][j]
                if 33> paint > 0:
                    self.screen.blit(self.world_terrain_list[paint], 
                    [self.world_map_pos[0] + i * self.world_cell_size,
                     self.world_map_pos[1] + j * self.world_cell_size])  
                #------------building draw-----------------------
                if self.world_cell_map.node_table[self.world_map_size[0] * j + i].status is not None:
                    same = self.world_cell_map.node_table[self.world_map_size[0] * j + i].status
                    for k, key in enumerate(list(building.keys())):
                        if key == same:
                            break
                    temp = self.world_etc_list[k + 1]
                    temp = pygame.transform.scale(temp, [self.world_cell_size, self.world_cell_size])
                    self.screen.blit(temp,
                        [self.world_map_pos[0] + i * self.world_cell_size,
                        self.world_map_pos[1] + j * self.world_cell_size])                   
                #------------team draw---------------------------
                cell = self.world_cell_map.giveCell([i,j])
                if cell.type is not None: 
                    self.screen.blit(self.world_etc_list[16], 
                    [self.world_map_pos[0] + i * self.world_cell_size,
                     self.world_map_pos[1] + j * self.world_cell_size])   
                    s = pygame.Surface([self.world_cell_size, self.world_cell_size])
                    s.set_alpha(100)
                    if cell.type.tag == "alliance":
                        s.fill([0, 0, 255])
                    elif cell.type.tag == "enemy":\
                        s.fill([255, 0, 0])
                    self.screen.blit(
                        s, [self.world_map_pos[0] + i * self.world_cell_size,
                            self.world_map_pos[1] + j * self.world_cell_size]) 
        #------------------cursor draw------------------
        x = self.cell_pos[0] * self.world_cell_size
        y = self.cell_pos[1] * self.world_cell_size
        pygame.draw.rect(self.screen, [200,216,0], 
            [self.world_map_pos[0] + x,
            self.world_map_pos[1] + y,
            self.world_cell_size, self.world_cell_size], 5)  
        #-----------------current cell select draw------
        if self.current_cell_pos is not None:
            b = self.world_map_pos[0] + self.current_cell_pos[0] * self.world_cell_size 
            c = self.world_map_pos[1] + self.current_cell_pos[1] * self.world_cell_size 
            pygame.draw.rect(self.screen, [255,0,76], [b, c,
              self.world_cell_size,   self.world_cell_size], 5)   
        #-----------------mini info draw-----------------
        cell = self.world_cell_map.giveCell(self.cell_pos)
        if cell is not None and cell.type is not None:  
            if self.world_map_pos[0] + x + self.world_cell_size + self.mini_frame.w\
                 <= self.world_map_screen_size[0]:
                self.mini_frame.x = self.world_map_pos[0] + x + self.world_cell_size
            else:
                self.mini_frame.x = self.world_map_pos[0] + x - self.mini_frame.w
            self.mini_frame.y = self.world_map_pos[1] + y
            self.mini_frame.text = cell.type.name + "\n" + cell.type.text
            for key, val in list(cell.type.buildings.items()):
                self.mini_frame.text += "\n" + str(key) + " : " + str(val)
            self.mini_frame.draw()     
        #----------UI---------------
        self.ui_frame.draw()
        self.btn1.draw(self.mouse_pos)
        self.castle_info()
        #----------resource UI draw------------------------
        self.resource_frame.draw() 
        for i in range(0, 7):   
            total = 0
            for cell in self.world_cell_map.node_table:
                if cell.type is not None and cell.type.tag == "alliance":
                    total  += list(cell.type.buildings.values())[i]
            x = i // 4
            y = i % 4
            temp = fun_pytxt.pytxt( str(self.resource[i]) + "+" + str(total),
                    [self.btn_size[0] * 1.2 * x + self.btn_size[0] * 0.5, self.btn_size[1] * 0.25 * y + self.btn_size[1] * 0.2],
                    15, [0,0,0], self.screen, None)
            temp.draw(self.mouse_pos)
            #---------resource icon draw-------------     
            temp = self.world_etc_list[i + 9]
            temp = pygame.transform.scale(temp, [self.btn_size[0] * 0.5, self.btn_size[1] * 0.5])
            self.screen.blit(temp, [self.btn_size[0]  * 1.2 * x + self.btn_size[0] * 0.02, self.btn_size[1] * 0.25 * y]) 
            
    def castle_info(self):
       #-----------castle info draw--------------
        if self.current_cell_pos is not None:
            self.btn0.draw(self.mouse_pos)   
            if self.build == True:
                self.frame1.draw()
                for i, btn in enumerate(self.btn_list):
                    #--------------building icon draw---------
                    temp = self.world_etc_list[i + 1]
                    temp = pygame.transform.scale(temp, [btn.w, btn.h * 2])
                    self.screen.blit(temp,[btn.x, btn.y - btn.h * 2])
                    #--------------gold icon draw-------------
                    if i < 3:
                        temp = self.world_etc_list[13]
                    if i == 3:
                        temp = self.world_etc_list[11]
                    if i == 4:
                        temp = self.world_etc_list[9]
                    if i == 5:
                        temp = self.world_etc_list[10]
                    self.screen.blit(temp,[btn.x, btn.y + btn.h*3 * 0.5])
                    #---------------cost text draw------------
                    temp_price = price[self.btn_list_txt[i]] #self.btn_list_txt[i] is th name of buildings
                    temp = fun_pytxt.pytxt(str(temp_price), 
                            [btn.x + btn.w * 0.5, btn.y + btn.h*3 * 0.5 + 10],15, 
                            [0,0,0], self.screen, "gold")
                    temp.draw(self.mouse_pos)
                    #-------------button draw------------------
                    btn.draw(self.mouse_pos)     
                    self.btn_list2[i].draw(self.mouse_pos)
                #---------------mini building draw-------------
                cell = self.world_cell_map.giveCell(self.current_cell_pos)
                num = 0
                for i, v in enumerate(cell.type.buildings.values()):
                    for k in range(0, v):
                        x = num%8
                        y = num//8
                        temp = self.world_etc_list[i + 1]
                        temp = pygame.transform.scale(temp, [btn.w * 0.5, btn.h])
                        self.screen.blit(temp,
                            [self.screen_size[0] * 0.25 + self.btn_size[0] * 0.5 * x, 
                            self.screen_size[1] * 0.25 + self.btn_size[1] * (4 + y)])
                        num += 1
                    #---------resource number-------------
                    x = i // 4
                    y = i % 4
                    temp = fun_pytxt.pytxt( "+" + str(v) + "/per",
                            [self.screen_size[0] * 0.25 + self.btn_size[0]* x * 2 + self.btn_size[0] * 0.5, 
                             self.screen_size[1] * 0.25 + self.btn_size[1] * 0.5 * (y + 11 * 0.25) + self.btn_size[1] * 0.25],
                             22, [0,0,0], self.screen, None)
                    temp.draw(self.mouse_pos)
                    #---------resource icon draw-------------     
                    temp = self.world_etc_list[i + 9]
                    temp = pygame.transform.scale(temp, [self.btn_size[0] * 0.5, self.btn_size[1] * 0.5])
                    self.screen.blit(temp,
                            [self.screen_size[0] * 0.25 + self.btn_size[0] * x * 2, 
                             self.screen_size[1] * 0.25 + self.btn_size[1] * 0.5 * (y + 3)]) 
   
    def castle_building_build(self):
        if self.phase in self.btn_list_txt and self.current_cell_pos:
            cell = self.world_cell_map.giveCell(self.current_cell_pos) 
            if cell is not None and cell.type is not None:
                eight_list = \
                self.world_cell_map.eight_dir_check(cell.pos, None, True, True)  
                total = 0
                for temp in price:
                    total += cell.type.buildings[temp]
                if total < len(eight_list):
                    num = 5
                    for  j, ttt in enumerate(list(price.keys())):
                        if ttt == self.phase:
                            break
                    if j < 3:
                        num = 4
                    if j == 3:
                        num = 2
                    if j == 4:
                        num = 0
                    if j == 5:
                        num = 1
                    if self.resource[num] >= price[self.phase]:
                        self.resource[num] -= price[self.phase]
                        cell.type.buildings[self.phase] += 1
            self.phase = None
        if self.phase in self.btn_list_txt2 and self.current_cell_pos:
            cell = self.world_cell_map.giveCell(self.current_cell_pos)
            if cell is not None and cell.type is not None:
                sub_temp = self.phase.split("_")[0]
                if cell.type.buildings[sub_temp] > 0:
                    cell.type.buildings[sub_temp] -= 1
                    total = 0
                    for temp in price:
                        total += cell.type.buildings[temp]   
            self.phase = None
    
    def castle_building_set(self):
        for cell in self.world_cell_map.node_table:
            if cell.type is not None:
                building_list = []
                for building in price: #since pric doesn't include population
                    for k in range(0, cell.type.buildings[building]):
                        building_list.append(building)
                if building_list:
                    eight_list = \
                        self.world_cell_map.eight_dir_check(cell.pos, None, True, True)
                    if eight_list:
                        k = 0
                        for a_building in building_list:
                            tile = self.world_cell_map.giveCell(eight_list[k])
                            tile.status = a_building
                            k += 1
                        cell.type.buildings[list(cell.type.buildings.keys())[-1]] = len(eight_list)
                    else:
                        cell.type.buildings[list(cell.type.buildings.keys())[-1]] = 8               

    def menu_event(self):
        while True:
            self.screen.fill([42,71,131])
            self.screen_move()
            for event in pygame.event.get():
                self.mouse_pos = pygame.mouse.get_pos()
                if self.world_cell_map.cols * self.world_cell_size > self.mouse_pos[0] >= 0 and\
                    self.world_cell_map.rows * self.world_cell_size > self.mouse_pos[1] >= 0:
                    self.cell_pos = [(self.mouse_pos[0] - self.world_map_pos[0])//self.world_cell_size,
                                    (self.mouse_pos[1] - self.world_map_pos[1])//self.world_cell_size]
                if event.type == pygame.QUIT:
                    return "quit"                
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    cell = self.world_cell_map.giveCell([self.cell_pos[0], self.cell_pos[1]])
                    if cell is not None and cell.type is not None and cell.type.tag == "alliance":
                        self.current_cell_pos = [self.cell_pos[0], self.cell_pos[1]] #current cell
                    if self.build == False and cell.type is not None and cell.type.tag == "enemy":
                        return "game"
                if self.build:
                    for btn in self.btn_list:
                        temp = btn.check(event, self.mouse_pos) 
                        if temp is not None:
                            self.phase = temp
                    for btn in self.btn_list2:
                        temp = btn.check(event, self.mouse_pos) 
                        if temp is not None:
                            self.phase = temp
                self.castle_building_build()
                self.castle_building_set()
                if self.btn1.check(event, self.mouse_pos) is not None:
                    fun_pyMap.world_txt_export("world_save.txt", self.world_cell_map)
                    return "main_title"
                if self.btn0.check(event, self.mouse_pos) is not None and self.current_cell_pos is not None:
                    self.build = not self.build 
                self.phase_check()
            self.draw()
            pygame.event.clear()
            pygame.time.Clock().tick(60)
            pygame.display.flip()
