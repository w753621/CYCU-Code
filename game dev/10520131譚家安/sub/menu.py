# -*- coding: utf-8 -*-
import pygame
from pygame.locals import *
import random
import os

from sub.start import *

def show_pic( img_path, screen, rect, size ) :
  img=pygame.image.load( img_path ).convert()
  img = pygame.transform.scale(img, (int(img.get_rect().size[0]*size), int(img.get_rect().size[1]*size))).convert()
  img_rect = img.get_rect()
  
  i = 1
  running = True
  button_down = False
  while running:
    for event in pygame.event.get():
      if event.type==QUIT:
        pygame.quit()
        sys.exit()
        
      elif event.type == KEYDOWN :
        if event.key == K_SPACE :
          button_down = True
          running = False
      elif event.type == MOUSEBUTTONDOWN :
        button_down = True
        running = False
          
    img.set_alpha(i)
    screen.fill((0, 0, 0))
    screen.blit(img, (rect.centerx-img_rect.centerx, rect.centery-img_rect.centery))
    pygame.time.delay(10)
    i+=1
    if i==240:
      running = False
    pygame.display.flip()
    
  if not button_down :
    pygame.time.delay(2000)
    running = True
    
  while running:
    for event in pygame.event.get():
      if event.type==QUIT:
        pygame.quit()
        sys.exit()
        
      elif event.type == KEYDOWN :
        if event.key == K_SPACE :
          button_down = True
          running = False
      elif event.type == MOUSEBUTTONDOWN :
        button_down = True
        running = False
          
    img.set_alpha(i)
    screen.fill((0, 0, 0))
    screen.blit(img, (rect.centerx-img_rect.centerx, rect.centery-img_rect.centery))
    pygame.time.delay(5)
    i-=1
    if i==0:
      running = False
    pygame.display.flip()
    
  screen.fill((0, 0, 0))
  pygame.display.flip()

