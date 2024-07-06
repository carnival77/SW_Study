/*
* 바이러스 백신
* 시간복잡도: 최악에 10C5 * n^2
*/
#include <cstdio>
#include <vector>
#include <queue>
#include <memory.h>
#include <algorithm>
using namespace std;

struct H {
	int r, c;
	H() {}
	H(int _r, int _c) :r(_r), c(_c) {}
};

const int INF = 1e9;
const int MAXN = 50;
const int MAXM = 10;

const int VIRUS = 0;
const int WALL = 1;
const int HOSPITAL = 2;

const int dr[] = { -1, 1, 0, 0 };
const int dc[] = { 0, 0, -1, 1 };

int n, m;
int brd[MAXN][MAXN];
int dis[MAXN][MAXN];
vector<H> hospitals;

int ans = INF;

bool is_in_range(int r, int c) {
	if (r < 0 || r >= n || c < 0 || c >= n) return false;
	return true;
}

int bfs(vector<int>& selected) {
	memset(dis, -1, sizeof(dis));

	queue<H> que;
	for (int i = 0; i < m; i++) {
		H h = hospitals[selected[i]];
		que.push(h);
		dis[h.r][h.c] = 0;
	}

	while (!que.empty()) {
		H cur = que.front();
		que.pop();

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (!is_in_range(nr, nc)) continue;
			if (brd[nr][nc] == WALL) continue;
			if (dis[nr][nc] != -1) continue;

			que.push(H(nr, nc));
			dis[nr][nc] = dis[cur.r][cur.c] + 1;
		}
	}

	int mx = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (brd[i][j] == VIRUS) {
				if (dis[i][j] == -1)
					return INF;
				mx = max(mx, dis[i][j]);
			}
		}
	}

	return mx;
}

void dfs(int si, int hi, vector<int>& selected) {
	if (si == m) {
		int x = bfs(selected);
		ans = min(ans, x);
		return;
	}

	for (int i = hi; i < hospitals.size(); i++) {
		selected.push_back(i);
		dfs(si + 1, i + 1, selected);
		selected.pop_back();
	}
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			// 병원
			if (brd[i][j] == HOSPITAL) {
				hospitals.push_back(H(i, j));
			}
		}
	}
	vector<int> selected;
	dfs(0, 0, selected);

	printf("%d", ans == INF ? -1 : ans);
}