from collections import deque

n,m=map(int,input().split())

a=[list(map(int,input().split())) for _ in range(n)]
ans=0

#  우,하,좌,상
dx=[0,1,0,-1]
dy=[1,0,-1,0]
d=0

dice=[1,2,3,4,5,6]
sx,sy=0,0

def rotate(a,d):
    b=[0]*6

    # 우
    if d==0:
        b[0],b[1],b[2],b[3],b[4],b[5]=a[3],a[1],a[0],a[5],a[4],a[2]
    # 하
    elif d==1:
        b[0], b[1], b[2], b[3], b[4], b[5] = a[4],a[0],a[2],a[3],a[5],a[1]
    # 좌
    elif d==2:
        b[0], b[1], b[2], b[3], b[4], b[5] = a[2],a[1],a[5],a[0],a[4],a[3]
    # 상
    else:
        b[0], b[1], b[2], b[3], b[4], b[5] = a[1],a[5],a[2],a[3],a[0],a[4]

    return b

def getPoint(sx,sy,no):
    global ans

    q=deque()
    q.append((sx,sy))
    visit=[[False]*n for _ in range(n)]
    visit[sx][sy]=True
    cnt=1

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny) or visit[nx][ny] or a[nx][ny]!=no:
                continue
            q.append((nx,ny))
            visit[nx][ny]=True
            cnt+=1

    ans+=no*cnt

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

# 처음에는 항상 오른쪽으로 이동
def process():
    global dice,sx,sy,d

    nx,ny=sx + dx[d], sy + dy[d]
    if not inBoard(nx,ny):
        d=(d+2)%4
        nx, ny = sx + dx[d], sy + dy[d]
    sx,sy=nx,ny
    dice=rotate(dice,d)
    getPoint(sx,sy,a[sx][sy])

round=1
process()
for round in range(2,m+1):

    if dice[5]>a[sx][sy]:
        d=(d+1)%4
    elif dice[5]<a[sx][sy]:
        d=(d-1)%4

    process()

print(ans)