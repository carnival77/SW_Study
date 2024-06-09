n,m=map(int,input().split())

a=[list(map(int,input().split())) for _ in range(n)]
ans=0

#  우,하,좌,상
dx=[0,1,0,-1]
dy=[1,0,-1,0]

visit=[[0]*m for _ in range(n)]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<m:
        return True
    return False

def dfs(x,y,cnt,s):
    global ans,visit

    if cnt==4:
        ans=max(ans,s)
        return
    if not inBoard(x,y):
        return
    if visit[x][y]==1:
        return
    visit[x][y]=1
    for k in range(4):
        nx,ny=x+dx[k],y+dy[k]
        dfs(nx,ny,cnt+1,s+a[x][y])
    visit[x][y]=0

def simulate(x,y,s):
    global ans

    if inBoard(x-1,y) and inBoard(x+1,y):
        ns=s+a[x-1][y]+a[x+1][y]
        if inBoard(x,y+1):
            ans=max(ans,ns+a[x][y+1])
        if inBoard(x,y-1):
            ans=max(ans,ns+a[x][y-1])

    if inBoard(x,y-1) and inBoard(x,y+1):
        ns=s+a[x][y-1]+a[x][y+1]
        if inBoard(x+1,y):
            ans=max(ans,ns+a[x+1][y])
        if inBoard(x-1,y):
            ans=max(ans,ns+a[x-1][y])

for x in range(n):
    for y in range(m):
        dfs(x,y,0,0)
        simulate(x,y,a[x][y])

print(ans)