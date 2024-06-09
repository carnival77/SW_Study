#include <cstdio>
#include <algorithm>
#include<vector>
using namespace std;
const int MAXN = 15;
const int MAXT = 5;
const int MAXP = 1000;

typedef struct V {
	int t, p;
	V() {};
	V(int _t, int _p) :t(_t), p(_p) {}
};

int dp[MAXN + 2];
// dp[i]: i번째 날까지 얻을 수 있는 최대 수익
int main() {
	int n;
	scanf("%d", &n);
	vector<V> works;
	
	for (int i = 0, t, p; i < n; i++) {
		scanf("%d %d", &t, &p);
		works.push_back(V(t, p));
	}
	
	for (int i = n - 1; i >=0; i--) {
		if (i + works[i].t <= n) {
			dp[i] = max(dp[i + 1], dp[i + works[i].t] + works[i].p);
		}
		else {
			dp[i] = dp[i + 1];
		}
		
	}
	printf("%d", dp[0]);
}