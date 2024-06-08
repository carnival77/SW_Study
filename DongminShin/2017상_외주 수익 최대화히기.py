n=int(input())

day,point=[],[]
for _ in range(n):
    t,p=map(int,input().split())
    day.append(t)
    point.append(p)

ans=0

def dfs(inx,s):
    global ans

    if inx>n:
        return

    if inx==n:
        ans=max(ans,s)
        return

    dfs(inx+day[inx],s+point[inx])
    dfs(inx+1,s)

dfs(0,0)

print(ans)