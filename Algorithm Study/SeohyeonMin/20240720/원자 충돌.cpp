#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 50;

// Atom
struct A {
	int w, v, d;
	A() {}
	A(int _w, int _v, int _d) : w(_w), v(_v), d(_d) {}
};

int n, m, k;
int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dc[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
vector<A> brd[MAXN][MAXN];

// O(n^2 * d)
void proceed() {
	// 1. 모든 원자 이동
	// O(max(n^2, m)) = O(n^2)
	vector<A> new_brd[MAXN][MAXN];

	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (brd[r][c].size()) {
				for (A& atom : brd[r][c]) {
					int nr = (r + dr[atom.d] * atom.v % n + n) % n;
					int nc = (c + dc[atom.d] * atom.v % n + n) % n;
					new_brd[nr][nc].push_back(atom);
				}
			}
		}
	}

	// brd 초기화
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			brd[r][c].clear();
		}
	}


	// 2. 합성
	// O(n^2 * d)
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (new_brd[r][c].size() < 2) {
				// 1칸에 1개만 있는 경우 그대로 보존
				for (A& atom : new_brd[r][c]) {
					brd[r][c].push_back(atom);
				}
				continue;
			}

			int wsum = 0;
			int vsum = 0;
			int cnt = new_brd[r][c].size();
			bool udlr = false, diagonal = false;
			for (A& atom : new_brd[r][c]) {
				wsum += atom.w;
				vsum += atom.v;
				if (atom.d & 1) diagonal = true;
				else udlr = true;
			}

			int nw = wsum / 5;
			// 질량이 0인 경우 소멸
			if (nw == 0) {
				new_brd[r][c].clear();
				continue;
			}
			int nv = vsum / cnt;

			// 대각선으로 퍼지는 경우
			if (udlr && diagonal) {
				for (int nd = 1; nd < 8; nd += 2) {
					brd[r][c].push_back(A(nw, nv, nd));
				}
			}
			// 상하좌우로 퍼지는 경우
			else {
				for (int nd = 0; nd < 8; nd += 2) {
					brd[r][c].push_back(A(nw, nv, nd));
				}
			}
		}
	}
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0, x, y, w, v, d; i < m; i++) {
		scanf("%d %d %d %d %d", &x, &y, &w, &v, &d);
		x--, y--;	// 0 base
		brd[x][y].push_back(A(w, v, d));
	}

	// O(k * n^2 * d)
	while (k--) {
		proceed();
	}

	int wsum = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			for (A& atom : brd[r][c]) {
				wsum += atom.w;
			}
		}
	}
	printf("%d", wsum);
}