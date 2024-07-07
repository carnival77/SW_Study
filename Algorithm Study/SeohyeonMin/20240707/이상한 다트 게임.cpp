#include <cstdio>
#include <vector>
#include <memory.h>
using namespace std;

const int MAXN = 51, MAXM = 51;
int n, m, q;
vector<int> brd[MAXN];

void rotate_circle(int tx, int d, int k) {
	vector<int> new_brd;
	// 반시계
	if (d == 1) {
		for (int i = 0; i < m; i++) {
			new_brd.push_back(brd[tx][(i + k) % m]);
		}
	}
	else {
		for (int i = 0; i < m; i++) {
			new_brd.push_back(brd[tx][(i - k + m) % m]);
		}
	}
	brd[tx] = new_brd;
}


bool is_in_range(int r, int c) {
	if (r <= 0 || r > n || c < 0 || c >= m) return false;
	return true;
}

int dr[] = { -1, 0, 1, 0 };
int dc[] = { 0, -1, 0, 1 };
bool visited[MAXN][MAXM];
int dfs(int cr, int cc, int num) {
	if (brd[cr][cc] == 0 || visited[cr][cc]) return 0;
	brd[cr][cc] = 0;
	visited[cr][cc] = true;
	int ret = 1;
	
	for (int d = 0; d < 4; d++) {
		int nr = cr + dr[d];
		int nc = (cc + dc[d]+m)%m;
		if (!is_in_range(nr, nc)) continue;
		if (brd[nr][nc] == 0 || visited[nr][nc]) continue;
		if (brd[nr][nc] != num) continue;

		ret += dfs(nr, nc, num);
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
			rotate_circle(tx, d, k);
		}

		bool is_erased = false;	
		memset(visited, false, sizeof(visited));
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j < m; j++) {
				if (brd[i][j] == 0 || visited[i][j]) continue;

				int target_num = brd[i][j];
				int cnt = dfs(i, j, target_num);
				if (cnt <= 1) {
					brd[i][j] = target_num;
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