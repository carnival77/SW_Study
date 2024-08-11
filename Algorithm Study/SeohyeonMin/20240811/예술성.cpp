#include <cstdio>
#include <memory.h>
#include <queue>
#include <vector>
using namespace std;

const int MAXN = 29 + 1;

const int dr[] = { -1, 1, 0, 0 };
const int dc[] = { 0, 0, -1, 1 };

struct V {
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {}
};

int n;
int brd[MAXN][MAXN];

// bfs
int gcnt;
int gnum[MAXN][MAXN];
int gsize[MAXN * MAXN];
int gvalue[MAXN * MAXN];

// dfs
bool visited_vertex[MAXN][MAXN];
bool visited_edge[MAXN][MAXN][MAXN][MAXN];
int adjacent_cnt[MAXN * MAXN][MAXN * MAXN];

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

// O(n^2)
void dfs(int r, int c) {
	if (visited_vertex[r][c]) return;
	visited_vertex[r][c] = true;

	int g_cur = gnum[r][c];
	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc)) continue;

		int n_cur = gnum[nr][nc];
		if (visited_vertex[nr][nc]) {
			if (visited_edge[r][c][nr][nc]) continue;

			if (g_cur != n_cur) {
				adjacent_cnt[g_cur][n_cur]++;
				adjacent_cnt[n_cur][g_cur]++;
			}
			visited_edge[r][c][nr][nc] = visited_edge[nr][nc][r][c] = true;
			continue;
		}

		if (g_cur != n_cur) {
			adjacent_cnt[g_cur][n_cur]++;
			adjacent_cnt[n_cur][g_cur]++;
		}
		visited_edge[r][c][nr][nc] = visited_edge[nr][nc][r][c] = true;
		dfs(nr, nc);
	}
}

// O(n^4), visited_edge 초기화 때문
int calc_score() {
	// 그룹 넘버링
	gcnt = 0;
	memset(gnum, 0, sizeof(gnum));
	memset(gsize, 0, sizeof(gsize));
	memset(gvalue, 0, sizeof(gvalue));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (gnum[i][j]) continue;

			gcnt++;
			gvalue[gcnt] = brd[i][j];

			queue<V> que;
			que.push(V(i, j));
			while (!que.empty()) {
				V cur = que.front();
				que.pop();
				if (gnum[cur.r][cur.c]) continue;
				gnum[cur.r][cur.c] = gcnt;
				gsize[gcnt]++;

				for (int d = 0; d < 4; d++) {
					int nr = cur.r + dr[d];
					int nc = cur.c + dc[d];
					if (!is_in_range(nr, nc)) continue;
					if (gnum[nr][nc]) continue;
					if (brd[nr][nc] != gvalue[gcnt]) continue;

					que.push(V(nr, nc));
				}
			}
		}
	}

	// 인접한 면 개수 계산
	memset(visited_vertex, false, sizeof(visited_vertex));
	memset(visited_edge, false, sizeof(visited_edge));
	dfs(0, 0);

	// 예술 점수 계산
	int score = 0;
	for (int a = 1; a < gcnt; a++) {
		for (int b = a + 1; b <= gcnt; b++) {
			int x = (gsize[a] + gsize[b]) * gvalue[a] * gvalue[b] * adjacent_cnt[a][b];
			score += x;

			// 초기화
			adjacent_cnt[a][b] = adjacent_cnt[b][a] = 0;
		}
	}
	return score;
}

// O(n)
void rotate_cross_line_counterclockwise() {
	// copy
	// 세로가 될 영역
	int r, c;
	vector<int> vertical_tmp;
	r = n / 2;
	for (c = n - 1; c >= 0; c--) {
		vertical_tmp.push_back(brd[r][c]);
	}

	// 가로가 될 영역
	vector<int> horizontal_tmp;
	c = n / 2;
	for (r = 0; r < n; r++) {
		horizontal_tmp.push_back(brd[r][c]);
	}

	// paste
	// 세로
	c = n / 2;
	r = 0;
	for (int i = 0; i < n; i++, r++) {
		brd[r][c] = vertical_tmp[i];
	}

	// 가로
	r = n / 2;
	c = 0;
	for (int i = 0; i < n; i++, c++) {
		brd[r][c] = horizontal_tmp[i];
	}
}

// O(n^2)
void rotate_clockwise(int sr, int sc, int sz) {
	vector<vector<int>> tmp(sz, vector<int>(sz, 0));

	for (int nr = 0, c = sc; nr < sz; nr++, c++) {
		for (int nc = 0, r = sr + sz - 1; nc < sz; nc++, r--) {
			tmp[nr][nc] = brd[r][c];
		}
	}
	for (int r = sr, nr = 0; r < sr + sz; r++, nr++) {
		for (int c = sc, nc = 0; c < sc + sz; c++, nc++) {
			brd[r][c] = tmp[nr][nc];
		}
	}
}

// O(n^4), calc_score
int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int score = 0;
	// 예술점수 계산
	score += calc_score();
	//printf("init: %d\n", score);

	for (int turn = 1; turn <= 3; turn++) {
		// 십자 영역 반시계 방향 회전
		rotate_cross_line_counterclockwise();

		// 나머지 영역 시계 방향 회전
		rotate_clockwise(0, 0, n / 2);
		rotate_clockwise(n / 2 + 1, 0, n / 2);
		rotate_clockwise(0, n / 2 + 1, n / 2);
		rotate_clockwise(n / 2 + 1, n / 2 + 1, n / 2);

		// 예술점수 계산
		score += calc_score();
	}

	printf("%d", score);
}