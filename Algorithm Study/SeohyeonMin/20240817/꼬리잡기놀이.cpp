#include <cstdio>
#include <memory.h>
#include <vector>
using namespace std;

const int MAXN = 20;
const int dr[] = { 0, -1, 0, 1 };
const int dc[] = { 1, 0, -1, 0 };

enum Direction {
	RIGHT = 0,
	UP,
	LEFT,
	DOWN
};

// 좌표
typedef struct V {
	int r, c;
	V() {}
	V(int _r, int _c): r(_r), c(_c) {}
};

// Team
typedef struct T {
	vector<V> rail;
	int head;	// rail내 머리의 인덱스
	int direction;	// 머리 -> 꼬리 방향 -1 or 1
	int cnt;	// 사람 수

	T() {}
	T(vector<V> r, int h, int d, int c) {
		rail = r;
		head = h;
		direction = d;
		cnt = c;
	}

	int calc_index(int from, int step) {
		return (from + direction * step + rail.size()) % rail.size();
	}
};

int n, m, k;
int brd[MAXN][MAXN];

int tcnt;	// 팀 수
int tnum[MAXN][MAXN];	// (r, c) 칸의 팀 번호
vector<T> teams;	// 팀 정보

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

void dfs(int r, int c, int& num, vector<V>& rail) {
	if (tnum[r][c]) return;
	tnum[r][c] = num;
	rail.push_back(V(r, c));

	for (int d = 0; d < 4; d++) {
		int nr = r + dr[d];
		int nc = c + dc[d];
		if (!is_in_range(nr, nc)) continue;
		if (!brd[nr][nc]) continue;
		dfs(nr, nc, num, rail);
	}
}

void update_brd_by_team(T& t) {
	for (int i = 0; i < t.rail.size(); i++) {
		brd[t.rail[i].r][t.rail[i].c] = -1;
	}

	for (int i = 0; i < t.cnt; i++) {
		int ci = t.calc_index(t.head, i);
		brd[t.rail[ci].r][t.rail[ci].c] = i;
	}
}

void move_one() {
	for (int i = 1; i <= tcnt; i++) {
		T& t = teams[i];
		t.head = t.calc_index(t.head, -1);
		update_brd_by_team(t);
	}
}

int shout_ball(int round) {
	int d_ball = round % (4 * n) / n;
	// 시작 위치 결정
	int sr = -1, sc = -1;
	if (d_ball == RIGHT) {
		sr = round % n;
		sc = 0;
	}
	else if (d_ball == UP) {
		sr = n - 1;
		sc = round % n;

	}
	else if (d_ball == LEFT) {
		sr = n - 1 - round % n;
		sc = n - 1;
	}
	else if (d_ball == DOWN) {
		sr = 0;
		sc = n - 1 - round % n;
	}

	// 공 받을 사람 탐색
	int score = 0;
	int tr = -1, tc = -1;
	for (int step = 0; step < n; step++) {
		int nr = sr + dr[d_ball] * step;
		int nc = sc + dc[d_ball] * step;
		if (!tnum[nr][nc]) continue;

		if (brd[nr][nc] != -1) {
			tr = nr, tc = nc;
			score = (brd[tr][tc] + 1) * (brd[tr][tc] + 1);
			break;
		}
	}

	// 공을 받은 사람이 없는 경우
	if (tr == -1 && tc == -1) return 0;

	// 공 받은 경우
	// 머리 변경
	T& t = teams[tnum[tr][tc]];
	t.head = t.calc_index(t.head, t.cnt-1);

	// 방향 변경
	t.direction *= -1;

	// brd 업데이트
	update_brd_by_team(t);

	return score;
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	// dummy for 1 base
	teams.push_back(T(vector<V>{}, 0, 1, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tnum[i][j]) continue;
			if (brd[i][j] == 0) continue;

			tcnt++;
			// 이동선
			vector<V> rail;
			dfs(i, j, tcnt, rail);

			int head = -1;
			int direction = 0;
			int cnt = 0;
			for (int i = 0; i < rail.size(); i++) {
				if (brd[rail[i].r][rail[i].c] == 1) {
					head = i;

					// 방향
					int left = (i - 1 + rail.size()) % rail.size();
					int right = (i + 1) % rail.size();
					int num_left = brd[rail[left].r][rail[left].c];
					int num_right = brd[rail[right].r][rail[right].c];
					// 꽉찬 경우
					if (num_left != 4 && num_right != 4) {
						if (num_left == 3) direction = 1;
						else direction = -1;
					}
					else if (num_left != 4) {
						direction = -1;
					}
					else {
						direction = 1;
					}
				}

				if (brd[rail[i].r][rail[i].c] != 4) {
					cnt++;
				}
			}

			// brd 변환. 
			// 사람 없는 칸은 -1, 
			// 나머지는 머리부터 순서대로 0, 1, 2, ..., size-1
			for (int i = 0; i < rail.size(); i++) {
				brd[rail[i].r][rail[i].c] = -1;
			}

			for (int step = 0; step < cnt; step++) {
				int ci = (head + direction * step + rail.size()) % rail.size();
				brd[rail[ci].r][rail[ci].c] = step;
			}

			teams.push_back(T(rail, head, direction, cnt));
		}
	}

	int score = 0;
	for (int round = 0; round < k; round++) {
		// 한 칸 이동
		move_one();

		int x = shout_ball(round);
		score += x;
	}
	printf("%d", score);
}