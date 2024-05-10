import sys
from copy import deepcopy
from itertools import combinations

input=sys.stdin.readline
MAX=int(1e9)

n,m,h=map(int,input().split())

a=[[0]*(n+1) for _ in range(h+2)]

ans=MAX

for _ in range(m):
    x,y=map(int,input().split())
    a[x][y]=1
    a[x][y+1]=2

# 가로선 넣을 자리 후보 뽑기
cand=[]
for x in range(1,h+1):
    for y in range(1,n):
        # 해당 칸이나 오른쪽 칸에 1 또는 2가 있을 경우는 제외
        if 1<=a[x][y]<=2 or 1<=a[x][y+1]<=2:
            continue
        cand.append([x,y])

# i열이 i가 나오는 지 검사
def check(a):
    result=True

    for y in range(1,n+1):
        col=y
        # 아래로 내려가며 탐색
        for x in range(1,h+1):
            # 1인 경우 오른쪽
            if a[x][y]==1:
                y+=1
            # 2인 경우 왼쪽
            elif a[x][y]==2:
                y-=1
        if y!=col:
            result=False
            break

    return result

# 사다리 추가 안 해도 될 시 0 출력
if check(a):
    print(0)
    sys.exit(0)

# 후보들 중 1~3개를 뽑는 모든 조합의 경우의 수 탐색
for num in range(1,4):
    for comb in combinations(cand,num):
        # 이번 조합의 칸들 채우기
        for x,y in comb:
            a[x][y]=1
            a[x][y+1]=2
        # i열이 i가 나오면 num 출력
        if check(a):
            print(num)
            sys.exit(0)
        # 이번 조합 칸들 비우기
        for x, y in comb:
            a[x][y]=0
            a[x][y+1]=0

print(-1)