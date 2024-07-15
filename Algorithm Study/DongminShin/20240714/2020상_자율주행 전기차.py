# 시간복잡도 : 최대 400명의 승객이 주어질 때 각각 승객을 태울 때 전체 맵 bfs로 탐색 = 400*20*20 = 약 16만

from collections import deque

n,m,c=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 0:도로, 1:벽
b=[[0]*n for _ in range(n)] # 0:빈 칸, 1~ : 승객 번호
start=[None]
finish=[None]
success=[None]+[0]*m
energy=c

#  상,좌,하,우
dx=[-1,0,1,0]
dy=[0,-1,0,1]

sx,sy=map(int,input().split())
sx-=1
sy-=1

for no in range(1,m+1):
    x_s,y_s,x_e,y_e=map(int,input().split())
    x_s, y_s, x_e, y_e=x_s-1,y_s-1,x_e-1,y_e-1
    b[x_s][y_s]=no
    start.append([x_s,y_s])
    finish.append([x_e,y_e])

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def move1():
    global energy,sx,sy,b

    if b[sx][sy]!=0:
        no=b[sx][sy]
        b[sx][sy] = 0
        return [True,no]

    q=deque()
    q.append((sx,sy))
    d=[[-1]*n for _ in range(n)]
    d[sx][sy]=0
    cand=[]

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny) or a[nx][ny]==1 or d[nx][ny]!=-1:
                continue
            q.append((nx,ny))
            d[nx][ny]=d[x][y]+1
            if b[nx][ny]!=0:
                cand.append([d[nx][ny],nx,ny,b[nx][ny]])

    if len(cand)>0:
        cand.sort(key=lambda x:(x[0],x[1],x[2]))
        no=cand[0][-1]
        dist=cand[0][0]
        tx=cand[0][1]
        ty=cand[0][2]

        if energy-dist>=0:
            energy-=dist
            sx,sy=tx,ty
            b[tx][ty]=0
            return [True,no]

    return [False,-1]

def move2(ex,ey):
    global energy,sx,sy

    q=deque()
    q.append((sx,sy))
    d=[[-1]*n for _ in range(n)]
    d[sx][sy]=0

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx, ny) or a[nx][ny] == 1 or d[nx][ny] != -1:
                continue
            q.append((nx, ny))
            d[nx][ny] = d[x][y] + 1
            if (nx,ny)==(ex,ey):
                dist=d[nx][ny]
                if energy-dist>=0:
                    energy-=dist
                    energy+=dist*2
                    sx,sy=nx,ny
                    return True

    return False

ok=True
while True:

    # 승객 선택 및 이동
    ok1,no=move1()
    if not ok1:
        ok=False
        break
    ex,ey=finish[no]

    # 목적지로 이동
    ok2=move2(ex,ey)
    if not ok2:
        ok=False
        break
    success[no]=1

    # 남아 있는 승객 없으면 성공
    if 0 not in success[1:]:
        ok=True
        break

if ok:
    print(energy)
else:
    print(-1)


