#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 20;
const int BRD_EMPTY = 0, BRD_WALL = -1;
const int dr[] = { -1, 0, 1, 0 };
const int dc[] = { 0, -1, 0, 1 };

int n, m, k, c;
int brd[MAXN][MAXN];	// 나무 양
int brd_diff[MAXN][MAXN];	// 나무 변화량
int is_unavailable[MAXN][MAXN];	// 제초제 남은 연수

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

void grow_tree() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (brd[i][j] == BRD_EMPTY || brd[i][j] == BRD_WALL) continue;

			int cnt = 0;
			for (int d = 0; d < 4; d++) {
				int nr = i + dr[d];
				int nc = j + dc[d];
				if (!is_in_range(nr, nc)) continue;
				if (brd[nr][nc] >= 1) {
					cnt++;
				}
			}
			brd_diff[i][j] += cnt;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] += brd_diff[i][j];
			// 초기화
			brd_diff[i][j] = 0;
		}
	}
}

void propagate_tree() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (brd[i][j] == BRD_EMPTY || brd[i][j] == BRD_WALL) continue;

			int cnt = 0;
			for (int d = 0; d < 4; d++) {
				int nr = i + dr[d];
				int nc = j + dc[d];
				if (!is_in_range(nr, nc)) continue;
				// 벽
				if (brd[nr][nc] == BRD_WALL) continue;
				// 다른 나무
				if (brd[nr][nc] >= 1) continue;
				// 제초제
				if (is_unavailable[nr][nc]) continue;
				cnt++;
			}

			if (cnt == 0) continue;

			int pcnt = brd[i][j] / cnt;
			for (int d = 0; d < 4; d++) {
				int nr = i + dr[d];
				int nc = j + dc[d];
				if (!is_in_range(nr, nc)) continue;
				// 벽
				if (brd[nr][nc] == BRD_WALL) continue;
				// 다른 나무
				if (brd[nr][nc] >= 1) continue;
				// 제초제
				if (is_unavailable[nr][nc]) continue;
				brd_diff[nr][nc] += pcnt;
			}
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] += brd_diff[i][j];
			// 초기화
			brd_diff[i][j] = 0;
		}
	}
}


const int dr_diagonal[] = { -1, -1, 1, 1 };
const int dc_diagonal[] = { -1, 1, 1, -1 };
int erase_tree() {
	// 제초제 잔여기한 -1년
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (is_unavailable[i][j]) is_unavailable[i][j]--;
		}
	}

	int sum_mx = 0;
	int r_mx = -1, c_mx = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			// 나무가 있는 칸에서 시작해야 함
			if (brd[i][j] < 1) continue;

			int sum = brd[i][j];
			for (int d = 0; d < 4; d++) {
				for (int step = 1; step <= k; step++) {
					int nr = i + dr_diagonal[d] * step;
					int nc = j + dc_diagonal[d] * step;
					if (!is_in_range(nr, nc)) continue;
					if (brd[nr][nc] <= 0) break;
					sum += brd[nr][nc];
				}
			}

			if (sum > sum_mx) {
				sum_mx = sum;
				r_mx = i, c_mx = j;
			}
		}
	}

	// 제초제 투여
	is_unavailable[r_mx][c_mx] = c;
	brd[r_mx][c_mx] = BRD_EMPTY;
	for (int d = 0; d < 4; d++) {
		for (int step = 1; step <= k; step++) {
			int nr = r_mx + dr_diagonal[d] * step;
			int nc = c_mx + dc_diagonal[d] * step;
			if (!is_in_range(nr, nc)) continue;
			if (brd[nr][nc] <= 0) {
				is_unavailable[nr][nc] = c;
				break;
			}
			is_unavailable[nr][nc] = c;
			brd[nr][nc] = BRD_EMPTY;
		}
	}

	return sum_mx;
}


int main() {
	scanf("%d %d %d %d", &n, &m, &k, &c);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int sum = 0;
	for (int year = 0; year < m; year++) {
		grow_tree();

		propagate_tree();

		sum += erase_tree();
	}
	printf("%d", sum);
}