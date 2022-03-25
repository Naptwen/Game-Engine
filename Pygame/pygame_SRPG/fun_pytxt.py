import pygame 
#___________________________________________________
class pytxt(object):
    x = 0
    y = 0
    w = 125
    h = 50
    rect = None
    myfont = None
    text = None
    rect = None
    font_size = 15
    text_surf = None
    text_rect = None
    text_color = [255,255,255]
    screen = None
    name = None
    
    def __init__(self, text, pos, font_size, text_color, screen, name):
        self.x, self.y = pos
        self.font_size = font_size
        self.text = text
        self.text_color = text_color
        self.screen = screen
        self.myfont = pygame.font.SysFont("arial",self.font_size, True, False)
        self.name = name
    
    def draw(self, mouse):
        h = self.myfont.get_height()
        lines = self.text.split("\n")
        for i, ll in enumerate(lines):
            self.text_surf = self.myfont.render(ll, True, self.text_color)
            self.screen.blit(self.text_surf, (self.x, self.y + (i * h)))
    
    def check(self, event, mouse):
        pass

 

