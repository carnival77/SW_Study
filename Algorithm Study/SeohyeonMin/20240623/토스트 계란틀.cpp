#include <cstdio>
#include <memory.h>
#include <algorithm>
#include <queue>
using namespace std;

struct V{
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {}
};

const int MAXN = 50;
const int dr[] = { -1, 0, 1, 0 };
const int dc[] = { 0, -1, 0, 1 };

int n;
int L, R;
int brd[MAXN][MAXN];
int chk[MAXN][MAXN];
int chknum;
int chksum[MAXN * MAXN];
int chkcnt[MAXN * MAXN];

void bfs(int sr, int sc) {
	chknum++;

	queue<V> que;
	que.push(V(sr, sc));
	while (!que.empty()) {
		V cur = que.front();
		que.pop();

		if (chk[cur.r][cur.c]) continue;
		chk[cur.r][cur.c] = chknum;
		chksum[chknum] += brd[cur.r][cur.c];
		chkcnt[chknum]++;

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
			if (chk[nr][nc]) continue;
			
			int diff = abs(brd[cur.r][cur.c] - brd[nr][nc]);
			if (!(L <= diff && diff <= R)) continue;

			que.push(V(nr, nc));
		}
	}
}

// 2000번 이하

void init_chk() {
	memset(chk, false, sizeof(chk));
	chknum = 0;
	memset(chksum, 0, sizeof(chksum));
	memset(chkcnt, 0, sizeof(chkcnt));
}

bool arrange_board() {
	bool is_changed = false;
	init_chk();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (chk[i][j]) continue;
			bfs(i, j);
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (chkcnt[chk[i][j]] > 1) {
				brd[i][j] = chksum[chk[i][j]] / chkcnt[chk[i][j]];
				is_changed = true;
			}
		}
	}
	
	return is_changed;
}

int main() {
	scanf("%d", &n);
	scanf("%d %d", &L, &R);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int cnt = 0;
	while (arrange_board()) {
		cnt++;
	}
	printf("%d", cnt);
}