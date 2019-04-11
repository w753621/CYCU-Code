# 演算法分析機測
# 學號: 10520127 / 10520121	/	10520119
# 姓名: 戴承浩   /  杜欣洋	/	黃博泓
#中原大學電資班 feat.資工系

class B234tree():
	def __init__ (self, root, head):
		self.head = head
		self.num = [root, None, None]
		self.sub = [None, None, None, None]
	def divide(sub, self ):
		full = True
		if self == None: return 
		for i in range(3):
			if self.num[i] == None : full = False
		if full :	# 滿 節點
			temp = self.head
			subhead = B234tree(self.num.pop(1), None)
			self.num.insert(2, None)
			subhead.sub[0] = self
			subhead.sub[1] = B234tree( self.num.pop(1), subhead )
			self.num.insert(2, None)
			self.head = subhead
			subhead.sub[1].sub[0] = self.sub[2]
			subhead.sub[1].sub[1] = self.sub[3]
			subhead.sub[0].sub[2] = None
			subhead.sub[0].sub[3] = None
			
			if int(subhead.num[0]) > int(sub.num[0]): ptr = subhead.sub[0]
			else : ptr = subhead.sub[1]
			if int(ptr.num[0]) > int(sub.num[0]) : i = 0 
			else : i = 1
			ptr.num.pop(2)
			ptr.num.insert(i,sub.num[0])
			ptr.sub.pop(3)
			ptr.sub.pop(i)
			ptr.sub.insert(i,sub.sub[1])
			if sub.sub[1] != None :self.sub[i].head = ptr
			ptr.sub.insert(i,sub.sub[0])
			if sub.sub[0] != None :self.sub[i].head = ptr
			subhead.divide( temp )
		else :		# 可塞
			i = 0
			for i in range(3):
				if self.num[i] == None: break
				if int(sub.num[0]) < int(self.num[i]) : break  
			self.num.pop(2)
			self.num.insert(i, sub.num[0])
			self.sub.pop(3)
			self.sub.pop(i)
			self.sub.insert(i, sub.sub[1])
			sub.sub[1].head = self
			self.sub.insert(i, sub.sub[0])
			sub.sub[0].head = self
	def insertval(self, key):
		leaf = True
		for w in range(4):
			if self.sub[w] != None : leaf = False
		if leaf :			# 沒有子節點
			for i in range(3):
				if self.num[i] == None : leaf = False
			if leaf :		# leaf已放滿要先分裂
				key = B234tree(key, None)
				key.divide(self)
			else : 			# 在leaf放入值
				for w in range(3):
					if self.num[w] == None : 
						self.num.insert(w, key )
						break
					elif int(self.num[w]) > int(key):
						self.num.insert(w, key )
						break
				self.num.pop(3)
		else :				# 遞迴子樹
			i = 0
			for i in range(4) :
				if i == 3:break
				if self.num[i] == None :
					break
				if int(key) < int(self.num[i]) :
					self.sub[i].insertval( key )
					return
			self.sub[i].insertval(key)
def top(self):
	if( self.head != None ):
		return top(self.head)
	return self

def Preprinttree(tree):
	print( '(%s'%tree.num[0], end='' )
	if tree.num[1] != None: print( ',', tree.num[1], end = '' )
	if tree.num[2] != None: print( ',', tree.num[2], end = '' )
	print( end = ') ' )
	for i in range(4):
		if tree.sub[i] != None: Preprinttree(tree.sub[i])
		
def Postprinttree(tree):
	for i in range(4):
		if tree.sub[i] != None: Postprinttree(tree.sub[i])
	print( '(%s'%tree.num[0], end='' )
	if tree.num[1] != None: print( ',', tree.num[1], end = '' )
	if tree.num[2] != None: print( ',', tree.num[2], end = '' )
	print( end = ') ' )
line = input() 
line = line.split()
tree = None 
for i in range(len(line)):
	if tree != None :
		tree.insertval(line[i])
		tree = top(tree)
	else :
		tree = B234tree(line[i], None)
print('2-3-4 Tree (Preorder):')	
Preprinttree(tree)
print()
print('2-3-4 Tree (Postorder):')
Postprinttree(tree)

