import pygame
def timer(frame, time):
    clock = pygame.time.Clock()
    showing_time = round(100/frame, 2)
    clock += time