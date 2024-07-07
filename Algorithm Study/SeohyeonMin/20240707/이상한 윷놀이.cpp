/*
* 이상한 윷놀이
* 시간복잡도: O(turn * k * k)
*/
#include <cstdio>
#include <vector>
using namespace std;

struct H {
	int x, y;
	int d;
	int floor;
	H() {}
	H(int _x, int _y, int _d, int _f) :x(_x), y(_y), d(_d), floor(_f) {}
};

const int MAXN = 12;
const int MAX_TURN = 1000;

const int RIGHT = 1, LEFT = 2, UP = 3, DOWN = 4;
const int WHITE = 0, RED = 1, BLUE = 2;

int n, k;
int brd[MAXN][MAXN];
vector<int> state[MAXN][MAXN];
vector<H> horses;

// 상 우 하 좌 순서
int dx[] = { -1, 0, 1, 0 };
int dy[] = { 0, 1, 0, -1 };

void update_crd(int x, int y) {
	for (int& hidx: state[x][y]) {
		horses[hidx].x = x;
		horses[hidx].y = y;
	}
}

bool is_in_range(int x, int y) {
	if (x < 0 || x >= n || y < 0 || y >= n) return false;
	return true;
}

bool do_move(int hidx) {
	H& h = horses[hidx];
	int nx = h.x + dx[h.d];
	int ny = h.y + dy[h.d];
	int nf = 0;

	if (!is_in_range(nx, ny) || brd[nx][ny] == BLUE) {
		// 방향 전환
		h.d = (h.d + 2) % 4;

		nx = h.x + dx[h.d];
		ny = h.y + dy[h.d];

		// 반대방향도 막힌 경우, 이동 종료
		if (!is_in_range(nx, ny) || brd[nx][ny] == BLUE) {
			return false;
		}
	}
	
	if (brd[nx][ny] == WHITE) {
		// 얹혀 있는 말 포함 이동
		int cur_f = h.floor;
		int sz = state[h.x][h.y].size();
		for (int f = cur_f; f < sz; f++) {
			horses[state[h.x][h.y][f]].floor = state[nx][ny].size();
			state[nx][ny].push_back(state[h.x][h.y][f]);
		}
		for (int f = cur_f; f < sz; f++) {
			state[h.x][h.y].pop_back();
		}
	}
	else if (brd[nx][ny] == RED) {
		// 얹혀 있는 말 포함 이동
		int cur_f = h.floor;
		int sz = state[h.x][h.y].size();
		for (int f = sz-1; f >= cur_f; f--) {
			horses[state[h.x][h.y][f]].floor = state[nx][ny].size();
			state[nx][ny].push_back(state[h.x][h.y][f]);
		}
		for (int f = cur_f; f < sz; f++) {
			state[h.x][h.y].pop_back();
		}
	}
	update_crd(nx, ny);
	
	return state[h.x][h.y].size() >= 4;
}

int main() {
	scanf("%d %d", &n, &k);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}
	for (int i = 0, x, y, d; i < k; i++) {
		scanf("%d %d %d", &x, &y, &d);
		x--, y--;
		if (d == RIGHT) d = 1;
		else if (d == LEFT) d = 3;
		else if (d == UP) d = 0;
		else if (d == DOWN) d = 2;

		horses.push_back(H(x, y, d, 0));
		state[x][y].push_back(horses.size() - 1);
	}

	int turn = 0;
	bool is_end = false;
	while (!is_end && turn < MAX_TURN) {
		turn++;

		// 이동
		for (int i = 0; i < horses.size() && !is_end; i++) {
			is_end |= do_move(i);
		}
	}
	printf("%d", turn >= MAX_TURN ? -1 : turn);
}
