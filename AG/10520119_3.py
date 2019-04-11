# 演算法分析機測
# 學號: 10520127 / 10520121	/	10520119
# 姓名: 戴承浩   /  杜欣洋	/	黃博泓
#中原大學電資班 feat.資工系

def recursion(tree, head, back):
	count = head
	if back > 1 :
		tree.insert(head+back-1, tree.pop(head) )
		while count < head+back :
			if tree[count].isupper() :
				if tree[count] < tree[head+back-1]:
					count = count + 1
				else :
					tree = recursion(tree, head, count-head)
					tree = recursion(tree,count, head+back-count-1)
					break
			elif int(tree[count]) < int(tree[head+back-1]):
				count = count + 1
			else :
				tree = recursion(tree, head, count-head)
				tree = recursion(tree,count, head+back-count-1)
				break
	return tree
line = input()
while line != '0' :
	x = line.split()
	x = recursion(x, 0, len(x))
	for i in range(0, len(x)-1):
		print( x[i], end= ' ' )
	print( x[len(x)-1] )
	line = input() 
