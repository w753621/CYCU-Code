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
	print(x)
	line = input() 
