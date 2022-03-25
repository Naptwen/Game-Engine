import pygame
import os
import fun_pyImgBtn
import fun_pygameboard
import fun_pyImgTxtBtn
from fun_pyMap import *
import fun_pytxt
import fun_pyBtn
import numpy as np
import copy

class main_title(object):
    screen = None
    phase = None
    option = False
    grab = False
    sound = 1
    def __init__(self, screen):
        self.screen = screen
        info = pygame.display.Info()
        self.screen_w = info.current_w
        self.screen_h = info.current_h
        self.phase = None
        self.option = False
        self.grab = False
        self.sound = 1

        self.btn0 = fun_pyImgTxtBtn.pyImgTxtBtn(
        [self.screen_w/2 - 400, self.screen_h/2 - 100], "START", 40, 
        [255,255,255], [190,215,200], [800, 100], "main_menu/banner.png", self.screen, "main_menu")
        self.btn1 = fun_pyImgTxtBtn.pyImgTxtBtn(
        [self.screen_w/2 - 400, self.screen_h/2], "OPTION", 40,
        [255,255,255], [190,215,200], [800, 100], "main_menu/banner.png", self.screen, "option_menu")
        self.btn2 = fun_pyImgTxtBtn.pyImgTxtBtn(
        [self.screen_w/2 - 400, self.screen_h/2 + 100], "MOUSE GRAB", 40,
        [255,255,255], [190,215,200], [400, 100], "main_menu/banner.png", self.screen, "mousegrab")    
        self.btn3 = fun_pyImgTxtBtn.pyImgTxtBtn(
        [self.screen_w/2, self.screen_h/2 + 100], "SOUND : " + str(self.sound), 40,
        [255,255,255], [190,215,200], [400, 100], "main_menu/banner.png", self.screen, "sound")    
        self.btn4 = fun_pyImgTxtBtn.pyImgTxtBtn(
        [self.screen_w/2 - 400, self.screen_h/2 + 100], "MAP EDITOR", 40, 
        [255,255,255], [190,215,200], [800, 100], "main_menu/banner.png", self.screen, "map")

    def main_event(self):
        while True:
            self.screen.fill((0, 0, 0))
            self.mouse_pos = pygame.mouse.get_pos()
            for event in pygame.event.get():
                if self.btn0.check(event, self.mouse_pos) is not None:
                    return "main_menu"
                elif self.btn1.check(event, self.mouse_pos) is not None:
                    self.option = not self.option
                elif self.btn4.check(event, self.mouse_pos) is not None:
                    return "map"
                if self.option == True:
                    self.btn4.y = self.screen_h/2 + 200
                    if self.btn2.check(event, self.mouse_pos) is not None:
                        self.grab = ~self.grab
                        pygame.event.set_grab(self.grab)
                    elif self.btn3.check(event, self.mouse_pos) is not None:
                        self.btn3.text = "SOUND : " + str(self.sound)
                        self.sound += 1
                        if self.sound > 4:
                            self.sound = 0
                else:
                    self.btn4.y = self.screen_h/2 + 100
                if event.type == pygame.QUIT:
                    return "quit"
            self.btn0.draw(self.mouse_pos)        
            self.btn1.draw(self.mouse_pos)
            self.btn4.draw(self.mouse_pos)
            if self.option == True:
                if self.grab:
                    self.btn2.text_color = [255,0,0]
                else :
                    self.btn2.text_color = [100,100,100]
                self.btn2.draw(self.mouse_pos)
                self.btn3.draw(self.mouse_pos)
            pygame.event.clear()
            pygame.time.Clock().tick(60)
            pygame.display.flip()
