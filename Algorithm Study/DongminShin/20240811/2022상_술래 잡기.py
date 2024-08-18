n,m,h,K=map(int,input().split())

class Flee:
    def __init__(self,no,x,y,d):
        self.no=no
        self.x=x
        self.y=y
        self.d=d

ans=0
a=[[[] for _ in range(n)] for _ in range(n)] # 도망자 맵
b=[[0]*n for _ in range(n)] # 나무 맵. 1이면 나무 존재
c=[[[] for _ in range(n)] for _ in range(n)] # 디버깅용 도망자 번호 맵
fs=[None]*(m+1)
sx=sy=n//2 # 술래 위치
route=[] # 술래 루트

for no in range(1,m+1):
    x,y,d=map(int,input().split())
    x-=1
    y-=1
    # 이동 방법 d가 1인 경우 좌우로 움직임을, 2인 경우 상하로만 움직임을 뜻합니다.
    # 또한, 좌우로 움직이는 사람은 항상 오른쪽을 보고 시작하며, 상하로 움직이는 사람은 항상 아래쪽을 보고 시작함
    if d==1:
        f = Flee(no,x, y, 1)
    else:
        f=Flee(no,x,y,2)
    fs[no]=f
    a[x][y].append(f)
    c[x][y].append(no)

for _ in range(h):
    x,y=map(int,input().split())
    x-=1
    y-=1
    b[x][y]=1

next=[[0]*n for _ in range(n)]
rev=[[0]*n for _ in range(n)]

#  상,우,하,좌
dx=[-1,0,1,0]
dy=[0,1,0,-1]

def getRoute():
    global next,rev

    x=y=n//2
    move_num=1
    d=0

    while x or y:
        for _ in range(move_num):
            next[x][y]=d
            x,y=x+dx[d],y+dy[d]
            rev[x][y]=(d+2)%4

            if (x,y)==(0,0):
                break

        d=(d+1)%4
        if d==0 or d==2:
            move_num+=1

    next[0][0]=2
    rev[n//2][n//2]=0

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def move1():
    global a,c

    for f in fs:
        if f is None: continue
        no,x,y,d=f.no,f.x,f.y,f.d
        dist=abs(sx-x)+abs(sy-y)
        if dist<=3:
            nx,ny=x+dx[d],y+dy[d]
            if not inBoard(nx,ny):
                d=(d+2)%4
                f.d = d
                nx, ny = x + dx[d], y + dy[d]
            if (sx,sy)!=(nx,ny):
                f.x,f.y=nx,ny
                a[nx][ny].append(f)
                a[x][y].remove(f)
                c[nx][ny].append(f.no)
                c[x][y].remove(f.no)

def move2():
    global sx,sy,ans,a,fs,c

    move_d=route[sx][sy]
    sx,sy=sx+dx[move_d],sy+dy[move_d]
    watch_d=route[sx][sy]

    for i in range(3):
        nx,ny=sx+dx[watch_d]*i,sy+dy[watch_d]*i
        if not inBoard(nx,ny):continue
        if b[nx][ny]==0 and len(a[nx][ny])>0:
            ans+=turn*len(a[nx][ny])
            for f in a[nx][ny]:
                no=f.no
                fs[no]=None
            a[nx][ny].clear()
            c[nx][ny].clear()

turn=0
getRoute()
route=next
for turn in range(1,K+1):
    # 도망자 움직임
    move1()
    # 술래 움직임
    move2()

    # 술래 루트 전환
    if turn%(n**2-1)==0:
        if route==next:
            route=rev
        else:
            route=next

print(ans)