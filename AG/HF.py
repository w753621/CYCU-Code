# coding=big5
#Huffman Encoding

#Tree-Node Type
class Node:
  def __init__(self,freq):
    self.left = None
    self.right = None
    self.father = None
    self.freq = freq
  def isLeft(self):
    return self.father.left == self
#create nodes
def createNodes(freqs):
  return [Node(freq) for freq in freqs]

#create Huffman-Tree
def createHuffmanTree(nodes):
  queue = nodes[:]
  while len(queue) > 1:
    queue.sort(key=lambda item:item.freq)
    node_left = queue.pop(0)
    node_right = queue.pop(0)
    node_father = Node(node_left.freq + node_right.freq)
    node_father.left = node_left
    node_father.right = node_right
    node_left.father = node_father
    node_right.father = node_father
    queue.append(node_father)
  queue[0].father = None
  return queue[0]

#Huffman
def huffmanEncoding(nodes,root):
  codes = [''] * len(nodes)
  for i in range(len(nodes)):
    node_tmp = nodes[i]
    while node_tmp != root:
      if node_tmp.isLeft():
        codes[i] = '0' + codes[i]
      else:
        codes[i] = '1' + codes[i]
      node_tmp = node_tmp.father
  return codes


if __name__ == '__main__':
  #chars = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N']
  #freqs = [10,4,2,5,3,4,2,6,4,4,3,7,9,6]
  chars_freqs = []
  password = ""
  a = -1
  number = 0
  i = 0
  a = int( input() )
  while a != 0:
    chars_freqs = []
    number = number + 1
    password = ""
    while a != 0:
      tuple1 = ()
      line = input().split()
      tuple1 = ( line[0], int( line[1] ) )
      a = a - 1
      chars_freqs = chars_freqs + [tuple1]
    password = input()
    nodes = createNodes([item[1] for item in chars_freqs])
    root = createHuffmanTree(nodes)
    codes = huffmanEncoding(nodes,root)
    print( "\n" + "Huffman Codes #" + str(number) ) 
    decode_string = ""
    for item in zip(chars_freqs,codes):
      print( item[0][0], item[1] )
    i = 0
    start = 0
    while i < len(password):
      start = i
      pos = 0
      for item in zip(chars_freqs,codes):
        end = len(item[1])
        end = end + start
        if password.startswith( item[1], start, end ):
          decode_string = decode_string + item[0][0]
          i = end
          break
        else:
          pos = pos + 1
    print( "Decode =",decode_string,"\n" )
    a = int( input() )