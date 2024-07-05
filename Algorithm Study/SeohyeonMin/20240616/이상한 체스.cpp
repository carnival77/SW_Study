#include <cstdio>
#include <vector>
#include <memory.h>
using namespace std;

const int MAXN = 8, MAXM = 8;
const int INF = 1e9;
// 북동남서
const int dr[] = { -1, 0, 1, 0};
const int dc[] = { 0, 1, 0, -1 };

struct ChessPiece {
	int r, c;
	int type;
	int d;	// direction
	ChessPiece() {}
	ChessPiece(int _r, int _c, int _t) :r(_r), c(_c), type(_t) {}
};

int n, m;
int brd[MAXN][MAXM];
int chk[MAXN][MAXM];
int ans = INF;

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= m);
}

void chk_forward(int sr, int sc, int d) {
	int r = sr, c = sc;
	
	chk[r][c] = true;

	int nr = r + dr[d];
	int nc = c + dc[d];
	while (is_in_range(nr, nc) && brd[nr][nc] != 6) {
		r = nr, c = nc;
		chk[r][c] = true;

		nr = r + dr[d];
		nc = c + dc[d];
	}
}

void chk_type1(ChessPiece cp) {
	// 앞방향
	chk_forward(cp.r, cp.c, cp.d);
}

void chk_type2(ChessPiece cp) {
	// 앞방향
	chk_forward(cp.r, cp.c, cp.d);

	// 뒷방향
	chk_forward(cp.r, cp.c, (cp.d+2)%4);
}

void chk_type3(ChessPiece cp) {
	// 앞방향
	chk_forward(cp.r, cp.c, cp.d);

	// 오른쪽 방향
	chk_forward(cp.r, cp.c, (cp.d+1)%4);
}

void chk_type4(ChessPiece cp) {
	// 앞방향
	chk_forward(cp.r, cp.c, cp.d);

	// 왼쪽 방향
	chk_forward(cp.r, cp.c, (cp.d-1+4)%4);

	// 오른쪽 방향
	chk_forward(cp.r, cp.c, (cp.d+1)%4);
}

void chk_type5(ChessPiece cp) {
	// 전방향
	for (int d = 0; d < 4; d++) {
		chk_forward(cp.r, cp.c, d);
	}
}


void dfs(int posi, vector<ChessPiece>& chessPieces) {
	if (posi == chessPieces.size()) {
		memset(chk, false, sizeof(chk));

		for (ChessPiece& cp: chessPieces) {
			if (cp.type == 1) {
				chk_type1(cp);
			}
			else if (cp.type == 2) {
				chk_type2(cp);
			}
			else if (cp.type == 3) {
				chk_type3(cp);
			}
			else if (cp.type == 4) {
				chk_type4(cp);
			}
			else if (cp.type == 5) {
				chk_type5(cp);
			}
		}

		int cnt = 0;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (chk[i][j] || brd[i][j]==6) cnt++;
			}
		}

		ans = min(ans, n* m - cnt);
		return;
	}

	for (int d = 0; d < 4; d++) {
		chessPieces[posi].d = d;
		dfs(posi + 1, chessPieces);
	}
}

int main() {
	scanf("%d %d", &n, &m);
	vector<ChessPiece> chessPieces;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			// 1~5: 내 말, 6: 상대말
			scanf("%d", &brd[i][j]);
			if (1 <= brd[i][j] && brd[i][j] <= 5) {
				chessPieces.push_back(ChessPiece(i, j, brd[i][j]));
			}
		}
	}

	// 내 말의 개수 최대 8개. 시간복잡도 = 4^8 * MAX(n, m)
	dfs(0, chessPieces);
	printf("%d", ans);
}