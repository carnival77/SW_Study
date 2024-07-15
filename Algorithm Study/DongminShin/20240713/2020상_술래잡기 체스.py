# 시간 복잡도 : 상어가 15마리를 다 먹을 때까지, 각 턴당 최대로 3번씩 움직인다면 최대 3^15 = 약 1천 4백만

a=[[0]*4 for _ in range(4)] # 도둑 번호
b=[[0]*4 for _ in range(4)] # 도둑 방향
n=4

dx=[-1,-1,0,1,1,1,0,-1]
dy=[0,-1,-1,-1,0,1,1,1]

ans=0
answer=0

for i in range(4):
    row=list(map(int,input().split()))
    for j in range(8):
        if j%2==0:
            a[i][j//2]=row[j]
        else:
            b[i][j//2]=row[j]-1

def inBoard(nx,ny):
    if 0<=nx<n and 0<=ny<n:
        return True
    return False

def move1(a,b,tx,ty):

    for no in range(1,17):
        ok = False
        for x in range(n):
            for y in range(n):
                if a[x][y]==no:
                    d=b[x][y]
                    nx,ny=-1,-1
                    for _ in range(7):
                        nx,ny=x+dx[d],y+dy[d]
                        if not inBoard(nx,ny) or (nx,ny)==(tx,ty):
                            d=(d+1)%8
                        else:
                            ok=True
                            break
                    if ok:
                        b[x][y]=d
                        b[nx][ny],b[x][y]=b[x][y],b[nx][ny]
                        a[nx][ny],a[x][y]=a[x][y],a[nx][ny]
                        break
            if ok:
                break

    return [a,b]

def copyBoard(a):
    return [row[:] for row in a]

def dfs(tx,ty,a,b,ans):
    global answer

    # 도둑 잡기
    ans+=a[tx][ty]
    td=b[tx][ty]
    a[tx][ty]=0
    b[tx][ty]=0

    # 도둑 이동
    a,b=move1(a,b,tx,ty)

    # 술래 이동
    for i in range(1,4):
        nx,ny=tx+dx[td]*i,ty+dy[td]*i
        if inBoard(nx,ny) and a[nx][ny]!=0:
            dfs(nx,ny,copyBoard(a),copyBoard(b),ans)
        else:
            answer=max(answer,ans)

    return

dfs(0,0,a,b,0)

print(answer)