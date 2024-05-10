from itertools import product

n,m=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 맵
b=[[0]*m for _ in range(n)] # 이동 가능 맵(1:이동가능,0:이동불가)
ans=int(1e9)

#  상,우,하,좌
dx=[-1,0,1,0]
dy=[0,1,0,-1]

p=[] # 말 리스트([[kind,x,y],...)
np=[] # 상대 말 위치 리스트([[x,y],...])

# 격자 탐색 - 말 위치 파악
for x in range(n):
    for y in range(m):
        if 1<=a[x][y]<=5:
            p.append([a[x][y],x,y])
        if a[x][y]==6:
            np.append([x,y])

def getDirections(kind,d):
    if kind==1:
        return [d]
    elif kind ==2:
        return [(d-1)%4,(d+1)%4]
    elif kind==3:
        return [d,(d+1)%4]
    elif kind==4:
        return [(d-1)%4,d,(d+1)%4]
    else:
        return [d,(d+1)%4,(d+2)%4,(d+3)%4]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<m:
        return True
    return False

# 모든 말들의 방향 선택에서의 중복 순열 경우의 수
prod=[]
arr=[0,1,2,3]
for event in product(arr,repeat=len(p)):
    prod.append(event)

# 모든 중복순열 경우의 수에서 비어있지만 이동불가한 격자 크기의 최솟값 찾기
for event in prod:
    # 이번 경우의 수에 말별로 비어있지만 이동불가한 격자 크기 찾기
    b=[[0]*m for _ in range(n)]
    # 1. 말 별로 이동가능 격자 파악
    for [kind,sx,sy],direction in zip(p,event):
        dirs=getDirections(kind,direction)
        for d in dirs:
            x,y=sx,sy
            while True:
                nx,ny=x+dx[d],y+dy[d]
                if not inBoard(nx,ny) or a[nx][ny]==6:
                    break
                x,y=nx,ny
                b[x][y]=1

    # 비어있지 않은 곳 모두 체크
    # 1. 원래 내 말 위치도 포함
    for _,x,y in p:
        b[x][y]=1
    # 2. 상대 말 위치 포함
    for x,y in np:
        b[x][y]=2
    # 비어있으면서 이동 불가한 격자 크기 구하고 최솟값 갱신
    res=0
    for x in range(n):
        for y in range(m):
            if b[x][y]==0:
                res+=1
    ans=min(ans,res)
print(ans)