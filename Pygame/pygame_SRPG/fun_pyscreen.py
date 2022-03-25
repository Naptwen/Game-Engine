import pygame
import os
import fun_pyBtn
import fun_pygameboard
import fun_pyImgBtn
import fun_pyMap
import fun_pytxt
import numpy as np
import copy
#___________________________________________________
cur_path = os.path.dirname(__file__)
source_path = os.path.join(cur_path, "images")
#___________________________________________________
class main_title:
    img_main = None
    screen_window = None

    def __init__(self, screen):
        self.screen_window = screen
        img_main = pygame.image.load(os.path.join(source_path, "test.png"))
        img_main = pygame.transform.scale(img_main, (1200,600))
        self.img_main = img_main

    def display(self):    
        self.screen_window.blit(self.img_main, (0,0))
        
    def key_handler(self, event):
        if event.type == pygame.MOUSEBUTTONUP:
            print("move to main menu")
            return "main_menu"
        return "main_title"
#___________________________________________________
class main_menu:
    screen_window = None
    btn1 = None
    btn2 = None
    btn3 = None
    back_img = None
    def __init__(self, screen):
        self.screen_window = screen
        self.btn1 = fun_pyBtn.Button("GAME",[1000,0],[200,100],20,[0,0,0],[100,100,100], self.screen_window, "GAME")
        self.btn2 = fun_pyBtn.Button("SHOP",[1000,100],[200,100],20,[0,0,0],[100,100,100], self.screen_window, "SHOP")
        self.btn3 = fun_pyBtn.Button("MAP",[1000,200],[200,100],20,[0,0,0],[100,100,100], self.screen_window, "MAP")
        self.back_img = pygame.image.load(os.path.join(source_path, "main_menu.jpg"))
        self.back_img = pygame.transform.scale(self.back_img, (1000,600))

    def display(self):    
        self.screen_window.fill([255,255,255])
        self.screen_window.blit(self.back_img, (0,0))
        self.btn1.draw()
        self.btn2.draw()
        self.btn3.draw()

    def key_handler(self, event):    
        if self.btn1.check(event) is not None:
            print("moving to map view")
            return "game_menu"
        elif self.btn2.check(event)is not None:
            print("moving to shop view")
            return "shop_menu"
        elif self.btn3.check(event)is not None:
            print("moving to map view")
            return "map_menu"
        if event.type == pygame.QUIT:
            return "quit"
        return "main_menu"
#___________________________________________________
class game_menu(object):
    screen_window = None
    game1 = None
    tile_size = 50
    def __init__(self, screen):
        self.screen_window = screen 

    def display(self):    
        self.screen_window.fill([255,255,255])

    def key_handler(self,event):
        return self.game1.game_event(event)  
#___________________________________________________

#___________________________________________________
class shop_menu:
    screen_window = None
    icon_size = [50,50]
    btn_list = []
    cur_mode = ""
    money = 0
    army = []
    def __init__(self, screen):
        self.money = 1000
        self.screen_window = screen
        img_main = pygame.image.load(os.path.join(source_path, "shop.jpg"))
        img_main = pygame.transform.scale(img_main, (800,600))
        gold = pygame.image.load(os.path.join(source_path, "gold.png"))
        gold = pygame.transform.scale(gold, (50,50))
        self.img_main = img_main
        self.gold = gold

        btn0 = fun_pyImgBtn.pyImgBtn([1000,0],self.icon_size,"Peasant.png", self.screen_window, "Peasant")
        btn1 = fun_pyImgBtn.pyImgBtn([1050,0],self.icon_size,"Footman.png", self.screen_window, "Footman")
        btn2 = fun_pyImgBtn.pyImgBtn([1100,0],self.icon_size,"Archer.png", self.screen_window, "Archer")
        btn10 = fun_pyImgBtn.pyImgBtn([1000,500],self.icon_size,"cancle.png", self.screen_window, "cancle")
        btn11 = fun_pyBtn.Button("SAVE",[1100,500],[100,50],20,[0,0,0],[100,100,100], self.screen_window, "SAVE")
        btn12 = fun_pyBtn.Button("LOAD",[1100,550],[100,50],20,[0,0,0],[100,100,100], self.screen_window, "LOAD")
        self.btn_list.append(btn0)
        self.btn_list.append(btn1)
        self.btn_list.append(btn2)
        self.btn_list.append(btn10)
        self.btn_list.append(btn11)
        self.btn_list.append(btn12)
        self.cur_mode = ""

    def display(self):    
        self.screen_window.fill([0,0,0])
        self.screen_window.blit(self.img_main, (0,0))
        self.screen_window.blit(self.gold, (1000,400))
        for btn in self.btn_list:
            btn.draw()
            if self.cur_mode == btn.name:
                pygame.draw.rect(self.screen_window, [255,0,0], [btn.x,btn.y,btn.w,btn.h], 5)
        temp = fun_pytxt.pyText(str(self.money),[1060,400],15, [255,255,255], self.screen_window)
        temp.draw()
        for i, temp in enumerate(self.army):
            j = i // 4
            i = i % 4
            btn = fun_pyImgBtn.pyImgBtn([850 + i * 25, 0 + j * 25],[25,25],\
                temp + ".png", self.screen_window, temp)
            btn.draw()
        
    def key_handler(self, event):
        for btn in self.btn_list:
            name = btn.check(event)
            if name is not None:
                self.cur_mode = name
                break
        if self.cur_mode == "Peasant" and self.money >= 100:
            print("buy Peasant")
            self.money -= 100
            self.army.append("Peasant")
            self.cur_mode = []
        if self.cur_mode == "Footman" and self.money >= 300:
            print("buy Footman")
            self.money -= 300
            self.army.append("Footman")
            self.cur_mode = []
        elif self.cur_mode == "Archer" and self.money >= 400:
            print("buy Archer")
            self.money -= 400
            self.army.append("Archer")
            self.cur_mode = []
        elif self.cur_mode == "cancle":
            self.cur_mode = []
            return "main_menu"
        elif self.cur_mode == "SAVE":
            fun_pyMap.army_txt_export("save.txt", self.army, self.money)
            self.cur_mode = []
        elif self.cur_mode == "LOAD":
            self.money, self.army = fun_pyMap.army_txt_import("save.txt")
            self.cur_mode = []
        if event.type == pygame.QUIT:
            return "quit"
        return "shop_menu"