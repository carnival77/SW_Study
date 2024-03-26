from collections import deque

n,m,K=map(int,input().split())

a=[list(map(int,input().split())) for _ in range(n)] # 0=빈칸, 1=머리사람, 2=나머지, 3=꼬리사람, 4=이동선
b=[[0]*n for _ in range(n)] # 팀 이동선 구분
teams=[None]*(m+1) # 팀별 데크 리스트. 각 팀별 0번째 인덱스가 머리사람, 마지막 인덱스가 꼬리사람
visit=[[False]*n for _ in range(n)]

ans=0

#  우,상,좌,하 - 공이 날아가는 방향
dx=[0,-1,0,1]
dy=[1,0,-1,0]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def bfs(no,sx,sy):
    global b,teams,visit

    b[sx][sy]=no
    visit[sx][sy]=True
    q=deque()
    q.append((sx,sy))

    # 머리부터 팀에 넣어졌고 머리와 인접한 3이 아닌 2부터 팀에 넣기 시작하여 3을 팀에 마지막으로 넣는다
    team=deque()
    head=[sx,sy]
    team.append(head)
    tail=None

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            # 네방향, 범위내, 미방문, 2~4 이고 머리 바로 옆의 3이 아닌 경우
            if inBoard(nx,ny) and 2<=a[nx][ny]<=4 and not visit[nx][ny] and not (a[nx][ny]==3 and (x,y)==(sx,sy)):
                q.append((nx, ny))
                visit[nx][ny] = True
                b[nx][ny] = no
                if a[nx][ny]==2:
                    team.append([nx,ny])
                elif a[nx][ny]==3:
                    tail=[nx,ny]

    team.append(tail)
    teams[no]=team

def init():

    no=1
    for x in range(n):
        for y in range(n):
            if a[x][y]==1 and not visit[x][y]: # 머리 사람 발견 시 bfs 탐색하여 팀 파악
                bfs(no,x,y)
                no+=1

# def getNext(kind,x,y,no):
#
#     if kind==1:
#         for k in range(4):
#             nx, ny = x + dx[k], y + dy[k]
#             if not inBoard(nx, ny): continue
#             if b[nx][ny] == no and 3 <= a[nx][ny] <= 4:
#                 return [nx,ny]
#
#     elif kind==3:
#         for k in range(4):
#             nx, ny = x + dx[k], y + dy[k]
#             if not inBoard(nx, ny): continue
#             if b[nx][ny]==no and a[nx][ny]==2:
#                 return [nx,ny]

def move():
    global teams,b,a

    for no in range(1,m+1):
        team=teams[no]
        if team is None: continue

        tx,ty=team.pop()
        a[tx][ty]=4
        b[tx][ty]=0
        new_tx,new_ty=team[-1]
        a[new_tx][new_ty]=3
        b[new_tx][new_ty]=no
        sx,sy=team[0]
        a[sx][sy]=2
        for k in range(4):
            nx,ny=sx+dx[k],sy+dy[k]
            if not inBoard(nx,ny):continue
            if a[nx][ny]==4:
                team.appendleft([nx,ny])
                a[nx][ny]=1
                b[nx][ny]=no
                break

        # new_team=deque()
        #
        # head=team[0]
        # tail=team[-1]
        # hx,hy=head
        # tx,ty=tail
        #
        # # 머리, 꼬리 다음 이동 위치 구하기
        # nhx,nhy=getNext(1,hx,hy,no)
        # ntx, nty = getNext(3, tx, ty, no)
        #
        # # 중간(나머지) 이동
        # for x,y in list(team)[:-2]:
        #     new_team.append([x,y])
        #
        # # 꼬리 이동
        # a[ntx][nty]=3
        # a[tx][ty]=4
        # new_team.append([ntx,nty])
        #
        # # 머리 이동
        # a[nhx][nhy]=1
        # a[hx][hy]=2
        # new_team.appendleft([nhx,nhy])
        #
        # teams[no]=new_team

def throw(round):

    dir=(round//n)%4 # 공이 날아가는 네 가지 방향
    offset=round%n # 각 방향별 상세 라인

    if dir==0:
        sx=offset
        sy=0
    elif dir==1:
        sx=n-1
        sy=offset
    elif dir==2:
        sx=n-1-offset
        sy=n-1
    else:
        sx=0
        sy=n-1-offset

    for i in range(n):
        nx,ny=sx+dx[dir]*i,sy+dy[dir]*i
        if 1<=a[nx][ny]<=3:
            return [True,nx,ny]

    return [False,-1,-1]

def process(sx,sy):
    global ans,teams,a

    no=b[sx][sy]
    team=teams[no]
    ans+=(team.index([sx,sy])+1)**2
    team.reverse()
    teams[no]=team

    # 방향 전환 맵에 반영
    head=team[0]
    tail=team[-1]
    hx,hy=head
    tx,ty=tail
    a[hx][hy]=1
    a[tx][ty]=3

# 팀 파악
init()

for round in range(K):

    # 팀 이동
    move()
    # 공 던지기
    res=throw(round)
    ok,sx,sy=res
    # 공 맞으면
    if ok:
        # 점수 획득 및 해당 팀 방향 전환
        process(sx,sy)

print(ans)