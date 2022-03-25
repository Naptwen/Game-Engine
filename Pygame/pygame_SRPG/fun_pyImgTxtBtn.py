import pygame 
import os
cur_path = os.path.dirname(__file__)
source_path = os.path.join(cur_path, "images")
#___________________________________________________
class pyImgTxtBtn(object):
    x = 0
    y = 0
    w = 0
    h = 0
    myfont = None
    text = None
    font_size = 15
    text_surf = None
    text_rect = None
    text_color = [255,255,255]
    screen = None
    img = None
    name = None

    def __init__(self, pos, text,  font_size, text_color, border_color, img_size, img, screen, name):
        self.x, self.y = pos
        self.w, self.h = img_size
        self.text = text
        self.text_color = text_color
        self.img = pygame.image.load(os.path.join(source_path, img))
        self.img  = pygame.transform.scale(self.img, (img_size[0],img_size[1]))
        self.screen = screen
        self.name = name
        self.font_size = font_size 
        self.border_color = border_color

    def _img_change(self, img):
        self.img = pygame.image.load(os.path.join(source_path, img))
        self.img  = pygame.transform.scale(self.img, (self.w, self.h))

    def draw(self, mouse):     
        self.screen.blit(self.img, [self.x, self.y, self.w, self.h])    
        self.myfont = pygame.font.SysFont("arial",self.font_size, True, False)
        self.text_surf = self.myfont.render(self.text, True, self.text_color)
        self.text_rect = self.text_surf.get_rect()
        self.text_rect.center = [self.x + self.w/2, self.y + self.h/2]
        self.screen.blit(self.text_surf, self.text_rect)
        if self.x + self.w > mouse[0] > self.x and self.y + self.h > mouse[1] > self.y:
            pygame.draw.rect(self.screen, self.border_color, [self.x, self.y, self.w, self.h],5)
            
    def check(self, event, mouse):
        if self.x + self.w > mouse[0] > self.x and self.y + self.h > mouse[1] > self.y:
            if event.type == pygame.MOUSEBUTTONDOWN:
                print("[BUTTON] : return " + str(self.name))
                return self.name
        return None
