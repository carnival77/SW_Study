from collections import deque

n,l,r=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)]
MAX=2000
ans=0
visit=[]

dx=[-1,0,1,0]
dy=[0,-1,0,1]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def bfs(sx,sy,g_no):
    global visit,group_cnt,group_sum,group,ok

    q=deque()
    q.append((sx,sy))
    visit[sx][sy]=True
    group[sx][sy]=g_no
    g_sum=a[sx][sy]
    g_cnt=1

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny) or visit[nx][ny]:
                continue
            diff=abs(a[x][y]-a[nx][ny])
            if l<=diff<=r:
                q.append((nx,ny))
                visit[nx][ny]=True
                g_sum+=a[nx][ny]
                g_cnt+=1
                group[nx][ny]=g_no
                ok=True

    if g_cnt>1:
        group_cnt[g_no]=g_cnt
        group_sum[g_no]=g_sum

def check():
    global g_no

    for x in range(n):
        for y in range(n):
            if visit[x][y]:continue
            bfs(x,y,g_no)
            g_no+=1

for round in range(1,MAX+1):
    # 라운드별 변수 초기화
    g_no=1
    group_cnt=[0]*2501
    group_sum=[0]*2501
    group=[[0]*n for _ in range(n)]
    visit = [[False] * n for _ in range(n)]
    ok=False

    # 이동 가능 여부 체크 및 대상 파악
    check()
    if not ok:
        break
    # 이동하여 하나로 합치고 다시 분리
    for x in range(n):
        for y in range(n):
            no=group[x][y]
            if group_cnt[no]>=2:
                a[x][y]=group_sum[no]//group_cnt[no]

    ans+=1

print(ans)