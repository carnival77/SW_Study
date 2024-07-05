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

int main() {
	scanf("%d %d", &n, &m);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			if (brd[i][j] == 1) ps.push_back(V(i, j));
			else if (brd[i][j] == 2) hs.push_back(V(i, j));
		}
	}

	int ans = INF;
	for (int state = 1; state < (1 << hs.size()); state++) {
		vector<V> hs_selected;
		for (int i = 0; i < hs.size(); i++) {
			if ((state >> i) & 1) {
				hs_selected.push_back(hs[i]);
			}
		}
		
		if (hs_selected.size() != m) continue;
		int sum = 0;
		for (V p : ps) {
			int diff_min = INF;
			for (V h : hs_selected) {
				int diff = abs(p.r - h.r) + abs(p.c - h.c);
				diff_min = min(diff_min, diff);
			}
			sum += diff_min;
		}

		ans = min(ans, sum);
	}
	
	printf("%d", ans);
}