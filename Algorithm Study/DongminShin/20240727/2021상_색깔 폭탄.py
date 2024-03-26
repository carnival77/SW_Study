# 시간복잡도 : O(n^4*m) = 20^4*m = 약 80만

from collections import deque

n,m=map(int,input().split())
# 0은 빈칸, -1은 해당 칸에 검은색 돌이, 6은 빨간색 폭탄이, 1이상 m이하의 숫자는 빨간색과는 다른 서로 다른 색의 폭탄이 들어가 있음
a=[list(map(int,input().split())) for _ in range(n)]
ans=0
round=0
visit=[]

for x in range(n):
    for y in range(n):
        if a[x][y]==0:
            a[x][y]=6

dx=[-1,0,1,0]
dy=[0,1,0,-1]

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def bfs(sx,sy,no):
    global visit

    q=deque()
    q.append((sx,sy))
    visit[sx][sy]=True
    group=[] # 폭탄 묶음
    group.append([sx,sy])
    cand=[] # 기준점 후보
    cand.append([sx,sy])
    cnt=1
    red=0

    while q:
        x,y=q.popleft()
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny) or visit[nx][ny] or a[nx][ny]==-1:continue
            # 모두 같은 색깔의 폭탄으로만 이루어져 있거나 빨간색 폭탄을 포함하여
            # 정확히 2개의 색깔로만 이루어진 폭탄을 의미합니다.
            if a[nx][ny]==6 or a[nx][ny]==no:
                group.append([nx,ny])
                q.append((nx,ny))
                visit[nx][ny]=True
                if a[nx][ny]==6:
                    red+=1
                if a[nx][ny]==no:
                    cnt+=1
                    cand.append([nx,ny])
    total=cnt+red
    # 여기서 기준점이란, 해당 폭탄 묶음을 이루고 있는 폭탄들 중 빨간색이 아니면서 행이 가장 큰 칸을 의미하며,
    # 만약 행이 가장 큰 폭탄이 여러 개라면 그 중 열이 가장 작은 칸을 의미합니다.
    cand.sort(key=lambda x:(-x[0],x[1]))
    tx,ty=cand[0] # 기준점 위치

    return [total,red,tx,ty,group,cnt]

def restoreRed(visit):

    for x,y in redPos:
        visit[x][y]=False

    return visit

# 현재 격자에서 크기가 가장 큰 폭탄 묶음을 찾습니다.
# 다만, 빨간색 폭탄으로만 이루어져 있는 경우는 올바른 폭탄 묶음이 아니며,
# 모든 폭탄들이 전부 격자 상에서 연결되어 있어야만 합니다.
def find():
    global visit

    cand=[] # 폭탄 묶음 후보
    visit = [[False] * n for _ in range(n)]

    for no in range(1,m+1):
        for x in range(n):
            for y in range(n):
                if a[x][y]==no and not visit[x][y]:
                    total,red,tx,ty,group,cnt=bfs(x,y,no)
                    # 폭탄 묶음이란 2개 이상의 폭탄으로 이루어져 있어야 하며, 빨간색 폭탄으로만 이루어져 있는 경우는 올바른 폭탄 묶음이 아니며
                    if total>=2 and cnt>=1:
                        cand.append([total,red,tx,ty,group])
                    visit=restoreRed(visit) # 빨간색 폭탄은 다른 폭탄 묶음 탐색 중에도 포함되도록 방문 여부 복구
    # 크기가 가장 큰 폭탄 묶음이라는 것은, 가장 많은 수의 폭탄들로 이루어진 폭탄 묶음을 의미합니다.
    # 만약 크기가 큰 폭탄 묶음이 여러 개라면 다음 우선순위에 따라 폭탄 묶음을 선택합니다.
    # (1) 크기가 큰 폭탄 묶음들 중 빨간색 폭탄이 가장 적게 포함된 것 부터 선택합니다.
    # (2) 만약 (1)번 조건까지 동일한 폭탄 묶음이 여러 개라면, 각 폭탄 묶음의 기준점 중 가장 행이 큰 폭탄 묶음을 선택합니다.
    # (3) 만약 (2)번 조건까지 동일한 폭탄 묶음이 여러 개라면, 그 중 폭탄 묶음의 기준점 중 가장 열이 작은 폭탄 묶음을 선택합니다.
    if len(cand)>0:
        cand.sort(key=lambda x:(-x[0],x[1],-x[2],x[3]))
        return cand[0][-1]
    else:
        return None

# 한 round 마다, 폭탄 묶음이 터지면서 폭탄 묶음을 이루고 있는 폭탄의 개수를 C라 했을 때 C * C 만큼의 점수를 얻게 됩니다
def remove(group):
    global ans,a

    ans+=len(group)**2
    for x,y in group:
        a[x][y]=0

# 선택된 폭탄 묶음에 해당되는 폭탄들을 전부 제거합니다.
# 폭탄들이 제거된 이후에는 중력이 작용하여 위에 있던 폭탄들이 떨어지지만,
# 여기서 유의해야 할 점은 돌은 특이한 성질을 띄고 있기 때문에 중력이 작용하더라도 떨어지지 않습니다.
def fall():
    global a

    cnt=1
    while cnt:
        cnt=0
        for x in range(n-1):
            for y in range(n):
                if a[x+1][y]==0 and a[x][y]>=1:
                    a[x+1][y],a[x][y]=a[x][y],a[x+1][y]
                    cnt+=1

def rotateCounterclockwise(a):
    return list(map(list,zip(*a)))[::-1]

# 더 이상 폭탄 묶음이 없을 때까지 계속 반복하려 합니다.
while True:
    round+=1

    # 빨간색 폭탄 위치 찾기
    redPos = []

    for x in range(n):
        for y in range(n):
            if a[x][y] == 6:
                redPos.append([x, y])
    # 폭탄 묶음 찾기. 없으면 중지
    group=find()
    if group is None:
        break
    # 폭탄 묶음 제거
    remove(group)
    # 중력 작용
    fall()
    # 반시계 방향 90도 회전
    a=rotateCounterclockwise(a)
    # 중력 작용
    fall()

print(ans)