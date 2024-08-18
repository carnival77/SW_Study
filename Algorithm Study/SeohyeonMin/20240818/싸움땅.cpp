#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int MAXN = 20;

// 상 우 하 좌  순서
const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, 1, 0, -1};

int n, m, k;
priority_queue<int> guns[MAXN][MAXN];
int player_map[MAXN][MAXN];

// Player
typedef struct P {
	int r, c, d;
	int num;
	int ability;
	int gun;
	int point;
	P() {}
	P(int _num, int _r, int _c, int _d, int _a) : num(_num), r(_r), c(_c), d(_d), ability(_a) {
		gun = 0;	// nothing
		point = 0;
	}
};

vector<P> ps;

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

void move_players() {
	for (int i = 1; i <= m; i++) {
		P& p = ps[i];

		int nr = p.r + dr[p.d];
		int nc = p.c + dc[p.d];
		if (!is_in_range(nr, nc)) {
			p.d = (p.d + 2) % 4;
			nr = p.r + dr[p.d];
			nc = p.c + dc[p.d];
		}
		player_map[p.r][p.c] = 0;
		p.r = nr, p.c = nc;

		// 이동할 칸에 아무도 없는 경우
		if (player_map[nr][nc] == 0) {
			// 총이 있는 경우
			if (!guns[nr][nc].empty()) {
				int cand = guns[nr][nc].top();
				// 쎈 총 획득
				if (cand > p.gun) {
					guns[nr][nc].pop();
					// 빈손이 아니라면 총 내려놓기
					if (p.gun != 0) {
						guns[nr][nc].push(p.gun);
					}
					// 총 획득
					p.gun = cand;
				}
			}
			
			player_map[p.r][p.c] = p.num;
		}
		// 플레이어와 싸우는 경우
		else {
			P& oponent = ps[player_map[nr][nc]];
			int my_stat = p.ability + p.gun;
			int op_stat = oponent.ability + oponent.gun;

			int winner_num = p.num, loser_num = oponent.num;
			if (my_stat < op_stat) {
				swap(winner_num, loser_num);
			}
			else if (my_stat == op_stat) {
				if (p.ability < oponent.ability) {
					swap(winner_num, loser_num);
				}
			}
			P& winner = ps[winner_num];
			P& loser = ps[loser_num];

			winner.point += (winner.ability + winner.gun) - (loser.ability + loser.gun);

			// 진 플레이어
			// 총이 있으면 총 내려놓기
			if (loser.gun != 0) {
				guns[nr][nc].push(loser.gun);
				loser.gun = 0;
			}
			// 이동
			int nr_loser = nr + dr[loser.d];
			int nc_loser = nc + dc[loser.d];

			// 사방이 막히는 경우는 없음. 누군가 움직여서 nr, nc로 왔기 때문
			// 무한 루프 X
			while (!is_in_range(nr_loser, nc_loser) || player_map[nr_loser][nc_loser]) {
				loser.d = (loser.d + 1) % 4;
				nr_loser = nr + dr[loser.d];
				nc_loser = nc + dc[loser.d];
			}

			loser.r = nr_loser, loser.c = nc_loser;
			// 진 플레이어 총 교체
			if (!guns[loser.r][loser.c].empty()) {
				int cand = guns[loser.r][loser.c].top();
				if (cand > loser.gun) {
					guns[loser.r][loser.c].pop();
					if (loser.gun != 0) {
						guns[loser.r][loser.c].push(loser.gun);
					}
					loser.gun = cand;
				}
			}

			// 이긴 플레이어 총 교체
			if (!guns[nr][nc].empty()) {
				int cand = guns[nr][nc].top();
				if (cand > winner.gun) {
					guns[nr][nc].pop();
					// 빈손이 아니면 내려놓기
					if (winner.gun != 0) {
						guns[nr][nc].push(winner.gun);
					}
					winner.gun = cand;
				}
			}

			// 플레이어 맵 정보 갱신
			player_map[winner.r][winner.c] = winner.num;
			player_map[loser.r][loser.c] = loser.num;
		}
	}
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int x;
			scanf("%d", &x);
			if (x != 0) {
				guns[i][j].push(x);
			}
		}
	}

	// dummy for 1 base
	ps.push_back(P(-1, -1, -1, -1, -1));
	for (int i = 1; i <= m; i++) {
		int x, y, d, s;
		scanf("%d %d %d %d", &x, &y, &d, &s);
		x--, y--;

		ps.push_back(P(i, x, y, d, s));
		player_map[x][y] = i;
	}

	for (int round = 0; round < k; round++) {
		move_players();
	}

	for (int i = 1; i <= m; i++) {
		printf("%d ", ps[i].point);
	}
}