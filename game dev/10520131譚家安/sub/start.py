import pygame
from pygame.locals import *
import random

from sub.map import map
from sub.block import block

def show_text( screen, rect, game_font, text, t ) :
  screen.fill((0,0,0))
  
  text_surf = game_font.render(text,
                               True,
                               (255, 255, 255),
                               (0, 0, 0))
                               
  screen.blit(text_surf, (rect.centerx-text_surf.get_rect().centerx,
                          (rect.centery/2)-text_surf.get_rect().centery))
  
  pygame.display.flip()
  pygame.time.delay(int(t*1000))
  for event in pygame.event.get() :
    pass

def show_random_text( screen, rect, game_font, text, t ) :
  screen.fill((0,0,0))
  
  text_surf = game_font.render(text,
                               True,
                               (random.randint(1,255),random.randint(1,255),random.randint(1,255)),
                               (random.randint(1,255),random.randint(1,255),random.randint(1,255)))
                               
  screen.blit(text_surf, ((rect.centerx-text_surf.get_rect().centerx),
                         ((rect.centery/2))-text_surf.get_rect().centery))
  
  pygame.display.flip()
  pygame.time.delay(int(t*1000))
  for event in pygame.event.get() :
    pass
  
def start( screen, rect, game_font, size ) :
  block_font = pygame.font.SysFont(pygame.font.get_default_font(), 30*size)
  show_text( screen, rect, game_font, "3", 1 )
  show_text( screen, rect, game_font, "2", 1 )
  show_text( screen, rect, game_font, "1", 1 )
  show_text( screen, rect, game_font, "START!!", 0.5 )
        
  m = map(size)
  m.set_box( 5 )
  score = 0
  running = True
  click = []
  
  while running and score < 5 :
    for event in pygame.event.get() :
      if event.type == QUIT :
        pygame.quit()
        
      elif event.type == KEYDOWN :
        if event.key == K_ESCAPE :
          running = False
          score = -1
          
      elif click == [] and event.type == MOUSEBUTTONDOWN :
        pos = pygame.mouse.get_pos()
        flag, click = m.who_was_clicked(pos[0], pos[1], size)
        
        if flag :
          m.block_list[click[0]][click[1]].set_down(True)
        else :
          click = []
          
      elif click != [] and event.type == MOUSEBUTTONUP :
        pos = pygame.mouse.get_pos()
        flag, temp = m.who_was_clicked(pos[0], pos[1], size)
        if flag and click[0] == temp[0] and click[1] == temp[1] :
          if [click[0], click[1]] in m.box_list :
            m.block_list[click[0]][click[1]].open("*", size)
            score += 1
            
          else :
            m.block_list[click[0]][click[1]].open("-", size)
          
          m.refresh_distance(size)
            
        else :
          m.block_list[click[0]][click[1]].set_down(False)
        
        click = []
          
    if running :
      screen.fill((0,0,0))
      m.show( screen, block_font, size )
      pygame.display.flip()
      pygame.time.delay(200)
    
  if score == 5 :
    for i in range(20) :
      show_random_text( screen, rect, game_font, "YOU WIN", 0.2 )
      
  screen.fill((0,0,0))
