# 시간복잡도 : 1000*10*약 10 = 10만
import sys

class Horse:
    def __init__(self,no,x,y,dir):
        self.no=no
        self.x=x
        self.y=y
        self.dir=dir

n,K=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)]
b=[[[] for _ in range(n)] for _ in range(n)]
c=[[[] for _ in range(n)] for _ in range(n)]
hs=[]

#  우,좌,상,하
dx=[0,0,-1,1]
dy=[1,-1,0,0]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def changeDirection(dir):
    if dir==0:
        return 1
    elif dir==1:
        return 0
    elif dir==2:
        return 3
    else:
        return 2

for no in range(1,K+1):
    x,y,d=map(int,input().split())
    x-=1
    y-=1
    d-=1
    horse=Horse(no,x,y,d)
    b[x][y].append(horse)
    c[x][y].append([no,d])
    hs.append(horse)

for turn in range(1,1001):
    for h in hs:
        group=[]
        pos_inx=0
        # index 함수로 고치기
        for inx,horse in enumerate(b[h.x][h.y]):
            if horse.no==h.no:
                pos_inx=inx
                break
        group=b[h.x][h.y][pos_inx:]
        nx,ny=h.x+dx[h.dir],h.y+dy[h.dir]
        if not inBoard(nx,ny) or a[nx][ny]==2:
            h.dir=changeDirection(h.dir)
            nx, ny = h.x + dx[h.dir], h.y + dy[h.dir]
            if not inBoard(nx,ny) or a[nx][ny]==2:
                continue
        if a[nx][ny]==1:
            group.reverse()
        b[nx][ny].extend(group)
        c[nx][ny].extend(group)
        if len(b[nx][ny])>=4:
            print(turn)
            sys.exit(0)
        b[h.x][h.y]=b[h.x][h.y][:pos_inx]
        c[h.x][h.y]=c[h.x][h.y][:pos_inx]
        for horse in group:
            horse.x,horse.y=nx,ny

print(-1)