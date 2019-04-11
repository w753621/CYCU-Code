# 演算法分析機測
# 學號: 10520127 / 10520121	/	10520119
# 姓名: 戴承浩   /  杜欣洋	/	黃博泓
#中原大學電資班 feat.資工系

def PrintAsubB( des, a, b ):
	x = 0
	y = 0
	if int(a) > int(b):
		c = 'A'
		d = 'B'
	else :
		c = 'B'
		d = 'A'
	if int(a) < int(b) :
		temp = a
		a = b
		b = temp
	while 1:
		if x == 0 :
			print('Fill', c )
			x = int(a)
		else:
			print('Empty', d )
			y = 0
		if x == int(des) or y == int(des) :break
		print('Pour', c, d )
		temp = y
		y = y + x
		if y > int(b): y = int(b)
		x = x - (int(b) - temp )
		if x < 0: x = 0
		if x == int(des) or y == int(des) :break
line = input()
while 1 :
	line = line.split()
	if len(line) == 3:
		if line[0] == '0' and line[1] == '0' and line[2] == '0':break
		PrintAsubB( line[2], line[0],line[1] )
		print('success')
	line = input() 


