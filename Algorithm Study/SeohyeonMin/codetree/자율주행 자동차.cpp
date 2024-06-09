#include <cstdio>

const int MAXN = 50, MAXM = 50;
// d : 0123 - 북동남서
const int dx[] = { -1, 0, 1, 0 };
const int dy[] = { 0, 1, 0, -1 };
int brd[MAXN][MAXM];
bool visited[MAXN][MAXM];

int left_curve(int cur_d) {
	return (cur_d - 1 + 4) % 4;
}
int right_curve(int cur_d) {
	return (cur_d + 1) % 4;
}

bool is_out(int x, int y, int n, int m) {
	return (x < 0 || y < 0 || x >= n || y >= m);
}

int main() {
	int n, m;
	scanf("%d %d", &n, &m);

	int sx, sy, sd;
	scanf("%d %d %d", &sx, &sy, &sd);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// 도로: 0, 인도: 1
			scanf("%d", &brd[i][j]);
		}
	}

	int cx = sx, cy = sy, cd = sd;
	visited[cx][cy] = true;

	int cnt = 0;
	while (true) {
		int nd = left_curve(cd);
		int nx = cx + dx[nd];
		int ny = cy + dy[nd];
		if (!is_out(nx, ny, n, m) && !brd[nx][ny] && !visited[nx][ny]) {			
			cx = nx, cy = ny, cd = nd;
			visited[cx][cy] = true;

			cnt = 0;
			continue;
		}

		cd = nd;
		cnt++;

		if (cnt == 4) {
			nx = cx - dx[nd];
			ny = cy - dy[nd];

			cx = nx, cy = ny, cd = nd;
			
			// 후진도 불가한 상황에 탈출
			if (is_out(cx, cy, n, m) || brd[cx][cy]) break;
			cnt = 0;
			continue;
		}
	}

	int sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			sum += visited[i][j];
		}
	}
	printf("%d", sum);
}