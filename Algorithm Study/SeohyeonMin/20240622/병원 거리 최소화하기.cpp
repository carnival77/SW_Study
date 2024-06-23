#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

struct V {
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {}
};

const int MAXN = 50;
const int MAXM = 13;
const int INF = 1e9;

int n, m;
int brd[MAXN][MAXN];
vector<V> ps;
vector<V> hs;

int pos[MAXM];
int ans = INF;
void dfs(int posi, int hi) {
	if (posi == m) {
		// 합 계산
		int sum = 0;
		for (int i = 0; i < ps.size(); i++) {
			int diff_min = INF;
			for (int j = 0; j < m; j++) {
				int diff = abs(ps[i].r - hs[pos[j]].r) + abs(ps[i].c - hs[pos[j]].c);
				diff_min = min(diff, diff_min);
			}
			sum += diff_min;
		}
		
		ans = min(ans, sum);
		return;
	}
	if (hi >= hs.size()) return;

	for (int i = posi; i < hs.size(); i++) {
		pos[posi] = i;
		dfs(posi + 1, i + 1);
	}
}

int main() {
	scanf("%d %d", &n, &m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			if (brd[i][j] == 1) ps.push_back(V(i, j));
			else if (brd[i][j] == 2) hs.push_back(V(i, j));
		}
	}

	dfs(0, 0);
	printf("%d", ans);
}