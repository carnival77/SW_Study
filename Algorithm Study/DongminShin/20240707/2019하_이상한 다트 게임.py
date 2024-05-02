from collections import deque

n,m,q=map(int,input().split())
ans=0

dx=[-1,0,1,0]
dy=[0,1,0,-1]

def outBound(ny):
    if ny==m:
        return 0
    if ny==-1:
        return m-1
    else:
        return ny

a=[None]
for _ in range(n):
    a.append(deque(list(map(int,input().split()))))

def process():
    ok = False
    b = [[0] * m for _ in range(n + 1)]
    # 원판에 수가 남아있으면
    flag1 = 0
    ok3 = False
    for i in range(1, n + 1):
        for j in range(m):
            if a[i][j] != 0:
                ok3 = True
    if ok3:
        # 인접한 같은 수 찾기
        flag3 = 0
        for x in range(1, n + 1):
            for y in range(m):
                if a[x][y]==0:continue
                for k in range(4):
                    nx, ny = x + dx[k], y + dy[k]
                    if not 1 <= nx <= n: continue
                    ny = outBound(ny)
                    if a[x][y] == a[nx][ny]:
                        b[x][y] = b[nx][ny] = 1
                        ok = True
        flag4 = 0
    # 인접한 같은 수가
    if ok:  # 존재하면 제거
        for i in range(1, n + 1):
            for j in range(m):
                if b[i][j] == 1:
                    a[i][j] = 0
    else:
        # 정규화
        cnt = 0
        s = 0
        for i in range(1, n + 1):
            for j in range(m):
                if a[i][j] > 0:
                    s += a[i][j]
                    cnt += 1

        if cnt > 0:
            avg = s // cnt
            for i in range(1, n + 1):
                for j in range(m):
                    if a[i][j]==0: continue
                    if a[i][j] > avg:
                        a[i][j] -= 1
                    elif a[i][j] < avg:
                        a[i][j] += 1
    flag5 = 0

for turn in range(1,q+1):
    x,d,k=map(int,input().split())

    # 회전
    flag2=0
    for no in range(1,n+1):
        if no%x==0:
            if d==0:
                a[no].rotate(k)
            else:
                a[no].rotate(-k)

    process()

for i in range(1,n+1):
    for j in range(m):
        if a[i][j]!=0:
            ans+=a[i][j]
print(ans)