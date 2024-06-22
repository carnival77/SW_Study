#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 20;
const int INF = 1e9;

int n;	// n은 짝수
int P[MAXN][MAXN];
bool pos[MAXN];

int ans = INF;

void dfs(int pi, int wi) {
	if (pi == n / 2) {
		vector<int> morning, evening;
		for (int i = 0; i < n; i++) {
			if (pos[i]) morning.push_back(i);
			else evening.push_back(i);
		}

		int sum1 = 0, sum2 = 0;
		for (int i = 0; i < n/2; i++) {
			for (int j = i+1; j < n/2; j++) {
				sum1 += P[morning[i]][morning[j]] + P[morning[j]][morning[i]];
				sum2 += P[evening[i]][evening[j]] + P[evening[j]][evening[i]];
			}
		}
		ans = min(ans, abs(sum2 - sum1));
		return;
	}
	if (wi >= n) return;

	for (int i = wi; i < n; i++) {
		pos[i] = true;
		dfs(pi + 1, i + 1);
		pos[i] = false;
	}
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &P[i][j]);
		}
	}

	dfs(0, 0);
	printf("%d", ans);
}