#include <cstdio>
#include <vector>
#include <memory.h>
using namespace std;

struct V {
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {}
};

const int MAXN = 51, MAXM = 51;
// 상, 좌, 하, 우
const int dr[] = { -1, 0, 1, 0 };
const int dc[] = { 0, -1, 0, 1 };

int n, m, q;
int start_idx[MAXN];
vector<int> brd[MAXN];
bool visited[MAXN][MAXM];

// O(1)
void rotate_circle(int tx, int d, int k) {
	if (d == 1) {
		start_idx[tx] = (start_idx[tx] + k) % m;
	}
	else {
		start_idx[tx] = (start_idx[tx] - k + m) % m;
	}
}

bool is_in_range(int r, int c) {
	if (r <= 0 || r > n || c < 0 || c >= m) return false;
	return true;
}

V brd_idx_to_original_idx(int r_brd, int c_brd) {
	if (start_idx[r_brd] <= c_brd) {
		return V(r_brd, c_brd - start_idx[r_brd]);
	}
	else {
		return V(r_brd, m - 1 - start_idx[r_brd] + c_brd + 1);
	}
}

V original_idx_to_brd_idx(int r_origin, int c_origin) {
	return V(r_origin, (start_idx[r_origin] + c_origin + m) % m);
}

// origin 위치 기준으로 순회
int dfs(int r_brd, int c_brd, int r_origin, int c_origin, int num) {
	if (brd[r_brd][c_brd] == 0 || visited[r_origin][c_origin]) return 0;
	brd[r_brd][c_brd] = 0;
	visited[r_origin][c_origin] = true;
	int ret = 1;
	
	for (int d = 0; d < 4; d++) {
		int nr_origin = r_origin + dr[d];
		int nc_origin = (c_origin + dc[d] + m) % m;
		if (!is_in_range(nr_origin, nc_origin)) continue;

		// (r_origin, c_origin)의 brd에서의 위치
		V brd_idx = original_idx_to_brd_idx(nr_origin, nc_origin);
		int nr_brd = brd_idx.r;
		int nc_brd = brd_idx.c;

		if (brd[nr_brd][nc_brd] == 0 || visited[nr_origin][nc_origin]) continue;
		if (brd[nr_brd][nc_brd] != num) continue;

		ret += dfs(nr_brd, nc_brd, nr_origin, nc_origin, num);
	}

	return ret;
}

void normalize() {
	int sum = 0;
	int cnt = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < m; j++) {
			sum += brd[i][j];
			if (brd[i][j] != 0) cnt++;
		}
	}

	if (cnt == 0) return;

	int avg = sum / cnt;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < m; j++) {
			if (brd[i][j] == 0) continue;

			if (brd[i][j] < avg) brd[i][j]++;
			else if (brd[i][j] > avg) brd[i][j]--;
		}
	}
}

int main() {
	scanf("%d %d %d", &n, &m, &q);
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < m; j++) {
			int x;
			scanf("%d", &x);
			brd[i].push_back(x);
		}
	}

	for (int i = 0, x, d, k; i < q; i++) {
		scanf("%d %d %d", &x, &d, &k);
		
		// x배수의 원판을 d방향으로 k칸 회전
		// d=0: 시계, d=1: 반시계
		for (int tx = x; tx <= n; tx += x) {
			// brd를 변화시키지 않고 시작 인덱스만 저장
			// O(1)
			rotate_circle(tx, d, k);
		}

		bool is_erased = false;	
		memset(visited, false, sizeof(visited));
		for (int r_brd = 1; r_brd <= n; r_brd++) {
			for (int c_brd = 0; c_brd < m; c_brd++) {
				// original_idx: brd[r_brd][c_brd]에 저장되어 있는 값의 원래 위치
				V original_idx = brd_idx_to_original_idx(r_brd, c_brd);
				int r_origin = original_idx.r;
				int c_origin = original_idx.c;

				if (brd[r_brd][c_brd] == 0 || visited[r_origin][c_origin]) continue;

				int target_num = brd[r_brd][c_brd];
				
				int cnt = dfs(r_brd, c_brd, r_origin, c_origin, target_num);
				if (cnt <= 1) {
					brd[r_brd][c_brd] = target_num;
				}
				else {
					is_erased = true;
				}
			}
		}

		if (!is_erased) {
			normalize();
		}
	}

	int sum = 0;
	for (int i = 1; i <= n; i++) {
		for (int j = 0; j < m; j++) {
			sum += brd[i][j];
		}
	}
	printf("%d", sum);
}