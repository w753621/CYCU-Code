# 演算法分析機測
# 學號: 10520127 / 10520121	/	10520119
# 姓名: 戴承浩   /  杜欣洋	/	黃博泓
#中原大學電資班 feat.資工系


def compare( path ,best): #比較數量用
    temp = []
    a = 0
    b = 0

    for i in range( len(line)): #把原本的0101用數字取代
        if (  path[i]== 1 ):
            temp.append(line[i])

    if (len(best) == 0 ):
        best = path
    else:
        for i in range (10):
           if(path[i]) == 1:
              a = a + 1

        for i in range (10):
            if(path[i]) == 1:
                b = b + 1

    return temp
#compare()

def subset(sum,n,want,path,line,best):
    path[n] = 1
    if(sum+line[n]) == want: #找到總和相等的一條路
        best = compare(path,best) #比較看看是否比已經找到的路短
    elif(n+1<len(line)) and (sum + line[n]) <=want:
        best = subset(sum+line[n],n+1,want,path,line,best)


    if(n+1<len(line)) and (sum + line[n+1]) <=want: #不取這個數
        path[n] = 0
        best = subset(sum , n + 1, want, path, line, best)


    return best
#subset()



answer = []
want = eval(input("請輸入\n"))
while want != 0 :
    line = input().strip().split( "{" )[1].split( "}" )[0].strip().split( ", " )

    line = list(map(int, line))
    line.sort()

    path =[0,0,0,0,0,0,0,0,0,0]
    best = []
    sum = 0
    n = 0
    best = subset(sum,n,want,path,line,best)

    if(len(best))!=0:
        answer.append(best)
    else:
        answer.append("No Solution")


    want = eval(input())

'''for i in range (0,len(answer)):
    print(answer[i])

'''


for i in range (0,len(answer)):
    if answer[i] != "No Solution":
        print("{ ", end='')
        for n in range ( len(answer[i])):
            print(answer[i][n], end='')
            if(n+1)<len(answer[i]):
                print(", ", end ='')
            else:
                print(" }")
    else:
        print(answer[i])