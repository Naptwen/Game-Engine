import pygame 
import os
cur_path = os.path.dirname(__file__)
source_path = os.path.join(cur_path, "images")
#___________________________________________________
class pyImgTxt(object):
    x = 0
    y = 0
    w = 0
    h = 0
    k = 0
    z = 0
    myfont = None
    text = None
    font_size = 15
    text_surf = None
    text_rect = None
    text_color = [255,255,255]
    screen = None
    img = None
    center = False
    def __init__(self, text, pos, font_size, text_color, text_pos, img, img_size, screen):
        self.x, self.y = pos
        self.w, self.h = img_size
        self.font_size = font_size
        self.text = text
        self.text_color = text_color
        self.img = pygame.image.load(os.path.join(source_path, img))
        self.img  = pygame.transform.scale(self.img, (img_size[0],img_size[1]))

        self.screen = screen
        self.myfont = pygame.font.SysFont("arial",self.font_size, True, False)
        
        if text_pos == None:
            self.center = True
        else:
            self.k, self.z = text_pos


    def draw(self):     
        if self.center == True :
            self.rect = self.img.get_rect()
            self.rect.center = [self.x, self.y]
            self.screen.blit(self.img, self.rect)
            self.text_surf = self.myfont.render(self.text, True, self.text_color)
            self.text_rect = self.text_surf.get_rect()
            self.text_rect.center = [self.x, self.y]
            self.screen.blit(self.text_surf, self.text_rect)   
        else:
            self.screen.blit(self.img, [self.x, self.y, self.w, self.h])
            h = self.myfont.get_height()
            lines = self.text.split("\n")
            for i, ll in enumerate(lines):
                self.text_surf = self.myfont.render(ll, True, self.text_color)
                self.screen.blit(self.text_surf, (self.k + self.x, self.z + self.y + (i * h)))
   

