#include <cstdio>
#include <memory.h>

const int MAXN = 50, MAXM = 50;

struct Crd {
	int r, c;
	Crd() {}
	Crd(int _r, int _c) :r(_r), c(_c) {}
};

struct Typoon {
	Crd upper = Crd(-1, -1);

	Typoon() {};
	Typoon(Crd _upper) : upper(_upper) {}
};

int n, m;
int t;
int brd[MAXN][MAXM];
int brd_tmp[MAXN][MAXM];
int dirt_diff[MAXN][MAXM];
Typoon typoon;

// O(1)
void spread(int r, int c) {
	const int dr[] = { 0, -1, 0, 1 };
	const int dc[] = { 1, 0, -1, 0 };

	int available_direction = 0;
	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
		if (brd[nr][nc] == -1) continue;
		available_direction++;
	}

	int amount = brd[r][c] / 5;
	dirt_diff[r][c] -= amount*available_direction;
	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
		if (brd[nr][nc] == -1) continue;
		dirt_diff[nr][nc] += amount;
	}
}

// O(n*m)
void dirt_spread() {
	memset(dirt_diff, 0, sizeof(dirt_diff));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// 돌풍 자리면 패스
			if (brd[i][j] == -1) continue;

			// 확산
			spread(i, j);
		}
	}

	// 확산된 먼지 갱신
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			brd[i][j] += dirt_diff[i][j];
		}
	}
}

// O(n + m)
void clean_upper(Crd upper) {
	// 오른쪽, 위, 왼쪽, 아래 순
	const int dr[] = { 0, -1, 0, 1 };
	const int dc[] = { 1, 0, -1, 0 };

	int r_cur = upper.r, c_cur = upper.c;
	int d_cur = 0;
	int remain = 2 * (upper.r+1) + 2 * m - 4 - 1;

	int dirt_tmp = 0;
	while (remain > 0) {
		int nr = r_cur + dr[d_cur];
		int nc = c_cur + dc[d_cur];
		if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
			nr = r_cur - dr[d_cur];
			nc = c_cur - dc[d_cur];
			d_cur++;
			
			nr = r_cur + dr[d_cur];
			nc = c_cur + dc[d_cur];
		}
		int dirt_next = brd[nr][nc];
		brd[nr][nc] = dirt_tmp;
		
		dirt_tmp = dirt_next;
		r_cur = nr, c_cur = nc;
		remain--;
	}
}

// O(n + m)
void clean_lower(Crd lower) {
	// 오른쪽, 아래, 왼쪽, 위 순
	const int dr[] = { 0, 1, 0, -1 };
	const int dc[] = { 1, 0, -1, 0 };

	int r_cur = lower.r, c_cur = lower.c;
	int d_cur = 0;
	int remain = 2 * (n - lower.r) + 2 * m - 4 - 1;

	int dirt_tmp = 0;
	while (remain > 0) {
		int nr = r_cur + dr[d_cur];
		int nc = c_cur + dc[d_cur];
		if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
			nr = r_cur - dr[d_cur];
			nc = c_cur - dc[d_cur];
			d_cur++;

			nr = r_cur + dr[d_cur];
			nc = c_cur + dc[d_cur];
		}
		int dirt_next = brd[nr][nc];
		brd[nr][nc] = dirt_tmp;

		dirt_tmp = dirt_next;
		r_cur = nr, c_cur = nc;
		remain--;
	}
}

// O(n + m)
void do_clean() {
	Crd upper = typoon.upper;
	Crd lower = Crd(typoon.upper.r + 1, 0);

	clean_upper(upper);
	clean_lower(lower);
}


int main() {
	scanf("%d %d %d", &n, &m, &t);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%d", &brd[i][j]);
			if (brd[i][j] == -1 && typoon.upper.r == -1) {
				typoon = Typoon(Crd(i, j));
			}
		}
	}

	for (int i = 0; i < t; i++) {
		// 먼지 확산
		dirt_spread();

		// 청소
		do_clean();
	}

	int sum = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (brd[i][j] == -1) continue;
			sum += brd[i][j];
		}
	}
	printf("%d", sum);
}