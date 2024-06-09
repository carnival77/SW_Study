from itertools import combinations
from collections import deque

dx=[-1,0,1,0]
dy=[0,-1,0,1]

n,m=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 번진 불 = 3, 원래 불 = 2, 방화벽 = 1, 빈 칸=0

cand=[]

def copyBoard(a):
    return [row[:] for row in a]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<m:
        return True
    return False

b,c=[],[]
for x in range(n):
    for y in range(m):
        if a[x][y]==0:
            b.append([x,y])
        if a[x][y]==2:
            c.append([x,y])

def bfs(a,sx,sy):
    q=deque()
    q.append((sx,sy))

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny) or a[nx][ny]!=0:
                continue
            q.append((nx,ny))
            a[nx][ny]=3
    return a

def process(a):
    res=0

    for x,y in c:
        a=bfs(a,x,y)
    for x in range(n):
        for y in range(m):
            if a[x][y]==0:
                res+=1
    return res

for comb in combinations(b,3):
    tmp=copyBoard(a)
    for x,y in comb:
        tmp[x][y]=1
    cand.append(process(tmp))

cand.sort()
ans=cand[-1]
print(ans)