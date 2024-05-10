n=100
a=[[0]*n for _ in range(n)]

#  우,상,좌,하
dx=[0,-1,0,1]
dy=[1,0,-1,0]

l=int(input())

def next_generation(dirs):
    next=[]
    res=dirs[::-1]
    for d in res:
        next.append((d+1)%4)
    return next

for _ in range(l):
    x,y,d,g=map(int,input().split())
    a[x][y]=1
    dirs=[d]
    for _ in range(g):
        next=next_generation(dirs)
        dirs+=next
    for d in dirs:
        nx,ny=x+dx[d],y+dy[d]
        a[nx][ny]=1
        x,y=nx,ny

ans=0
for x in range(n-1):
    for y in range(n-1):
        if a[x][y]==a[x][y+1]==a[x+1][y]==a[x+1][y+1]==1:
            ans+=1

print(ans)