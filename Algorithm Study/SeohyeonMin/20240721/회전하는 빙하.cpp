#include <cstdio>
#include <vector>
#include <queue>
using namespace std;

const int MAXN = 6;

struct V {
	int r, c;
	V() {}
	V(int _r, int _c) : r(_r), c(_c) {}
};
int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, -1, 0, 1 };

int n, q;
int pow2[MAXN + 1];

void update_mini_brd(int r, int c, int len, vector<vector<int>>& mini_brd, vector<vector<int>>& brd) {
	// (r, c)부터 len x len 만큼을 mini_brd로 업데이트 하라는 뜻
	for (int i = 0; i < len; i++) {
		for (int j = 0; j < len; j++) {
			brd[r + i][c + j] = mini_brd[i][j];
		}
	}
}

void dnc(int r, int c, int len, int lv_target, vector<vector<int>>& brd) {
	// base condition
	if (len == pow2[lv_target]) {
		vector<vector<int>> tmp(len, vector<int>(len, 0));
		vector<vector<int>> mini_brd(len/2, vector<int>(len/2, 0));

		for (int i = 0; i < len; i++) {
			for (int j = 0; j < len; j++) {
				tmp[i][j] = brd[r + i][c + j];
			}
		}

		// 좌측 상단 -> 우측 상단
		for (int i = 0; i < len/2; i++) {
			for (int j = 0; j < len/2; j++) {
				mini_brd[i][j] = tmp[i][j];
			}
		}
		update_mini_brd(r, c + len / 2, len / 2, mini_brd, brd);

		// 우측 상단 -> 우측 하단
		for (int i = 0; i < len / 2; i++) {
			for (int j = 0; j < len / 2; j++) {
				mini_brd[i][j] = tmp[i][len / 2 + j];
			}
		}
		update_mini_brd(r + len/2, c + len/2, len / 2, mini_brd, brd);

		// 우측 하단 -> 좌측 하단
		for (int i = 0; i < len / 2; i++) {
			for (int j = 0; j < len / 2; j++) {
				mini_brd[i][j] = tmp[len / 2 + i][len / 2 + j];
			}
		}
		update_mini_brd(r + len / 2, c, len / 2, mini_brd, brd);

		// 좌측 하단 -> 좌측 상단
		for (int i = 0; i < len / 2; i++) {
			for (int j = 0; j < len / 2; j++) {
				mini_brd[i][j] = tmp[len/2 + i][j];
			}
		}
		update_mini_brd(r, c, len / 2, mini_brd, brd);
		return;
	}

	int len_next = len / 2;
	dnc(r, c, len_next, lv_target, brd);
	dnc(r + len_next, c, len_next, lv_target, brd);
	dnc(r, c + len_next, len_next, lv_target, brd);
	dnc(r + len_next, c + len_next, len_next, lv_target, brd);
}

// O(2^n * 2^n * min(n, lv))
void rotate(vector<vector<int>>& brd, int lv) {
	int len = pow2[n];
	dnc(0, 0, len, lv, brd);
}


bool is_in_range(int r, int c) {
	return !(r < 0 || r >= pow2[n] || c < 0 || c >= pow2[n]);
}

// O(2^n * 2^n)
void melting(vector<vector<int>>& brd) {
	vector<vector<int>> diff(brd.size(), vector<int>(brd.size(), 0));
	for (int i = 0; i < brd.size(); i++) {
		for (int j = 0; j < brd.size(); j++) {
			if (brd[i][j] == 0) continue;

			int cnt = 0;
			for (int d = 0; d < 4; d++) {
				int nr = i + dr[d];
				int nc = j + dc[d];
				if (!is_in_range(nr, nc)) continue;
				if (brd[nr][nc] != 0) cnt++;
			}

			if (cnt < 3) {
				diff[i][j]--;
			}
		}
	}

	for (int i = 0; i < brd.size(); i++) {
		for (int j = 0; j < brd.size(); j++) {
			brd[i][j] += diff[i][j];
		}
	}
}

int calc_remain(vector<vector<int>>& brd) {
	int remain = 0;
	for (int i = 0; i < pow2[n]; i++) {
		for (int j = 0; j < pow2[n]; j++) {
			remain += brd[i][j];
		}
	}
	return remain;
}

// O(2^n * 2^n * d)
// d: 이동 가능한 방향 개수
int calc_max_group_size(vector<vector<int>>& brd) {
	vector<vector<int>> g(brd.size(), vector<int>(brd[0].size(), 0));
	int gnum = 0;
	vector<int> gsz(pow2[n] * pow2[n], 0);

	queue<V> que;
	for (int i = 0; i < pow2[n]; i++) {
		for (int j = 0; j < pow2[n]; j++) {
			// 빙하가 존재하지 않으면 패스
			if (brd[i][j] == 0) continue;
			// 이미 그룹 번호가 부여된 경우 패스
			if (g[i][j] != 0) continue;

			gnum++;
			que.push(V(i, j));
			while (!que.empty()) {
				V cur = que.front();
				que.pop();
				if (g[cur.r][cur.c] != 0) continue;
				g[cur.r][cur.c] = gnum;
				gsz[gnum]++;

				for (int d = 0; d < 4; d++) {
					int nr = cur.r + dr[d];
					int nc = cur.c + dc[d];
					if (!is_in_range(nr, nc)) continue;
					if (brd[nr][nc] == 0) continue;
					if (g[nr][nc] != 0) continue;

					que.push(V(nr, nc));
				}
			}
		}
	}

	int mx = 0;
	for (int i = 1; i <= gnum; i++) {
		mx = max(mx, gsz[i]);
	}
	return mx;
}


int main() {
	pow2[0] = 1;
	for (int i = 1; i <= MAXN; i++) {
		pow2[i] = pow2[i - 1] * 2;
	}

	scanf("%d %d", &n, &q);
	vector<vector<int>> brd(pow2[n], vector<int>(pow2[n], 0));
	for (int i = 0; i < pow2[n]; i++) {
		for (int j = 0; j < pow2[n]; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	// O(q * 2^n * 2^n * n)
	int lv;
	while (q--) {
		scanf("%d", &lv);
		// 회전
		rotate(brd, lv);

		// 녹는 시간
		melting(brd);
	}

	// O(2^n * 2^n)
	int remain = calc_remain(brd);

	// O(2^n * 2^n * d)
	int max_group_size = calc_max_group_size(brd);
	printf("%d\n%d", remain, max_group_size);
}