import pygame 
import os
cur_path = os.path.dirname(__file__)
source_path = os.path.join(cur_path, "images")
#___________________________________________________
class pyImgBtn(object):
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

    def __init__(self, pos, img_size, img, screen, name):
        self.x, self.y = pos
        self.w, self.h = img_size
        self.img = pygame.image.load(os.path.join(source_path, img))
        self.img  = pygame.transform.scale(self.img, (img_size[0],img_size[1]))
        self.screen = screen
        self.name = name

    def _img_change(self, img):
        self.img = pygame.image.load(os.path.join(source_path, img))
        self.img  = pygame.transform.scale(self.img, (self.w, self.h))

    def draw(self, mouse):        
        self.screen.blit(self.img, [self.x, self.y])
        if self.x + self.w > mouse[0] > self.x and self.y + self.h > mouse[1] > self.y:
            pygame.draw.rect(self.screen, [0,0,0], [self.x,self.y,self.w,self.h],5)
            
    def check(self, event, mouse):
        if self.x + self.w > mouse[0] > self.x and self.y + self.h > mouse[1] > self.y:
            if event.type == pygame.MOUSEBUTTONDOWN:
                print("[BUTTON] : return " + str(self.name))
                return self.name
        return None
