#include <cstdio>
#include <memory.h>
#include <algorithm>
using namespace std;

const int MAXN = 20 + 1;

int n, m, k;
// 0: 빈공간, 1: 사무실구역, 2~5: 에어컨 L, U, R, D 방향
int office[MAXN][MAXN];
// 현재 시원함 정도
int brd[MAXN][MAXN];
// 시원함 변화량
int diff_brd[MAXN][MAXN];
// my_wall[r][c][nr][nc]: (r, c) -> (nr, nc) 경로에 벽 존재
int my_wall[MAXN][MAXN][MAXN][MAXN];

// L U R D 순서
const int dr[] = { 0, -1, 0, 1 };
const int dc[] = { -1, 0, 1, 0 };

bool is_in_range(int r, int c) {
	return !(r <= 0 || r > n || c <= 0 || c > n);
}

bool visited[MAXN][MAXN];
// (r, c)에서 d 방향으로 에어컨 바람 전파
void dfs(int r, int c, int d, int amount) {
	if (amount == 0) return;
	visited[r][c] = true;
	diff_brd[r][c] += amount;

	// 직선 방향
	int nr = r + dr[d];
	int nc = c + dc[d];
	if (is_in_range(nr, nc) && !visited[nr][nc] 
		&& !my_wall[r][c][nr][nc]) {
		dfs(nr, nc, d, amount - 1);
	}

	// 위 대각선 방향
	int nr1 = r + dr[(d - 1 + 4) % 4];
	int nc1 = c + dc[(d - 1 + 4) % 4];
	int nr2 = nr1 + dr[d];
	int nc2 = nc1 + dc[d];
	if (is_in_range(nr2, nc2) && !visited[nr2][nc2] 
		&& !my_wall[r][c][nr1][nc1] && !my_wall[nr1][nc1][nr2][nc2]) {
		dfs(nr2, nc2, d, amount - 1);
	}

	// 아래 대각선 방향
	nr1 = r + dr[(d + 1) % 4];
	nc1 = c + dc[(d + 1) % 4];
	nr2 = nr1 + dr[d];
	nc2 = nc1 + dc[d];
	if (is_in_range(nr2, nc2) && !visited[nr2][nc2] 
		&& !my_wall[r][c][nr1][nc1] && !my_wall[nr1][nc1][nr2][nc2]) {
		dfs(nr2, nc2, d, amount - 1);
	}
}

void blow_winds() {
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			// 에어컨인 경우만 탐색
			if (office[r][c] <= 1) continue;
			int d = office[r][c] - 2;
			int sr = r + dr[d];
			int sc = c + dc[d];
			if (!is_in_range(sr, sc)) continue;

			memset(visited, 0, sizeof(visited));
			dfs(sr, sc, d, 5);
		}
	}
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			brd[r][c] += diff_brd[r][c];
			// 초기화
			diff_brd[r][c] = 0;
		}
	}
}

void diffuse() {
	// 시원함 확산
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			if (!brd[r][c]) continue;

			for (int d = 0; d < 4; d++) {
				int nr = r + dr[d];
				int nc = c + dc[d];
				if (!is_in_range(nr, nc)) continue;
				if (my_wall[r][c][nr][nc]) continue;
				if (brd[r][c] <= brd[nr][nc]) continue;
				// brd[r][c]가 높은 경우만 확산
				int diff = brd[r][c] - brd[nr][nc];
				diff_brd[nr][nc] += diff / 4;
				diff_brd[r][c] -= diff / 4;
			}
		}
	}
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			brd[r][c] += diff_brd[r][c];
			// 초기화
			diff_brd[r][c] = 0;
		}
	}
}

// 외벽
void minus_one() {
	for (int r = 1; r <= n; r++) {
		brd[r][1] = max(brd[r][1]-1, 0);
		brd[r][n] = max(brd[r][n] - 1, 0);
	}
	for (int c = 2; c < n; c++) {
		brd[1][c] = max(brd[1][c] - 1, 0);
		brd[n][c] = max(brd[n][c] - 1, 0);
	}
}

bool is_end() {
	bool ret = true;
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			if (office[r][c] != 1) continue;
			ret &= (brd[r][c] >= k);
		}
	}
	return ret;
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int r = 1; r <= n; r++) {
		for (int c = 1; c <= n; c++) {
			scanf("%d", &office[r][c]);
		}
	}

	for (int i = 0, x, y, s; i < m; i++) {
		// (x, y): 1 base
		scanf("%d %d %d", &x, &y, &s);
		// s=0: 위, s=1: 왼쪽
		int nx = x, ny = y;
		if (s == 0) {
			nx--;
		}
		else {
			ny--;
		}
		my_wall[x][y][nx][ny] = my_wall[nx][ny][x][y] = true;
	}

	int cnt = 0;
	while (cnt <= 100) {
		cnt++;
		blow_winds();
		diffuse();
		minus_one();
		if (is_end()) break;
	}
	printf("%d", cnt > 100 ? -1 : cnt);
}