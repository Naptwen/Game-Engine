\
import os

import pygame
import copy
import numpy as np

import fun_pyCell
import fun_pyBtn
import fun_pyMap
import fun_pytxt
import fun_pyImgTxt
import fun_pyImgBtn
import fun_Ai


import os
# return index


def find_list(ele, list):
    for i, sub in enumerate(list):
        if ele == sub:
            return i
    return -1


# ___________________________________________________
cur_path = os.path.dirname(__file__)
source_path = os.path.join(cur_path, "images")


class game_mother_board(object):
    # ----------------Setting------------
    deploy = True
    cols, rows = [0, 0]
    table = None
    table_window = [0, 0]
    table_screen_pos = [0, 0]
    cell_size = 50
    screen = None
    screen_w = 0
    screen_h = 0
    event = None
    mouse_pos = [0, 0]

    path = []
    phase = ""
    selection = -1
    cell_pos = []
    click_cell_pos = []
    cur_cell_pos = []
    Ai_who = []
    Ai_target = []

    army_list = []
    money = 0

# ---------------IMAGE---------------
    mission_fail = None
    mission_success = None
    turn_image = None
    ttxt = None  # information for statement of character

    btn_size = [1000, 200]
    btn_list = []  # Turn Button
    btn2_list = []  # Deployment Button

    damage_txt = None
    skull_image = None
    turn_image = None

    tile_map = []
    tile_img_map = []
    obj_img_map = []

    unit_txt_list = ["Footman.png", "Archer.png",
                     "Peasant.png", "orc1.png", "ogr1.png", "troll1.png"]
    unit_img_list = {}
    tile_txt_list = ["map_tile/grass1.png", "map_tile/dirt1.png", "map_tile/ground1.png", "map_tile/ground2.png"]
    tile_img_list = {}
    obj_txt_list = ["map_tile/tree1.png", "map_tile/water1.png", "map_tile/wall1.png"]
    obj_img_list = {}

    origin_unit_img = {}
    origin_tile_img = {}
    origin_obj_img = {}

    def __init__(self, table_window, map_name, screen):
        self.screen = screen

        info = pygame.display.Info()
        self.screen_w = info.current_w
        self.screen_h = info.current_h

        self.deploy = True

        self.table_window = table_window
        self.table_screen_pos = [0, 0]
        self.cols, self.rows = [0, 0]
        self.table = fun_pyCell.Cell_Table(self.cols, self.rows)
        self.btn_size = [self.screen_w - self.table_window[0], 50]

        self.path = []
        self.phase = ""
        self.selection = -1
        self.click_cell_pos = []
        self.cur_cell_pos = []
        self.Ai_who = []
        self.Ai_target = []
        w, h = pygame.display.get_surface().get_size()
        self.mission_fail = fun_pyImgTxt.pyImgTxt(
            "MISSION FAILED", [w* 0.5, h* 0.5], 100, [255, 255, 255], None, "mission_fail.png", [w, h], self.screen)
        self.mission_success = fun_pyImgTxt.pyImgTxt(
            "MISSION COMPLETE", [w* 0.5, h* 0.5], 100, [255, 255, 255], None, "mission_suc.jpg", [w, h], self.screen)
        btn1 = fun_pyBtn.Button("MOVING",   [self.btn_size[0], 0], [self.btn_size[1], 50], 15, [
                                0, 0, 0],   [100, 100, 100], [236, 239, 241], screen, "MOVE")
        btn2 = fun_pyBtn.Button("ATTACK",   [self.btn_size[0], 50], [self.btn_size[1], 50], 15, [
                                0, 0, 0],   [100, 100, 100], [236, 239, 241], screen, "ATTACK")
        btn3 = fun_pyBtn.Button("AUTO",     [self.btn_size[0], 100], [self.btn_size[1], 50], 15, [
                                0, 0, 0],   [100, 100, 100], [236, 239, 241], screen, "AUTO")
        btn4 = fun_pyBtn.Button("TURN END", [self.btn_size[0], 150], [self.btn_size[1], 50], 15, [
                                0, 0, 0],   [100, 100, 100], [236, 239, 241], screen, "TURN END")
        btn7 = fun_pyBtn.Button("FINISH",   [self.btn_size[0], 0], [self.btn_size[1], 50], 15, [
                                0, 0, 0],   [100, 100, 100], [236, 239, 241], screen, "FINISH")
        self.turn_image = fun_pyImgTxt.pyImgTxt("", [self.screen_w* 0.5, self.screen_h* 0.5], 35,
                                                [255, 255, 255], None, "banner.png", [500, 200], self.screen)
        self.image_loader()
        self.btn_list.append(btn1)
        self.btn_list.append(btn2)
        self.btn_list.append(btn3)
        self.btn_list.append(btn4)
        self.btn2_list.append(btn7)
        self.map_load(map_name)
        self.enemy_deploy(map_name)

    def enemy_deploy(self, map_name):
        for cell in self.table:
            if cell.status == "enemy_deploy" and cell.block == False and cell.obstacle == False:
                cell.type = None

    def map_load(self, map_name):
        self.tile_img_map, self.obj_img_map, self.table = fun_pyMap.map_load_for_game(map_name)
        self.money, self.army_list = fun_pyMap.army_txt_import("save.txt")
        print("[GAME] : LOAD MAP " + str(map_name))
        print("[GAME] : MAP INFO" + str([self.table.cols, self.table.rows]))
        print("[GAME] : LOAD ARMY LIST " + str(self.army_list))

    def image_loader(self):
        cur_path = os.path.dirname(__file__)
        source_path = os.path.join(cur_path, "images")
        self.unit_img_list = {}
        for i, img in enumerate(self.unit_txt_list):
            temp = pygame.image.load(os.path.join(
                source_path,  "character/" + img)).convert()
            temp = pygame.transform.scale(
                temp, (self.cell_size, self.cell_size))
            txt = self.unit_txt_list[i].split(".")
            self.unit_img_list[txt[0]] = temp
        self.tile_img_list = {}
        for i, img in enumerate(self.tile_txt_list):
            temp = pygame.image.load(os.path.join(source_path, img)).convert()
            for j in range(0, 16):
                sprite_img = temp.subsurface([52 * j, 0, 52, 52])
                sprite_img = pygame.transform.scale(
                    sprite_img, (self.cell_size, self.cell_size))
                self.tile_img_list[16 * i + j] = sprite_img
        self.obj_img_list = {}
        for i, img in enumerate(self.obj_txt_list):
            temp = pygame.image.load(os.path.join(source_path, img)).convert()
            for j in range(0, 16):
                sprite_img = temp.subsurface([52 * j, 0, 52, 52])
                sprite_img = pygame.transform.scale(
                    sprite_img, (self.cell_size, self.cell_size))
                self.obj_img_list[16 * i + j] = sprite_img
        self.origin_unit_img = self.unit_img_list.copy()
        self.origin_tile_img = self.tile_img_list.copy()
        self.origin_obj_img = self.obj_img_list.copy()

        self.damage_txt = fun_pyImgTxt.pyImgTxt(
            "", [0, 0], 20, [0, 0, 0], None, "wow.png", [100, 100], self.screen)
        self.ttxt = fun_pytxt.pytxt(
            "", [self.table_window[0], 200], 15, [255, 255, 255], self.screen, "info")
        print("[GAME] : Deployment Phase")
        self.phase = "deploy"
        self.turn_image.text = "DEPLOYMENT TURN"

    def phase_check(self):
        if self.phase == "MOVE":
            pass
        elif self.phase == "ATTACK":
            self.player_attack_event()
        elif self.phase == "AUTO":
            self.Ai_event("troop", "enemy")
            self.phase = "TURN END"
        elif self.phase == "TURN END":
            self.trun_end_event()
            self.phase = "ENEMY"
        elif self.phase == "ENEMY":
            self.Ai_event("enemy", "troop")
            for cell in self.table.node_table:
                if cell.type is not None and cell.type.tag == "enemy":
                    cell.type.phase_end()
        elif self.phase == "FINISH":
            self.deploy = False
        elif self.phase == "LOSE":
            self.money += 100
            for cell in self.table.node_table:
                if cell.type is not None and cell.type.tag == "troop":
                    self.army_list.append(cell.type.name)
            fun_pyMap.army_txt_export("save.txt", self.army_list, self.money)
            self.phase = "main_menu"
        elif self.phase == "WIN":
            self.money += 200
            for cell in self.table.node_table:
                if cell.type is not None and cell.type.tag == "troop":
                    self.army_list.append(cell.type.name)
            fun_pyMap.army_txt_export("save.txt", self.army_list, self.money)
            self.phase = "main_menu"

    def deploy_event(self, click_cell_pos):
        if self.table.cols > click_cell_pos[0] >= 0 and self.table.rows > click_cell_pos[1] >= 0:
            index = self.table.cols * click_cell_pos[1] + click_cell_pos[0]
            if self.table.node_table[index].obstacle == False\
                    and self.table.node_table[index].block == False\
                    and self.table.node_table[index].status == "deploy"\
                    and self.selection != -1:
                self.table.node_table[index].block = True
                name = self.army_list[self.selection]
                temp = fun_pyMap.troop[name]
                self.table.node_table[index].type = copy.deepcopy(temp)
                self.army_list.pop(self.selection)
                if self.selection > len(self.army_list) - 1:
                    self.selection -= 1
                print("[GAME] : deployed" + str(self.table.node_table[index].pos) +
                      " Unit : " + self.table.node_table[index].type.name + " selection : " + str(self.selection))

    def Ai_event(self, Ai_from, Ai_to):
        self.cur_cell_pos = []
        self.click_cell_pos = []
        print("-----------------------------")
        print("[GAME] : AI Working team [" + Ai_from + "]--to[" + Ai_to + "]")
        for cell in self.table.node_table:
            if cell.type is not None and cell.type.tag == Ai_from:
                self.Ai_who = cell.pos
                order_list = fun_Ai.Ai_order(cell.pos, Ai_to, self.table, "FOX")
                print("-----------------------------")
                print("[GAME] : " + str(self.Ai_who))
                self.table_screen_pos[0] = -cell.pos[0] * self.cell_size + self.screen_w* 0.5
                self.table_screen_pos[1] = -cell.pos[1] * self.cell_size + self.screen_h* 0.5
                self.draw()
                for order in order_list:
                    print("[GAME] : order <" +
                          str(order[0]) + ">" + str(order[1]))
                    #---animation part-----
                    start_pos = cell.pos
                    end_pos = order[1]
                    for i in range(0,15):
                        a = float(start_pos[0] - end_pos[0]) * self.cell_size/15 * i
                        b = float(start_pos[1] - end_pos[1]) * self.cell_size/15 * i
                        s = pygame.Surface([self.cell_size, self.cell_size], pygame.SRCALPHA)
                        s.fill([214, 225, 64, 25])
                        self.screen.blit(s, (
                            self.table_screen_pos[0] + start_pos[0] * self.cell_size - a,
                            self.table_screen_pos[1] + start_pos[1] * self.cell_size - b))
                        pygame.display.flip()
                    #----------------
                    if order[0] == "target":
                        self.Ai_target = order[1]
                    elif order[0] == "mov":
                        self.table.swap_cell(cell.pos, order[1])
                        cell = self.table.giveCell(order[1])
                        cell.type.mov -= 1
                        self.Ai_who = cell.pos
                        self.draw()
                    elif order[0] == "atk":
                        self.Ai_target = order[1]
                        self.hit_event(order[1], self.Ai_who)
                        self.draw()
        print("-----------------------------")
        self.Ai_who = []
        self.Ai_target = []
        self.phase = ""

    def hit_event(self, target_pos, attacker_pos):
        attacker = self.table.giveCell(attacker_pos)
        target = self.table.giveCell(target_pos)
        if target is not None and attacker.type.hit > 0:
            print("[GAME] : ATTACK")
            attacker.type.hit -= 1
            result = attacker.type.attack(target.type)
            txt = "MISS"
            txt_pos = [self.table_window[0] + target.pos[0] * self.cell_size,
                       self.table_window[1] + target.pos[1] * self.cell_size]
            if result[0] == 2:
                txt = "CRI" + "-" + str(result[1])
            elif result[0] == 1:
                txt = "-" + str(result[1])
            self.damage_txt.text = txt
            self.damage_txt.x, self.damage_txt.y = txt_pos
            attacker.type.mov = 0
            self.draw()

    def player_mouse_event(self, event, cell_pos):
        cell = None
        if self.cur_cell_pos and self.phase == "MOVE":
            start = self.table.giveCell(self.cur_cell_pos)
            end = self.table.giveCell(cell_pos)
            if start is not None and end is not None:
                self.path = self.table.Astar(self.cur_cell_pos, cell_pos, True)
        if event.type == pygame.MOUSEBUTTONDOWN:     
            if event.button == 1:
                print("[GAME] : mouse LEFT click")
                cell = self.table.giveCell(cell_pos)
                if cell is not None:
                    self.click_cell_pos =  cell.pos
                    print("[GAME] : Cell Pos" + str(self.click_cell_pos))
                    if self.deploy == True:
                        if  self.click_cell_pos:
                            self.deploy_event(self.click_cell_pos)
                    else:
                        if self.phase == "MOVE" and cell.type is not None and cell.type.tag == "troop":
                            self.cur_cell_pos =  self.click_cell_pos 
                            self.path = []
                        elif self.cur_cell_pos and self.phase == "MOVE" and\
                            self.path and self.click_cell_pos and\
                            self.table.giveCell(self.cur_cell_pos).type is not None and\
                            self.table.giveCell(self.click_cell_pos).obstacle == False and\
                            self.table.giveCell(self.click_cell_pos).block == False:
                            num = find_list(self.click_cell_pos, self.path)
                            step = copy.deepcopy(self.table.giveCell(self.cur_cell_pos).type.mov)
                            if step > num >= 0:
                                for i in range(0, num + 1):
                                    self.table.swap_cell(self.cur_cell_pos, self.path[i])
                                    self.cur_cell_pos = self.path[i]
                                    self.draw()
                                    step -= 1
                                    pygame.time.wait(50)
                                character = self.table.giveCell(self.cur_cell_pos)
                                character.type.mov = step
                                self.click_cell_pos = []
                                self.path = []
                                print("[GAME] : MOVE action done")       
                        elif self.phase == "ATTACK" and cell.type is not None and cell.type.tag == "troop" :
                            self.cur_cell_pos =  self.click_cell_pos 
                            self.path = []
                        elif self.phase == "ATTACK" and find_list(self.click_cell_pos, self.path) < 0:
                            print("[GAME] : ATTACK cancle")
                            self.cur_cell_pos = []
                            self.path = []
                           
            elif event.button == 4:
                self.cell_size -= 2
                if self.cell_size < 20:
                    self.cell_size = 20
                for k, img in self.origin_unit_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.unit_img_list[k] = temp_img
                for k, img in self.origin_tile_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.tile_img_list[k] = temp_img
                for k, img in self.origin_obj_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.obj_img_list[k] = temp_img
            elif event.button == 5:
                self.cell_size += 20
                if self.cell_size > 100:
                    self.cell_size = 100
                for k, img in self.origin_unit_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.unit_img_list[k] = temp_img
                for k, img in self.origin_tile_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.tile_img_list[k] = temp_img
                for k, img in self.origin_obj_img.items():
                    temp_img = pygame.transform.scale(
                        img, (self.cell_size, self.cell_size))
                    self.obj_img_list[k] = temp_img
  
    def player_attack_event(self):
        if self.cur_cell_pos:
            temp = self.table.giveCell(self.cur_cell_pos)
            if temp.type.tag == "troop":
                self.path = self.table.area(
                    self.cur_cell_pos, temp.type.ran, "line", True)
            if self.click_cell_pos and find_list(self.click_cell_pos, self.path) >= 0 and not self.click_cell_pos == self.cur_cell_pos:
                temp = self.table.giveCell(self.click_cell_pos)
                temp2 = self.table.giveCell(self.cur_cell_pos)
                if temp.type is not None and temp2.type is not None and temp2.type.hit > 0:
                    self.hit_event(temp.pos, temp2.pos)
                self.click_cell_pos = []

    def death_check_event(self):
        if not self.deploy:
            troop = 0
            enemy = 0
            for node in self.table.node_table:
                if node.type is not None:
                    if node.type.die_check():
                        node.type = None
                        node.block = False
                        self.skull_image = pygame.image.load(
                            os.path.join(source_path, "skull.png"))
                        self.skull_image = pygame.transform.scale(
                            self.skull_image, (self.cell_size, self.cell_size))
                        self.screen.blit(self.skull_image, [
                            self.table_screen_pos[0] +
                            node.pos[0] * self.cell_size,
                            self.table_screen_pos[1] + node.pos[1] * self.cell_size])
                        pygame.display.flip()
                        pygame.time.wait(100)
                    elif node.type.tag == "troop":
                        troop += 1
                    elif node.type.tag == "enemy":
                        enemy += 1
            if troop == 0:
                print("MISSION FAILED")
                self.mission_fail.draw()
                pygame.time.wait(2000)
                self.money += 100
                self.phase = "LOSE"
            elif enemy == 0:
                print("MISSION SUCCESS")
                self.mission_success.draw()
                pygame.time.wait(2000)
                self.money += 200
                self.phase = "WIN"

    def trun_end_event(self):
        self.Ai_event("enemy", "troop")
        for cell in self.table.node_table:
            if cell.type is not None and cell.type.tag == "troop":
                cell.type.phase_end()
        self.path = []
        self.cur_cell_pos = []
        self.click_cell_pos = []
        self.phase = []

    def map_draw(self):
        for j in range(0, self.table.rows):
            for i in range(0, self.table.cols):
                tile = int(self.tile_img_map[i][j])
                paint = int(self.obj_img_map[i][j])
                if tile > 0:
                    self.screen.blit(self.tile_img_list[tile - 1], [
                        self.table_screen_pos[0] + i * self.cell_size,
                        self.table_screen_pos[1] + j * self.cell_size])
                if paint > 0:
                    self.screen.blit(self.obj_img_list[paint - 1], [
                        self.table_screen_pos[0] + i * self.cell_size,
                        self.table_screen_pos[1] + j * self.cell_size])

    def army_draw(self):
        for i, txt in enumerate(self.army_list):
            j = i // 8  # (get secreen w - game screen ) / cell size
            k = i % 8
            x = self.table_window[0] + k * self.btn_size[0]/8
            y = self.btn_size[1] + j * self.btn_size[1]
            temp = fun_pyImgBtn.pyImgBtn(
                [x, y], [25, 25], "character/" + txt + ".png", self.screen, i)
            temp.draw(self.mouse_pos)
            click = temp.check(self.event, self.mouse_pos)
            if click is not None:
                self.selection = click
                print("[GAME] : selection " + str(self.selection))
            if self.selection == temp.name:
                pygame.draw.rect(self.screen, [255, 0, 0], [
                    temp.x, temp.y, temp.w, temp.h], 5)

    def path_draw(self):
        if self.path:
            if self.phase == "MOVE" and self.cur_cell_pos:
                temp = self.table.giveCell(self.cur_cell_pos)
                if temp.type is not None and temp.type.tag == "troop":
                    step = copy.deepcopy(temp.type.mov) - 1
                    for way in self.path:
                        s = pygame.Surface([self.cell_size, self.cell_size])
                        s.set_alpha(100)
                        if step >= 0:
                            s.fill([100, 225, 0])
                            step -= 1
                            ttemp = self.table.giveCell(way)
                            if ttemp.obstacle == True or ttemp.block == True:
                                s.fill([255, 0, 0])
                        else:
                            s.fill([255, 0, 0])
                        self.screen.blit(
                            s, (self.table_screen_pos[0] + way[0] * self.cell_size,
                                self.table_screen_pos[1] + way[1] * self.cell_size))
            elif self.phase == "ATTACK" and self.cur_cell_pos:
                temp = self.table.giveCell(self.cur_cell_pos)
                for way in self.path:
                    s = pygame.Surface([self.cell_size, self.cell_size])
                    s.set_alpha(100)
                    if temp.type.hit > 0:
                        s.fill([0, 225, 0])
                        if self.table.giveCell(way).type is not None:
                            s.fill([0, 0, 255])
                    else:
                        s.fill([255, 0, 0])
                    self.screen.blit(
                        s, (self.table_screen_pos[0] + way[0] * self.cell_size,
                            self.table_screen_pos[1] + way[1] * self.cell_size))

    def mouse_draw(self):
        if self.table_window[0] > self.mouse_pos[0] > 0 and self.table_window[1] > self.mouse_pos[1] > 0:
            mouse_cell_pos = [int(self.mouse_pos[0] - self.table_screen_pos[0])//self.cell_size,
                              int(self.mouse_pos[1] - self.table_screen_pos[1])//self.cell_size]
            temp = self.table.giveCell(mouse_cell_pos)
            if temp is not None:
                pygame.draw.rect(self.screen, [255, 255, 255], [
                    self.table_screen_pos[0] + temp.pos[0] * self.cell_size,
                    self.table_screen_pos[1] + temp.pos[1] * self.cell_size,
                    self.cell_size, self.cell_size], 5)

    def fog_unit_draw(self):
        eye_sight = []
        for cell in self.table.node_table:
            if cell.type is not None and cell.type.tag == "troop":
                temp = self.table.water(cell.pos, 5)
                eye_sight += temp
        for cell in self.table.node_table:
            if not (cell.pos in eye_sight):
                s = pygame.Surface(
                    [self.cell_size, self.cell_size], pygame.SRCALPHA)
                s.fill([0, 0, 0, 100])
                self.screen.blit(s, (
                    self.table_screen_pos[0] + cell.pos[0] * self.cell_size,
                    self.table_screen_pos[1] + cell.pos[1] * self.cell_size))
            else:
                if cell.type is not None:
                    self.screen.blit(self.unit_img_list[cell.type.name], [
                        self.table_screen_pos[0] +
                        cell.pos[0] * self.cell_size,
                        self.table_screen_pos[1] + cell.pos[1] * self.cell_size])
                    if cell.type.mov <= 0 and cell.type.hit <= 0:
                        s = pygame.Surface(
                            [self.cell_size, self.cell_size], pygame.SRCALPHA)
                        s.fill([20, 20, 20, 150])
                        self.screen.blit(s, [
                            self.table_screen_pos[0] +
                            cell.pos[0] * self.cell_size,
                            self.table_screen_pos[1] + cell.pos[1] * self.cell_size])
                    x = self.table_screen_pos[0] + cell.pos[0] * self.cell_size
                    y = self.table_screen_pos[1] + cell.pos[1] * \
                        self.cell_size + self.cell_size * 3* 0.25
                    pygame.draw.rect(self.screen, [0, 0, 0], [
                        x, self.table_screen_pos[1] + cell.pos[1] * self.cell_size, self.cell_size, self.cell_size], 2)
                    pygame.draw.rect(self.screen, [0, 0, 0], [
                        x, y, self.cell_size, self.cell_size* 0.25])
                    pygame.draw.rect(self.screen, [255, 0, 0], [
                        x, y, cell.type.hp/cell.type.max_hp * self.cell_size, self.cell_size* 0.25])
                    pygame.draw.rect(self.screen, [0, 0, 0], [
                        x, y, self.cell_size, self.cell_size* 0.25], 2)

    def deploy_draw(self):
        if self.deploy:
            for cell in self.table.node_table:
                if cell.status == "deploy":
                    pygame.draw.rect(self.screen, [0, 255, 0],
                             [self.table_screen_pos[0] + cell.pos[0] * self.cell_size,
                              self.table_screen_pos[1] + cell.pos[1] * self.cell_size,
                              self.cell_size, self.cell_size], 1)

    def mini_draw(self):
        if self.table is not None\
            and self.table.cols is not None and self.table.rows is not None\
            and self.table.cols > 0  and self.table.rows > 0:
            mini_window = [self.table_window[0], 500]
            mini_map_size= [200, 200]
            mini_size = min(mini_map_size[0]//self.table.cols, mini_map_size[1]//self.table.rows)
            mini_cell= [mini_size, mini_size]
            pygame.draw.rect(self.screen,[0,0,0], [mini_window[0], mini_window[1], mini_map_size[0], mini_map_size[1]])
            for j in range(0, self.table.rows):
                for i in range(0, self.table.cols):
                    tile = int(self.tile_img_map[i][j])
                    paint = int(self.obj_img_map[i][j])
                    if tile == 0:
                        pygame.draw.rect(self.screen,[0,0,0],
                        [mini_window[0] + i * mini_cell[0], 
                        mini_window[1] + j * mini_cell[1],
                        mini_cell[0], mini_cell[1]])
                    elif tile > 0:
                        temp = pygame.transform.scale(
                                self.tile_img_list[tile - 1], 
                                (mini_cell[0],mini_cell[1]))
                        self.screen.blit(temp,
                            [mini_window[0] + i * mini_cell[0], 
                            mini_window[1] + j * mini_cell[1]])
                    if paint > 0:
                        temp = pygame.transform.scale(
                                self.obj_img_list[paint - 1], 
                                (mini_cell[0],mini_cell[1]))
                        self.screen.blit(temp,
                            [mini_window[0] + i * mini_cell[0], 
                            mini_window[1] + j * mini_cell[1]])
                    status = self.table.node_table[self.table.cols * j + i].status
                    if status is not None:
                        s = pygame.Surface([mini_cell[0], mini_cell[1]])
                        s.set_alpha(100)
                        if status == "deploy" and self.deploy:
                            s.fill([0,255,0])
                        elif status == "enemy_deploy":
                            s.fill([218,165,32])
                        self.screen.blit(s, [mini_window[0] + i * mini_cell[0], mini_window[1] + j * mini_cell[1]])
            eye_sight = []
            for cell in self.table.node_table:
                if cell.type is not None and cell.type.tag == "troop":
                    temp = self.table.water(cell.pos, 5)
                    eye_sight += temp
            for cell in self.table.node_table:
                if not (cell.pos in eye_sight):
                    s = pygame.Surface(
                        [mini_cell[0], mini_cell[1]], pygame.SRCALPHA)
                    s.fill([0, 0, 0, 124])
                    self.screen.blit(s, (
                        mini_window[0] + cell.pos[0] * mini_cell[0],
                        mini_window[1] + cell.pos[1] * mini_cell[1]))
                elif cell.type is not None:
                    color = [18,255,32]
                    if cell.type.tag == "enemy":
                        color = [218,165,32]
                    pygame.draw.rect(self.screen,color,
                        [mini_window[0] + cell.pos[0] * mini_cell[0], 
                        mini_window[1] + cell.pos[1] * mini_cell[1],
                        mini_cell[0], mini_cell[1]])

            x =  self.table_screen_pos[0] * self.table.cols * mini_size/(self.table.cols * self.cell_size)
            y =  self.table_screen_pos[1] * self.table.rows * mini_size/(self.table.rows * self.cell_size)
            k =  max(self.table.cols * self.cell_size, self.table.rows * self.cell_size)
            w =  mini_map_size[0] * self.table_window[0]/k
            h =  mini_map_size[1] * self.table_window[1]/k   
            pygame.draw.rect(self.screen,[255,255,255], 
                        [mini_window[0] - x, mini_window[1] - y, w, h], 2)
                        
    def information_draw(self):
        if self.click_cell_pos:
            temp = self.table.giveCell(self.click_cell_pos)
            if temp.type is not None:
                txt = "Name : " + str(temp.type.name) \
                    + "\n hp : [" + str(temp.type.hp) + "/" + str(temp.type.max_hp) + "]"\
                    + "\n hit : [" + str(temp.type.hit) + "/" + str(temp.type.max_hit) + "]"\
                    + "\n mov : [" + str(temp.type.mov) + "/" + str(temp.type.max_mov) + "]"\
                    + "\n atk : " + str(temp.type.atk) + " dep : " + str(temp.type.dep)\
                    + "\n spd : " + str(temp.type.spd) + \
                    " range : " + str(temp.type.ran)
                if self.cur_cell_pos is not None and self.click_cell_pos is not None:
                    unit = self.table.giveCell(self.cur_cell_pos)
                    opp = self.table.giveCell(self.click_cell_pos)
                    if unit is not None and opp is not None and unit.type is not None and opp.type is not None:
                        [Cri, Hit, Miss] = unit.type.damage_percentage(
                            opp.type)
                        txt += "\n" + str(unit.type.name) + \
                            " to " + str(opp.type.name)
                        txt += "\n Cri : [" + str(Cri) + "]%"
                        txt += "\n Hit : [" + str(Hit) + "]%"
                        txt += "\n Mis : [" + str(Miss) + "]%"
                        [Cri, Hit, Miss] = opp.type.damage_percentage(
                            unit.type)
                        txt += "\n" + str(opp.type.name) + \
                            " to " + str(unit.type.name)
                        txt += "\n Cri : [" + str(Cri) + "]%"
                        txt += "\n Hit : [" + str(Hit) + "]%"
                        txt += "\n Mis : [" + str(Miss) + "]%"
                self.ttxt.text = txt
            self.ttxt.draw(self.mouse_pos)

    def Ai_turn_draw(self, current, target):
        if current:
            pygame.draw.rect(self.screen, [255, 255, 255], [
                self.table_screen_pos[0] + current[0] * self.cell_size,
                self.table_screen_pos[1] + current[1] * self.cell_size,
                self.cell_size, self.cell_size], 5)
            if target:
                pygame.draw.rect(self.screen, [200, 212, 0], [
                    self.table_screen_pos[0] + target[0] * self.cell_size,
                    self.table_screen_pos[1] + target[1] * self.cell_size,
                    self.cell_size, self.cell_size], 5)

    def btn_draw(self):
        if self.deploy == True:
            for i, btn in enumerate(self.btn2_list):
                btn.x = self.table_window[0]
                btn.y = self.btn_size[1] * i
                btn.w = self.btn_size[0]
                btn.h = self.btn_size[1]
                btn.draw(self.mouse_pos)
                if self.phase == btn.name:
                    pygame.draw.rect(self.screen, [255, 0, 0], [
                        btn.x, btn.y, btn.w, btn.h], 5)
        else:
            for i, btn in enumerate(self.btn_list):
                btn.x = self.table_window[0]
                btn.y = self.btn_size[1] * i
                btn.w = self.btn_size[0]
                btn.h = self.btn_size[1]
                btn.draw(self.mouse_pos)
                if self.phase == btn.name:
                    pygame.draw.rect(self.screen, [255, 0, 0], [
                                     btn.x, btn.y, btn.w, btn.h], 5)

    def table_screen_move(self):
        keys = pygame.key.get_pressed()
        if keys[pygame.K_a]:
            self.table_screen_pos[0] += 10
        elif keys[pygame.K_d]:
            self.table_screen_pos[0] -= 10
        elif keys[pygame.K_w]:
            self.table_screen_pos[1] += 10
        elif keys[pygame.K_s]:
            self.table_screen_pos[1] -= 10
        mouse_pos = pygame.mouse.get_pos()
        speed = 20
        if mouse_pos[0] == self.screen_w - 1:
            self.table_screen_pos[0] -= speed
            if mouse_pos[1] == self.screen_h - 1:
                self.table_screen_pos[1] -= speed
            elif mouse_pos[1] == 0:
                self.table_screen_pos[1] += speed
        elif mouse_pos[0] == 0:
            self.table_screen_pos[0] += speed
            if mouse_pos[1] == self.screen_h - 1:
                self.table_screen_pos[1] -= speed
            elif mouse_pos[1] == 0:
                self.table_screen_pos[1] += speed
        elif mouse_pos[1] == self.screen_h - 1:
            self.table_screen_pos[1] -= speed
            if mouse_pos[0] == self.screen_w - 1:
                self.table_screen_pos[0] -= speed
            elif mouse_pos[0] == 0:
                self.table_screen_pos[0] += speed
        elif mouse_pos[1] == 0:
            self.table_screen_pos[1] += speed
            if mouse_pos[0] == self.screen_w - 1:
                self.table_screen_pos[0] -= speed
            elif mouse_pos[0] == 0:
                self.table_screen_pos[0] += speed

        w = self.table.cols * self.cell_size
        h = self.table.rows * self.cell_size
        if w >= self.table_window[0]:
            if  self.table_screen_pos[0] <= -w + self.table_window[0] :
                self.table_screen_pos[0] = -w + self.table_window[0]
            elif self.table_screen_pos[0] >= 0:
                self.table_screen_pos[0] = 0
        else:
            if self.table_screen_pos[0] >= self.table_window[0] - w :
                self.table_screen_pos[0] = self.table_window[0] - w
            elif self.table_screen_pos[0] <= 0:
                self.table_screen_pos[0] = 0
        if h >= self.table_window[1]:
            if  self.table_screen_pos[1] <= -w + self.table_window[1] :
                self.table_screen_pos[1] = -w + self.table_window[1]
            elif self.table_screen_pos[1] >= 0:
                self.table_screen_pos[1] = 0
        else:
            if self.table_screen_pos[1] >= self.table_window[1] - w :
                self.table_screen_pos[1] = self.table_window[1] - w
            elif self.table_screen_pos[1] <= 0:
                self.table_screen_pos[1] = 0

    def game_event(self):
        self.deploy = True
        while True:
            self.table_screen_move()
            for event in pygame.event.get():
                self.event = event
                self.mouse_pos = pygame.mouse.get_pos()
                self.cell_pos = [(self.mouse_pos[0] - self.table_screen_pos[0])//self.cell_size,
                            (self.mouse_pos[1] - self.table_screen_pos[1])//self.cell_size]
                self.phase_check()
                if self.deploy == True:
                    for btn in self.btn2_list:
                        temp = btn.check(event, self.mouse_pos)
                        if temp is not None:
                            self.phase = temp
                            print("[GAME] : " + str(self.phase))
                            break
                else:
                    for btn in self.btn_list:
                        temp = btn.check(event, self.mouse_pos)
                        if temp is not None:
                            self.path = []
                            self.phase = temp
                            self.turn_image.text = temp
                            break
                if event.type == pygame.QUIT:
                    return "quit"
                elif event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
                    print("[GAME] : ESC")
                    self.phase = ""
                    self.path = []
                    self.click_cell_pos = []
                    self.cur_cell_pos = []     
                else:
                    self.player_mouse_event(event, self.cell_pos)
                if self.phase == "main_menu":
                    return "main_menu"           
            pygame.event.pump()
            self.draw()
            pygame.time.Clock().tick(60)

    def draw(self):
        self.screen.fill((0, 0, 0))
        self.map_draw()
        self.fog_unit_draw()
        self.deploy_draw()
        self.Ai_turn_draw(self.Ai_who, self.Ai_target)
        self.path_draw()
        self.mouse_draw()
        pygame.draw.rect(self.screen, [255, 255, 212], [
                         self.table_window[0], 0, self.btn_size[0], self.screen_h])
        if self.deploy:
            self.army_draw()
        self.mini_draw()
        self.btn_draw()
        self.information_draw()
        pygame.draw.line(self.screen,[255,255,255],[self.table_window[0] - 2, 0], [self.table_window[0] - 2,self.screen_h],2)
        if self.cur_cell_pos:
            temp = self.table.giveCell(self.cur_cell_pos)
            pygame.draw.rect(self.screen, [200, 215, 0], [
                self.table_screen_pos[0] + temp.pos[0] * self.cell_size,
                self.table_screen_pos[1] + temp.pos[1] * self.cell_size,
                self.cell_size, self.cell_size], 5)
        if self.damage_txt.text != "":
            self.damage_txt.draw()
            pygame.display.flip()
            pygame.time.wait(1000)
            self.damage_txt.text = ""
        if self.turn_image.text != "":
            self.turn_image.draw()
            pygame.time.wait(500)
            self.turn_image.text = ""
        self.death_check_event()
        pygame.display.flip()
