from itertools import permutations,combinations

n=int(input())
a=[list(map(int,input().split())) for _ in range(n)]
ans=int(1e9)

array=[i for i in range(1,n+1)]

arr=[]
arr1=[]
arr2=[]
for comb in combinations(array,n//2):
    arr.append(comb)
arr1=arr[:len(arr)//2]
arr2=arr[len(arr)//2:]
arr2=arr2[::-1]

for a1,a2 in zip(arr1,arr2):
    s1,s2=0,0
    for x1,y1 in permutations(a1,2):
        s1+=a[x1-1][y1-1]
    for x2,y2 in permutations(a2,2):
        s2+=a[x2-1][y2-1]
    ans=min(ans,abs(s1-s2))

print(ans)