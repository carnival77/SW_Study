from collections import deque
from itertools import combinations

n=int(input())
a=[list(map(int,input().split())) for _ in range(n)]
group=[[0]*n for _ in range(n)]
ans=0

groupTotalCnt=0
groupNum=[None]
groupCnt=[None]
visit=[]
near=[]
round=0

dx=[-1,0,1,0]
dy=[0,1,0,-1]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def bfs(sx,sy,no,groupNo):
    global visit,group,groupCnt

    q=deque()
    q.append((sx,sy))
    visit[sx][sy]=True
    group[sx][sy]=groupNo
    cnt=1

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if inBoard(nx,ny) and not visit[nx][ny] and a[nx][ny]==no:
                q.append((nx,ny))
                visit[nx][ny]=True
                group[nx][ny]=groupNo
                cnt+=1

    groupCnt.append(cnt)

def find():
    global groupTotalCnt,visit,groupNum

    visit=[[False]*n for _ in range(n)]
    groupNo=1
    for x in range(n):
        for y in range(n):
            if visit[x][y]:continue
            no=a[x][y]
            bfs(x,y,no,groupNo)
            groupTotalCnt+=1
            groupNo+=1
            groupNum.append(no)

def getPoint():
    global near,ans

    near=[[0]*(groupTotalCnt+1) for _ in range(groupTotalCnt+1)]

    for x in range(n):
        for y in range(n):
            i=group[x][y]
            for k in range(4):
                nx,ny=x+dx[k],y+dy[k]
                if not inBoard(nx,ny):continue
                j=group[nx][ny]
                if i<j:
                    near[i][j]+=1

    arr=[i for i in range(1,groupTotalCnt+1)]
    for comb in combinations(arr,2):
        g1,g2=comb
        ans+=(groupCnt[g1]+groupCnt[g2])*groupNum[g1]*groupNum[g2]*near[g1][g2]

def rotateClockwise(a):
    return list(map(list,zip(*a[::-1])))

def rotateCounterClockwise(a):
    return list(map(list,zip(*a)))[::-1]

def rotate():
    global a
    
    N=n//2
    
    # 반시계 방향 회전
    tmp=rotateCounterClockwise(a)
    for x in range(n):
        a[x][N]=tmp[x][N]
    for y in range(n):
        a[N][y]=tmp[N][y]
    flag1=0

    # 시계 방향 회전
    for x in range(0,n,N+1):
        for y in range(0,n,N+1):
            tmp=[[0]*N for _ in range(N)]
            for i in range(N):
                for j in range(N):
                    tmp[i][j]=a[x+i][y+j]
            rotatedTmp=rotateClockwise(tmp)
            for i in range(N):
                for j in range(N):
                    a[x+i][y+j]=rotatedTmp[i][j]
    flag2=0

find()
getPoint()

def process():
    rotate()
    find()
    getPoint()

for round in range(3):
    groupTotalCnt = 0
    groupNum = [None]
    groupCnt = [None]
    visit = []
    near = []
    process()

print(ans)