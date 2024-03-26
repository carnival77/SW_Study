from collections import deque

MAX=20
n,m,K=map(int,input().split())

a=[list(map(int,input().split())) for _ in range(n)] # 맵
b=[[[[0]*MAX for _ in range(MAX)] for _ in range(MAX)] for _ in range(MAX)] # 벽 맵. b[r][c][nr][nc]: (r, c) -> (nr, nc) 경로에 벽 존재
c=[[0]*n for _ in range(n)] # 시원함 맵

#  좌,상,우,하
dx=[0,-1,0,1]
dy=[-1,0,1,0]

# 벽 설치
for _ in range(m):
    x,y,s=map(int,input().split()) # 0:위쪽, 1:왼쪽
    x-=1
    y-=1
    if s==0:
        b[x][y][x-1][y]=b[x-1][y][x][y]=1
    elif s==1:
        b[x][y][x][y-1]=b[x][y-1][x][y]=1

# 에어컨 위치
aircon=[] # 에어컨 위치, 종류
for x in range(n):
    for y in range(n):
        if 2<=a[x][y]<=5:
            aircon.append([x,y,a[x][y]-2])

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def spread():
    global c

    for sx,sy,d in aircon:
        tmp = [[0] * n for _ in range(n)]
        nx,ny=sx+dx[d],sy+dy[d]
        num=5
        if not inBoard(nx,ny):continue
        q=deque()
        q.append([nx,ny,num])
        while q:
            x,y,num=q.popleft()
            tmp[x][y] = num
            num-=1
            if num==0:continue

            # 바라보는 방향에서 우측 대각선 45도
            # 시계방향 90도 회전 후 이동할 칸
            nd1 = (d + 1) % 4
            nx1, ny1 = x + dx[nd1], y + dy[nd1]
            if inBoard(nx1, ny1) and b[x][y][nx1][ny1] == 0:
                # 그 후 반시계방향 90도 회전 후 이동할 칸
                nd2 = (nd1 - 1) % 4
                nx2, ny2 = nx1 + dx[nd2], ny1 + dy[nd2]
                if inBoard(nx2,ny2) and b[nx1][ny1][nx2][ny2]==0:
                    # 위 이동할 칸 둘 다 조건 만족 시 큐에 삽입
                    q.append([nx2,ny2,num])
            # 바라보는 방향 정면
            nx1,ny1=x+dx[d],y+dy[d]
            if inBoard(nx1,ny1) and b[x][y][nx1][ny1]==0:
                q.append([nx1,ny1,num])
            # 바라보는 방향에서 좌측 대각선 45도
            # 반시계방향 90도 회전 후 이동할 칸
            nd1 = (d - 1) % 4
            nx1, ny1 = x + dx[nd1], y + dy[nd1]
            if inBoard(nx1, ny1) and b[x][y][nx1][ny1] == 0:
                # 그 후 시계방향 90도 회전 후 이동할 칸
                nd2 = (nd1 + 1) % 4
                nx2, ny2 = nx1 + dx[nd2], ny1 + dy[nd2]
                if inBoard(nx2, ny2) and b[nx1][ny1][nx2][ny2] == 0:
                    q.append([nx2, ny2, num])

        for x in range(n):
            for y in range(n):
                c[x][y]+=tmp[x][y]

def mix():
    global c

    tmp=[row[:] for row in c]

    for x in range(n):
        for y in range(n):
            # 네 방향 탐색 시 값이 서로 다른 두 칸에서 시원함 중복 이동이 발생 가능하므로
            # 좌,상 두 방향만 탐색하여 두 칸 간 탐색은 한 번씩만
            for k in range(2):
                nx,ny=x+dx[k],y+dy[k]
                if not inBoard(nx,ny): continue
                # 좌
                if k==0:
                    if b[x][y][nx][ny]==1:
                        continue
                # 상
                elif k==1:
                    if b[x][y][nx][ny]==1:
                        continue
                dif=abs(c[x][y]-c[nx][ny])//4
                if dif==0: continue
                if c[x][y]>c[nx][ny]:
                    tmp[nx][ny]+=dif
                    tmp[x][y]-=dif
                elif c[x][y]<c[nx][ny]:
                    tmp[nx][ny]-=dif
                    tmp[x][y]+=dif
    c=tmp

def decrease():
    global c

    for x in range(0,n-1):
        if c[x][0]>0:
            c[x][0]-=1
    for y in range(0,n-1):
        if c[n-1][y]>0:
            c[n-1][y]-=1
    for x in range(1,n):
        if c[x][n-1]>0:
            c[x][n-1]-=1
    for y in range(1,n):
        if c[0][y]>0:
            c[0][y]-=1

def check():

    ok=True
    for x in range(n):
        for y in range(n):
            if a[x][y]==1 and c[x][y]<K:
                ok=False
                return ok

    return ok

time=0
while True:
    time+=1

    spread()

    mix()

    decrease()

    if check():
        break

    if time>100:
        break

if time>100:
    print(-1)
else:
    print(time)