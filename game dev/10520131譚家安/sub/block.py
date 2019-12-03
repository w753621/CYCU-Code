import pygame

class block :
  def __init__( self, x, y, size ) :
    self.block_surf = pygame.Surface((40*size,40*size))
    self.status = '~' # '~' : cover, '*' : box, num : distance
    self.down = False
    self.pos = [x*50*size,y*50*size]
    
  def set_down( self, set ) :
    self.down = set
  
  def is_in_scope( self, target_pos, size ) :
    return ( self.status == '~' and \
             target_pos[0] >= self.pos[0]+5*size and \
             target_pos[0] <= self.pos[0]+45*size and  \
             target_pos[1] >= self.pos[1]+5*size and  \
             target_pos[1] <= self.pos[1]+45*size )
    
  def open( self, set, size ) :
    self.status = set
    if self.status == '*' : # is box
      self.block_surf = pygame.Surface((30*size,30*size))

  def show( self, screen, block_font, size ) :
    if self.status == '~' : # cover
      if self.down : # down, Dark gray 80*80
        self.block_surf.fill((100,100,100))
      
      else : # Gray 80*80
        self.block_surf.fill((50,50,50))
        
      screen.blit(self.block_surf,(self.pos[0]+5*size, self.pos[1]+5*size))

    elif self.status == '*' : # box, Red 60*60
      self.block_surf.fill((200,0,0))
      screen.blit(self.block_surf,(self.pos[0]+10*size, self.pos[1]+10*size))
     
    else : # distance, White 80*80
      self.block_surf.fill((255,255,255))
      if int(self.status) <= 3 : # Red word
        text_surf = block_font.render(self.status, True, (200, 0, 0), (255, 255, 255))
        
      else : # Black word
        text_surf = block_font.render(self.status, True, (0, 0, 0), (255, 255, 255))
      
      self.block_surf.blit(text_surf, (self.block_surf.get_rect().centerx-text_surf.get_rect().centerx,
                                        self.block_surf.get_rect().centery-text_surf.get_rect().centery))
                                        
      screen.blit(self.block_surf,(self.pos[0]+5*size, self.pos[1]+5*size))
