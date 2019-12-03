import pygame
import random
import math
from sub.block import block

class map :
  def __init__( self, size ) :
    self.block_list = [[block(i, j, size) for j in range(10)] for i in range(8)]
    self.box_list = []

  def set_box( self, box_n ) :
    while len(self.box_list) < box_n:
        new_box = [random.randint(0,7), random.randint(0,9)]
        if new_box not in self.box_list:
            self.box_list.append(new_box)
      
      
  def who_was_clicked( self, x, y, size ) :
    i = 0
    j = 0
    while i < 8 :
      j = 0
      while j < 10 :
        if self.block_list[i][j].is_in_scope( [x, y], size ) :
          return True, [ i, j ]
        
        j += 1
        
      i += 1
      
    return False, [ i, j ]
    
  def cal_distance( self, a, b ) :
    return round(math.sqrt(math.pow(a[0] - b[0], 2)+ math.pow(a[1] - b[1],2)))
    
  def find_least_distance( self, i, j ) :
    minD = 66666
    for blk in self.box_list :
      if self.block_list[blk[0]][blk[1]].status == "~" :
        distance = self.cal_distance( [i, j], [blk[0], blk[1]] )
        if distance < minD:
          minD = distance
          
    return minD
    
  def refresh_distance( self, size ) :
    i = 0
    j = 0
    while i < 8 :
      j = 0
      while j < 10 :
        if self.block_list[i][j].status != "*" and \
          self.block_list[i][j].status != "~" :
          self.block_list[i][j].open(str(self.find_least_distance(i, j)), size)
          
        j += 1
      i += 1
    
  def show( self, screen, block_font, size ) :
    for i in range(8) :
      for j in range(10) :
        self.block_list[i][j].show( screen, block_font, size )