def menu() :
  pic = os.path.dirname(os.path.dirname(__file__)) + "\\pic"
  size = 1
  pygame.init()
  screen = pygame.display.set_mode((400*size,500*size))
  pygame.display.set_caption(u"*10520131 譚家安*")
  pygame.display.set_icon(pygame.image.load(pic + "\\sonar.jpg").convert_alpha())
  rect = screen.get_rect()
  game_font = pygame.font.SysFont(pygame.font.get_default_font(),70*size)
  text_font = pygame.font.SysFont(pygame.font.get_default_font(),40*size)
  
  pygame.mouse.set_pos((200*size,250*size))
  
  screen.fill((0, 0, 0))
  pygame.display.flip()
  
  #show member
  show_pic( pic + "\\sonar.jpg", screen, rect, 0.25 )
  show_pic( pic + "\\member.png", screen, rect, 2 )
  
  #show menu
  
  #gmae name
  GAME_text_surf = game_font.render("Sonar !!",
                                    True,
                                    (255, 255, 255),
                                    (0, 0, 0))        
  screen.blit(GAME_text_surf, (rect.centerx-GAME_text_surf.get_rect().centerx,(rect.centery/2)-GAME_text_surf.get_rect().centery))
  
  #START BUTTON
  START_surf = pygame.Surface((300*size,50*size))
  START_surf.fill((0,0,100))
  screen.blit(START_surf,(rect.centerx-START_surf.get_rect().centerx,rect.centery-START_surf.get_rect().centery))
  
  START_text_surf = text_font.render("START",
                                     True,
                                     (255, 255, 255),
                                     (0, 0, 100))        
  screen.blit(START_text_surf, (rect.centerx-START_text_surf.get_rect().centerx,rect.centery-START_text_surf.get_rect().centery))
  
  #END BUTTON
  END_surf = pygame.Surface((300*size,50*size))
  END_surf.fill((0,0,100))
  screen.blit(END_surf,(rect.centerx-END_surf.get_rect().centerx,(rect.centery*3/2)-END_surf.get_rect().centery))
  
  END_text_surf = text_font.render("END",
                                   True,
                                   (255, 255, 255),
                                   (0, 0, 100))        
  screen.blit(END_text_surf, (rect.centerx-END_text_surf.get_rect().centerx,(rect.centery*3/2)-END_text_surf.get_rect().centery))
  
  pygame.display.flip()  
  
  click = ""
  running = True
  while running :
    for event in pygame.event.get() :
      if event.type == KEYDOWN :
        if event.key == K_ESCAPE :
          running = False
          
      elif click == "" and event.type == MOUSEBUTTONDOWN :
        pos = pygame.mouse.get_pos()
        
        if pos[0] >= (rect.centerx-START_surf.get_rect().centerx) and \
           pos[0] <= (rect.centerx-START_surf.get_rect().centerx+300*size) and \
           pos[1] >= (rect.centery-START_surf.get_rect().centery) and \
           pos[1] <= (rect.centery-START_surf.get_rect().centery+50*size) :
           # click START
          click = "START"
          
          #START BUTTON
          START_surf.fill((0,0,200))
          screen.blit(START_surf,(rect.centerx-START_surf.get_rect().centerx,rect.centery-START_surf.get_rect().centery))
          screen.blit(START_text_surf, (rect.centerx-START_text_surf.get_rect().centerx,rect.centery-START_text_surf.get_rect().centery))
           
        elif pos[0] >= (rect.centerx-END_surf.get_rect().centerx) and \
             pos[0] <= (rect.centerx-END_surf.get_rect().centerx+300*size) and \
             pos[1] >= ((rect.centery*3/2)-END_surf.get_rect().centery) and \
             pos[1] <= ((rect.centery*3/2)-END_surf.get_rect().centery+50*size) :
             # click END
          click = "END"
          
          #END BUTTON
          END_surf.fill((0,0,200))
          screen.blit(END_surf,(rect.centerx-END_surf.get_rect().centerx,(rect.centery*3/2)-END_surf.get_rect().centery))
          screen.blit(END_text_surf, (rect.centerx-END_text_surf.get_rect().centerx,(rect.centery*3/2)-END_text_surf.get_rect().centery))
            
      elif click != "" and event.type == MOUSEBUTTONUP :
        pos = pygame.mouse.get_pos()
        
        if click == "START" and \
           pos[0] >= (rect.centerx-START_surf.get_rect().centerx) and \
           pos[0] <= (rect.centerx-START_surf.get_rect().centerx+300*size) and \
           pos[1] >= (rect.centery-START_surf.get_rect().centery) and \
           pos[1] <= (rect.centery-START_surf.get_rect().centery+50*size) :
           # click START
          start( screen, rect, game_font, size )
          
        elif click == "END" and \
             pos[0] >= (rect.centerx-END_surf.get_rect().centerx) and \
             pos[0] <= (rect.centerx-END_surf.get_rect().centerx+300*size) and \
             pos[1] >= ((rect.centery*3/2)-END_surf.get_rect().centery) and \
             pos[1] <= ((rect.centery*3/2)-END_surf.get_rect().centery+50*size) :
             # click END
          running = False
          
        click = ""
          
        
          
      elif event.type == QUIT :
          running = False

    if running and click == "" :
      #gmae name
      screen.blit(GAME_text_surf, (rect.centerx-GAME_text_surf.get_rect().centerx,(rect.centery/2)-GAME_text_surf.get_rect().centery))
      
      #START BUTTON
      START_surf.fill((0,0,100))
      screen.blit(START_surf,(rect.centerx-START_surf.get_rect().centerx,rect.centery-START_surf.get_rect().centery))
      screen.blit(START_text_surf, (rect.centerx-START_text_surf.get_rect().centerx,rect.centery-START_text_surf.get_rect().centery))
      
      #END BUTTON
      END_surf.fill((0,0,100))
      screen.blit(END_surf,(rect.centerx-END_surf.get_rect().centerx,(rect.centery*3/2)-END_surf.get_rect().centery))
      screen.blit(END_text_surf, (rect.centerx-END_text_surf.get_rect().centerx,(rect.centery*3/2)-END_text_surf.get_rect().centery))
    elif not running :
      BYE_surf = pygame.Surface((400*size,500*size))
      BYE_surf.fill((0,0,0))
      screen.blit(BYE_surf,(0,0))
      
      BYE_text_surf = game_font.render("BYE BYE!!",
                                   True,
                                   (255, 255, 255),
                                   (0, 0, 0))        
      screen.blit(BYE_text_surf, (rect.centerx-BYE_text_surf.get_rect().centerx,(rect.centery/2)-BYE_text_surf.get_rect().centery))
    
    pygame.display.flip()
  
  pygame.time.delay(500)
  pygame.quit()
  
