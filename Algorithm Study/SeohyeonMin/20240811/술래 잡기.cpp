#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

enum Direction {
	DOWN = 0,
	RIGHT,
	UP,
	LEFT
};

// 좌표
struct V {
	int r, c;
	V() {}
	V(int _r, int _c) : r(_r), c(_c) {}
};

// 도망자
struct T {
	int r, c;
	int d;
	T() {}
	T(int _r, int _c, int _d) : r(_r), c(_c), d(_d) {}
};

const int MAXN = 99 + 1;
// 하 우 상 좌 순
const int dr[] = { 1, 0, -1, 0 };
const int dc[] = { 0, 1, 0, -1 };

int n, m, h, k;
bool is_tree[MAXN][MAXN];
// 도망자 정보
vector<T> thieves;

// snail_idx[r][c]: (r, c)위치가 snail 배열의 몇번째 인덱스 인지
int snail_idx[MAXN][MAXN];
// snail: (n/2, n/2) -> (0, 0) 달팽이 순서로 저장
// ex) snail[0] = V(n/2, n/2), snail[1] = V(n/2 - 1, n/2), ...
vector<V> snail;

// 술래 정보 (snail 기반 위치 정보)
int police_idx;	// snail 인덱스
int police_add;	// +1, -1
enum Direction police_d;	// 하우상좌

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

void make_snail() {
	vector<V> snail_reverse;
	int r = 0, c = 0, d = 0;
	int remain = n * n - 1;
	snail_reverse.push_back(V(r, c));
	snail_idx[r][c] = remain;

	while (remain > 0) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc) || snail_idx[nr][nc]) {
			d = (d + 1) % 4;
			nr = r + dr[d];
			nc = c + dc[d];
		}

		r = nr, c = nc;
		snail_reverse.push_back(V(r, c));
		remain--;
		snail_idx[r][c] = remain;
	}
	reverse(snail_reverse.begin(), snail_reverse.end());
	snail = snail_reverse;
}

bool is_there_police(int r, int c) {
	int pr = snail[police_idx].r;
	int pc = snail[police_idx].c;
	return (pr == r && pc == c);
}

void move_thief() {
	int pr = snail[police_idx].r;
	int pc = snail[police_idx].c;
	for (T& t : thieves) {
		// 맨하탄 거리
		int dis = abs(t.r - pr) + abs(t.c - pc);
		if (dis > 3) continue;

		int nr = t.r + dr[t.d];
		int nc = t.c + dc[t.d];
		// 격자를 벗어나는 경우
		if (!is_in_range(nr, nc)) {
			t.d = (t.d + 2) % 4;
			nr = t.r + dr[t.d];
			nc = t.c + dc[t.d];
		}

		// 술래가 있는 경우
		if (is_there_police(nr, nc)) continue;

		t.r = nr, t.c = nc;
	}
}

void move_police() {
	int next_police_idx = police_idx + police_add;
	if (next_police_idx < 0 || next_police_idx >= snail.size()) {
		police_add *= -1;
		next_police_idx = police_idx + police_add;
	}

	police_idx = next_police_idx;
	// 술래가 보는 방향 결정
	next_police_idx = police_idx + police_add;
	if (next_police_idx < 0 || next_police_idx >= snail.size()) {
		next_police_idx = police_idx + police_add * -1;
	}
	int r = snail[police_idx].r;
	int c = snail[police_idx].c;

	int nr = snail[next_police_idx].r;
	int nc = snail[next_police_idx].c;

	if (nr == r) {
		// 오른쪽 이동
		if (nc > c) {
			police_d = RIGHT;
		}
		else {
			police_d = LEFT;
		}
	}
	else {
		// 아래 이동
		if (nr > r) {
			police_d = DOWN;
		}
		else {
			police_d = UP;
		}
	}
}

// 잡은 도망자 수 반환
int catch_thieves() {
	int pr = snail[police_idx].r;
	int pc = snail[police_idx].c;
	int pd = police_d;

	int cnt = 0;
	for (int step = 0; step < 3; step++) {
		int nr = pr + dr[pd] * step;
		int nc = pc + dc[pd] * step;
		if (!is_in_range(nr, nc)) break;
		if (is_tree[nr][nc]) continue;

		// 잡힌 도망자 제거
		vector<T> new_thieves;
		for (T& t : thieves) {
			if (t.r == nr && t.c == nc) {
				cnt++;
				continue;
			}
			new_thieves.push_back(t);
		}
		thieves = new_thieves;
	}

	return cnt;
}

int main() {
	scanf("%d %d %d %d", &n, &m, &h, &k);

	for (int i = 0, x, y, d; i < m; i++) {
		scanf("%d %d %d", &x, &y, &d);
		// 0 base
		x--, y--;

		// d=1: 좌우(우 시작), d=2: 상하(하 시작)
		if (d == 1) {
			thieves.push_back(T(x, y, RIGHT));
		}
		else {
			thieves.push_back(T(x, y, DOWN));
		}
	}

	for (int i = 0, x, y; i < h; i++) {
		scanf("%d %d", &x, &y);
		// 0 base
		x--, y--;

		is_tree[x][y] = true;
	}

	// 달팽이 배열 초기화
	make_snail();

	// 경찰 위치 초기화
	police_idx = 0;
	police_add = 1;
	police_d = UP;

	int ans = 0;
	for (int turn = 1; turn <= k; turn++) {
		// 도망자 이동
		move_thief();

		// 술래 이동
		move_police();

		// 도망자 잡기
		ans += turn * catch_thieves();
	}
	printf("%d", ans);
}