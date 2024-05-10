from itertools import combinations

MAX=int(1e9)

n,m=map(int,input().split())
a=[list(map(int,input().split())) for _ in range(n)] # 0 : 빈칸, 1 : 사람, 2 : 병원
ans=MAX

# 사람,병원 위치 찾기
p,h=[],[]
for x in range(n):
    for y in range(n):
        if a[x][y]==2:
            h.append([x+1,y+1])
        elif a[x][y]==1:
            p.append([x+1,y+1])

for comb in combinations(h,m):
    s=0
    for px,py in p:
        dist = MAX
        for hx,hy in comb:
            d=abs(px-hx)+abs(py-hy)
            dist=min(dist,d)
        s+=dist
    ans=min(ans,s)

print(ans)