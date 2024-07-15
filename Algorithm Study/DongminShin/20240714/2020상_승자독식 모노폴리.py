# 시간복잡도 : 1000*20*20*4 = 약 160만

n,m,K=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 플레이어 번호 맵. 0:빈 칸, 1~ : 플레이어 번호
b=[[0]*n for _ in range(n)] # 독점 계약 번호
c=[[0]*n for _ in range(n)] # 남은 계약 기간

pd=[None]+list(map(int,input().split())) # 플레이어 방향
for i in range(1,m+1):
    pd[i]-=1
dir=[[-1,0],[1,0],[0,-1],[0,1]] # 상,하,좌,우

directions=[None]+[[[0]*4 for _ in range(4)] for _ in range(m)] # 플레이어별 방향

for no in range(1,m+1):
    for d in range(4):
        row=list(map(int,input().split()))
        for i in range(4):
            row[i]-=1
        directions[no][d]=row

def get():
    global b,c

    for x in range(n):
        for y in range(n):
            if a[x][y]==0:continue
            b[x][y]=a[x][y]
            c[x][y]=K

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

# 본인에게 인접한 상하좌우 4 칸 중 아무도 독점계약을 맺지 않은 칸으로 이동하고
# 만약 그러한 칸이 없을 경우에는 인접한 4방향 중 본인이 독점계약한 땅으로 이동합니다.
# 이때 이동할 수 있는 칸이 여러개일 수 있음으로 이동 우선순위에 따라 움직일 칸을 결정하게 됩니다.
# 플레이어가 보고 있는 방향은 그 직전에 이동한 방향입니다.
def move():
    global pd,a

    tmp=[[0]*n for _ in range(n)]
    tpd=[None]+[0]*m

    for x in range(n):
        for y in range(n):
            if a[x][y]==0:continue
            no=a[x][y]
            d=pd[no]
            dirs=directions[no][d]
            # 아무도 독점계약을 맺지 않은 칸으로 이동 가능 여부 확인
            ok1=False
            nx,ny,nd=-1,-1,-1
            for k in range(4):
                nx,ny=x+dir[dirs[k]][0],y+dir[dirs[k]][1]
                if not inBoard(nx,ny):
                    continue
                if b[nx][ny]==0:
                    ok1=True
                    nd=dirs[k]
                    break
            # 만약 그러한 칸이 없을 경우에는 인접한 4방향 중 본인이 독점계약한 땅으로 이동
            if not ok1:
                for k in range(4):
                    nx,ny=x+dir[dirs[k]][0],y+dir[dirs[k]][1]
                    if not inBoard(nx, ny):
                        continue
                    if b[nx][ny] == no:
                        nd = dirs[k]
                        break
            # 만약 이동할 칸이 비어있는 경우
            if tmp[nx][ny]==0:
                tmp[nx][ny]=no
                tpd[no]=nd
            # 만약 이동할 칸이 비어있지 않은 경우
            else:
                tno=tmp[nx][ny]
                if no<tno:
                    tmp[nx][ny]=no
                    tpd[no]=nd
    a=tmp
    pd=tpd

def decrease():
    global c

    for x in range(n):
        for y in range(n):
            if a[x][y]==0 and c[x][y]>0:
                c[x][y]-=1
                if c[x][y]==0:
                    b[x][y]=0

def check():
    for x in range(n):
        for y in range(n):
            if a[x][y]>1:
                return False
    return True

ok=False
for turn in range(1,1001):

    # 플레이어별 현재 위치한 칸 독점 계약
    get()
    # 플레이어 전체 이동
    move()
    # 독점 계약 기간 줄어들기
    decrease()
    # 1번 플레이어만 남았는지 확인
    if check():
        ok=True
        break

if ok:
    print(turn)
if not ok:
    print(-1)