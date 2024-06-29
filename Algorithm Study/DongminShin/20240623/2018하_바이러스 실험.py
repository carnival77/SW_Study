n,m,K=map(int,input().split())
a=[[5]*n for _ in range(n)] # 양분 맵
c=[list(map(int,input().split())) for _ in range(n)] # 양분 추가 맵
b=[[[] for _ in range(n)] for _ in range(n)] # 바이러스 맵
ans=0

# 8방향
dx=[-1,-1,-1,0,0,1,1,1]
dy=[-1,0,1,-1,1,-1,0,1]


def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

for _ in range(m):
    x,y,age=map(int,input().split())
    b[x-1][y-1].append(age)

for round in range(1,K+1):

    p=[[0]*n for _ in range(n)]
    for x in range(n):
        for y in range(n):
            live=[]
            dead=0
            b[x][y].sort()
            for inx,age in enumerate(b[x][y]):
                if a[x][y]>=age:
                    a[x][y]-=age
                    live.append(age+1)
                    if (age+1)%5==0:
                        for k in range(8):
                            nx, ny = x + dx[k], y + dy[k]
                            if not inBoard(nx, ny): continue
                            p[nx][ny]+=1
                else:
                    dead+=age//2
            a[x][y]+=dead
            b[x][y]=live
            a[x][y]+=c[x][y]

    for x in range(n):
        for y in range(n):
            for _ in range(p[x][y]):
                b[x][y].append(1)

for x in range(n):
    for y in range(n):
        if len(b[x][y])>0:
            ans+=len(b[x][y])

print(ans)