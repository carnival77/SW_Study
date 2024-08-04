#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 20;
const int MAXM = 1000;

// 우 하 좌 상 순서
// F L F' L'
const int dr[] = { 0, 1, 0, -1 };
const int dc[] = { 1, 0, -1, 0 };

// dice
struct D {
	int r, c;
	// U R D L F B
	vector<int> num = { 1, 3, 6, 4, 2, 5 };
	// direction
	int d = 0;
	D(){}
	D(int _r, int _c, int _d): r(_r), c(_c), d(_d) {}

	// F 
	void roll_front_clockwise() {
		num = { num[3], num[0], num[1], num[2], num[4], num[5] };
	}
	// L
	void roll_left_clockwise() {
		num = { num[5], num[1], num[4], num[3], num[0], num[2] };
	}
	// F'
	void roll_front_counterclockwise() {
		num = { num[1], num[2], num[3], num[0], num[4], num[5] };
	}
	// L'
	void roll_left_counterclockwise() {
		num = { num[4], num[1], num[5], num[3], num[2], num[0] };
	}
};

int n, m;
int brd[MAXN][MAXN];

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

int vsum[MAXN * MAXN];
int visited[MAXN][MAXN];
int vcnt;
int dfs(int r, int c, int target) {
	int ret = target;
	visited[r][c] = vcnt;

	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc)) continue;
		if (visited[nr][nc]) continue;
		if (brd[nr][nc] != target) continue;
		ret += dfs(nr, nc, target);
	}
	return ret;
}

int roll_dice(D& dice) {
	// 구를 방향, 좌표 결정
	int nd = dice.d;
	int nr = dice.r + dr[nd];
	int nc = dice.c + dc[nd];
	if (!is_in_range(nr, nc)) {
		nd = (nd + 2) % 4;
		nr = dice.r + dr[nd];
		nc = dice.c + dc[nd];
	}

	// 주사위 nd 방향으로 굴리기
	if (nd == 0) {
		dice.roll_front_clockwise();
	}
	else if (nd == 1) {
		dice.roll_left_clockwise();
	}
	else if (nd == 2) {
		dice.roll_front_counterclockwise();
	}
	else {
		dice.roll_left_counterclockwise();
	}

	// 다음 이동 방향 지정
	int x = dice.num[2];
	if (x > brd[nr][nc]) {
		dice.d = (nd + 1) % 4;
	}
	else if (x < brd[nr][nc]) {
		dice.d = (nd - 1 + 4) % 4;
	}
	else {
		dice.d = nd;
	}
	dice.r = nr;
	dice.c = nc;
	
	return vsum[visited[nr][nc]];
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	// 전처리
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (visited[i][j]) continue;
			vcnt++;
			int sum = dfs(i, j, brd[i][j]);
			vsum[vcnt] = sum;
		}
	}
	
	D dice = D(0, 0, 0);
	int score = 0;
	while (m--) {
		score += roll_dice(dice);
	}
	printf("%d", score);
}