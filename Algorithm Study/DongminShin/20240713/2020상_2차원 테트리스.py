# 시간복잡도 : 대략 10만

K=int(input())
point=0
ans=0
n=10
m=4

b=[[0]*m for _ in range(n)]
c=[[0]*n for _ in range(m)]

def transpose(a):
    return list(map(list,zip(*a)))

def blockMove(a,x,y,t):
    
    if t==1:
        i=x
        while i<n-1:
            if a[i+1][y]==0:
                a[i+1][y],a[i][y]=a[i][y],a[i+1][y]
            else:
                break
            i+=1
    elif t==2:
        i=x
        while i<n-1:
            if a[i+1][y]==0 and a[i+1][y+1]==0:
                a[i + 1][y], a[i][y] = a[i][y], a[i + 1][y]
                a[i + 1][y+1], a[i][y+1] = a[i][y+1], a[i + 1][y+1]
            else:
                break
            i+=1
    elif t==3:
        i=x+1
        while i<n-1:
            if a[i+1][y]==0:
                a[i + 1][y], a[i][y] = a[i][y], a[i + 1][y]
                a[i][y],a[i-1][y]=a[i-1][y],a[i][y]
            else:
                break
            i+=1
    
    return a

def check1(a):
    ok=False
    rowInxArr=[]

    for rowInx,row in enumerate(a):
        if 0 not in row:
            ok=True
            rowInxArr.append(rowInx)

    return [ok,rowInxArr]

def remove(rowInxArr,a):
    global point

    cnt=len(rowInxArr)
    point+=cnt

    for rowInx in rowInxArr:
        a[rowInx]=[0]*4

    for _ in range(cnt):
        for rowInx in range(n-1,4,-1):
            if a[rowInx]==[0]*4:
                a[rowInx],a[rowInx-1]=a[rowInx-1],a[rowInx]

    return a

def check2(a):
    ok=False
    cnt=0

    for rowInx in range(4,6):
        if a[rowInx]!=[0]*4:
            cnt+=1
            ok=True

    return [ok,cnt]

def light(a,cnt):

    for _ in range(cnt):
        for rowInx in range(n-1,4,-1):
            a[rowInx]=[0]*4
            a[rowInx], a[rowInx - 1] = a[rowInx - 1], a[rowInx]

    return a

def process(t,x,y,no,a):
    
    # 블록 생성 및 이동
    if t==1:
        a[x][y]=no
        a=blockMove(a,x,y,1)
    elif t==2:
        a[x][y]=no
        a[x][y+1]=no
        a=blockMove(a,x,y,2)
    else:
        a[x][y]=no
        a[x+1][y]=no
        a=blockMove(a,x,y,3)
        
    # 꽉찬 행이 있을 경우 행의 블록 제거
    ok1,rowInxArr=check1(a)
    if ok1:
        a=remove(rowInxArr,a)

    # 연한 부분 블록 처리
    ok2,cnt=check2(a)
    if ok2:
        a=light(a,cnt)
    
    return a

for no in range(1,K+1):
    t,x,y=map(int,input().split())

    b = process(t, x, y, no,b)

    if t==1:
        c = transpose(process(1, y, x, no, transpose(c)))
    elif t==2:
        c=transpose(process(3,y,x,no,transpose(c)))
    else:
        c=transpose(process(2,y,x,no,transpose(c)))

c=transpose(c)
for x in range(n):
    for y in range(m):
        if b[x][y]!=0:
            ans+=1
        if c[x][y]!=0:
            ans+=1
print(point)
print(ans)