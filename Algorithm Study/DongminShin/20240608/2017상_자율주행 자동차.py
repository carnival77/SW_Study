n,m=map(int,input().split())
x,y,d=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 도로 = 0, 인도 = 1

#  상,우,하,좌
dx=[-1,0,1,0]
dy=[0,1,0,-1]

ans=0
b=[[0]*m for _ in range(n)] # 미방문 = 0, 방문 = 1
b[x][y]=1

while True:
    nd=(d-1)%4
    nx,ny=x+dx[nd],y+dy[nd]
    cnt=1
    while True:
        if a[nx][ny]==1 or b[nx][ny]==1:
            nd=(nd-1)%4
            nx,ny=x+dx[nd],y+dy[nd]
            cnt+=1
            if cnt>=4:
                break
        else:
            break
    if cnt>=4:
        if a[nx][ny]==1 or b[nx][ny]==1:
            nx,ny=x-dx[nd],y-dy[nd]
            if a[nx][ny]==1:
                break
        else:
            nx,ny=x+dx[nd],y+dy[nd]
    b[nx][ny]=1
    x,y=nx,ny
    d=nd

for x in range(n):
    for y in range(m):
        if b[x][y]==1:
            ans+=1
print(ans)