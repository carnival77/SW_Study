# 시간 복잡도 = O(n^4) = 약 80만

n=int(input())

a=[[0]*n for _ in range(n)]
ans=0

dx=[-1,0,1,0]
dy=[0,-1,0,1]

# 학생 정보 받기
order=[]
like=[None]*(n**2+1)
for _ in range(n**2):
    n0, n1, n2, n3, n4 = map(int, input().split())
    like[n0]=[n1,n2,n3,n4]
    order.append(n0)

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

for round in range(1,n**2+1):

    no=order[round-1]
    # 탑승 위치 탐색
    tx,ty=-1,-1
    cand = [] # 탑승 위치 후보
    for x in range(n):
        for y in range(n):
            cnt = 0  # 좋아하는 친구 수
            empty = 0  # 비어있는 칸의 수
            # 항상 비어있는 칸으로만 이동합니다.
            if a[x][y]!=0:continue
            for k in range(4):
                nx,ny=x+dx[k],y+dy[k]
                if not inBoard(nx,ny):continue
                if a[nx][ny] in like[no]:
                    cnt+=1
                elif a[nx][ny]==0:
                    empty+=1
            cand.append([cnt,empty,x,y])
    # 격자를 벗어나지 않는 4방향으로 인접한 칸 중 앉아있는 좋아하는 친구의 수가 가장 많은 위치로 갑니다.
    # 만약 1번 조건을 만족하는 칸의 위치가 여러 곳이라면, 그 중 인접한 칸 중 비어있는 칸의 수가 가장 많은 위치로 갑니다. 단 이때 격자를 벗어나는 칸은 비어있는 칸으로 간주하지 않습니다.
    # 만약 2번 조건까지 동일한 위치가 여러 곳이라면, 그 중 행 번호가 가장 작은 위치로 갑니다.
    # 만약 3번 조건까지 동일한 위치가 여러 곳이라면, 그 중 열 번호가 가장 작은 위치로 갑니다.
    cand.sort(key=lambda x:(-x[0],-x[1],x[2],x[3]))
    tx,ty=cand[0][-2],cand[0][-1]
    # 학생 탑승
    a[tx][ty]=no

for x in range(n):
    for y in range(n):
        cnt=0
        no=a[x][y]
        for k in range(4):
            nx,ny=x+dx[k],y+dy[k]
            if not inBoard(nx,ny):continue
            if a[nx][ny] in like[no]:
                cnt+=1
        if cnt==0:
            continue
        else:
            ans+=10**(cnt-1)

print(ans)