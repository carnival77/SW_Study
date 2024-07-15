#include<cstdio>
#include <algorithm>
#include <vector>
using namespace std;

// State
struct S {
	int pnum, t;	// 소유자, 계약한 턴
	S() {}
	S(int _n, int _t) : pnum(_n), t(_t) {}
};

// player
struct P {
	int r, c;
	int d;	// 보고 있는 방향
	int num;	// 본인 번호
	vector<int> priority_rule[4];

	P() {}
	P(int _r, int _c, int _num) :r(_r), c(_c), num(_num) {}
};

struct V {
	int r, c;
	int d;	// 방향
	V() {}
	V(int _r, int _c, int _d) :r(_r), c(_c), d(_d) {}
};

const int MAXN = 20;
const int UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3;
const int EMPTY_STATE = 0;
// 상 하 좌 우 순
const int dr[] = { -1, 1, 0, 0 };
const int dc[] = { 0, 0, -1, 1 };

int n, m, k;
S state_brd[MAXN][MAXN];	// brd의 상태 저장 공간
P players[MAXN * MAXN + 2];

bool is_in_range(int r, int c) {
	if (r < 0 || r >= n || c < 0 || c >= n) return false;
	return true;
}

V select_next_vertex_by_rule(P& player, vector<V>& cands) {
	// 바라보고 있는 방향 기준으로, 이동할 방향 선택
	vector<int> rule = player.priority_rule[player.d];
	// 룰 기준 내림차순 정렬
	sort(cands.begin(), cands.end(),
		[&](V& v1, V& v2) {
			for (int i = 0; i < 4; i++) {
				if (rule[i] == v1.d) return true;
				else if (rule[i] == v2.d) return false;
			}
		}
	);
	return cands[0];
}

V calc_next_vertex(P& player, int turn) {
	// 이동 우선순위
	// priority1=1순위: 독점계약이 없는 칸
	// priority2=2순위: 본인이 독점계약한 칸
	// 같은 우선순위의 경우 각자의 방향 우선순위에 따라 결정
	vector<V> priority1, priority2;
	for (int d = 0; d < 4; d++) {
		int nr = player.r + dr[d];
		int nc = player.c + dc[d];
		if (!is_in_range(nr, nc)) continue;

		
		if (state_brd[nr][nc].pnum == EMPTY_STATE
			|| turn - state_brd[nr][nc].t > k) {
			state_brd[nr][nc] = S(0, 0);
			priority1.push_back(V(nr, nc, d));
		}
		else if (state_brd[nr][nc].pnum == player.num) {
			priority2.push_back(V(nr, nc, d));
		}
	}

	V v_next;
	if (priority1.size() > 0) {
		v_next = select_next_vertex_by_rule(player, priority1);
	}
	else {
		v_next = select_next_vertex_by_rule(player, priority2);
	}
	return v_next;
}

struct Tmp {
	int pnum, d;
	Tmp() {}
	Tmp(int _n, int _d) : pnum(_n), d(_d) {}
};

bool proceed(int turn) {
	vector<Tmp> tmp_brd[MAXN][MAXN];

	// 모든 플레이어 이동
	for (int pi = 1; pi <= m; pi++) {
		// 제거된 플레이어
		if (players[pi].num == -1) continue;
		V v_next = calc_next_vertex(players[pi], turn);

		tmp_brd[v_next.r][v_next.c].push_back(Tmp(pi, v_next.d));
	}

	// 플레이어 이동 및 겹친 플레이어 제거
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (tmp_brd[i][j].size() >= 1) {
				// 겹친 플레이어 제거
				for (int k = 1; k < tmp_brd[i][j].size(); k++) {
					int pnum_remove = tmp_brd[i][j][k].pnum;
					players[pnum_remove].num = -1;
				}

				// 살아남은 플레이어 이동
				int pnum_owner = tmp_brd[i][j][0].pnum;
				players[pnum_owner].r = i;
				players[pnum_owner].c = j;
				players[pnum_owner].d = tmp_brd[i][j][0].d;
				state_brd[i][j].pnum = pnum_owner;
				state_brd[i][j].t = turn;
			}
		}
	}

	bool is_end = true;
	for (int i = 2; i <= m; i++) {
		if (players[i].num != -1) {
			is_end = false;
			break;
		}
	}
	return is_end;
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0, pnum; j < n; j++) {
			scanf("%d", &pnum);
			// 0: 빈칸
			// n(n!=0): 플레이어 번호
			if (pnum != 0) {
				players[pnum] = P(i, j, pnum);
				// brd[i][j] 플레이어가, 0턴에 독점 계약
				state_brd[i][j] = S(pnum, 0);
			}
			else {
				state_brd[i][j] = S(0, 0);
			}
		}
	}
	for (int i = 1, d; i <= m; i++) {
		scanf("%d", &d);
		d--;	// 0 base
		players[i].d = d;
	}

	// 플레이어 별, 보고 있는 방향 별, 이동할 방향 우선순위 입력
	for (int pi = 1; pi <= m; pi++) {
		for (int d = 0; d < 4; d++) {
			for (int i = 0, d_priority; i < 4; i++) {
				scanf("%d", &d_priority);
				d_priority--;
				players[pi].priority_rule[d].push_back(d_priority);
			}
		}
	}

	bool is_end = false;
	for (int turn = 1; turn < 1000; turn++) {
		is_end = proceed(turn);
		if (is_end) {
			printf("%d\n", turn);
			return 0;
		}
	}

	puts("-1");
}