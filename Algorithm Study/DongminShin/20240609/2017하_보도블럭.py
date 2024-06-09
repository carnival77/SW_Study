n,l=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)]

ans=0

def inBoard(nx):
    if 0<=nx<n:
        return True
    return False

def process(a):
    global ans
    # 행 탐색
    b=[0]*n # 경사로 설치 여부
    ok = True
    for x in range(n-1):
        #1
        if a[x]==a[x+1]:
            continue
        #2
        elif a[x]==a[x+1]+1:
            for i in range(l):
                if inBoard(x+1+i) and a[x+1]==a[x+1+i] and b[x+1+i]==0:
                    b[x+1+i]=1
                    continue
                else:
                    ok=False
                    break
        #3
        elif a[x]+1==a[x+1]:
            for i in range(l):
                if inBoard(x-i) and a[x]==a[x-i] and b[x-i]==0:
                    b[x-i]=1
                    continue
                else:
                    ok=False
                    break
        #4
        else:
            ok=False
            break
    if ok:
        ans+=1

for row in a:
    process(row)
for x in range(n):
    col = []
    for y in range(n):
        col.append(a[y][x])
    process(col)

print(ans)