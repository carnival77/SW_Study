#include <cstdio>
#include <algorithm>
#include <queue>
#include <memory.h>
using namespace std;

struct Crd {
	int r, c;
	Crd() {}
	Crd(int _r, int _c) : r(_r), c(_c) {}
};

struct Robot {
	Crd crd;
	int level = 2;
	int cnt = 0;
	Robot() {}
	Robot(Crd _crd) : crd(_crd) {}
};

struct V {
	int r, c;
	int dis;
	V() {}
	V(int _r, int _c, int _dis) : r(_r), c(_c), dis(_dis) {}
};

const int MAXN = 20;
const int INF = 1e9;
const int dr[] = { -1, 0, 1, 0 };
const int dc[] = { 0, -1, 0, 1 };

int n;
int brd[MAXN][MAXN];
bool visited[MAXN][MAXN];

Crd choose_crd_by_rule(Crd crd_monster1, Crd crd_monster2) {
	Crd list_monster[] = { crd_monster1, crd_monster2 };
	// 우선순위: 가장 위쪽, 가장 왼쪽 순
	sort(list_monster, list_monster + 2, [](const Crd& c1, const Crd& c2) -> bool {
		return c1.r == c2.r ? c1.c < c2.c : c1.r < c2.r;
		});
	return list_monster[0];
}

V find_closest_monster(Robot& robot) {
	Crd target = Crd(-1, -1);
	int dis_min = INF;
	memset(visited, false, sizeof(visited));

	queue<V> que;
	que.push(V(robot.crd.r, robot.crd.c, 0));

	while (!que.empty()) {
		V cur = que.front();
		que.pop();

		if (visited[cur.r][cur.c]) continue;
		visited[cur.r][cur.c] = true;

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
			if (visited[nr][nc]) continue;
			if (brd[nr][nc] > robot.level) continue;

			Crd crd_next = Crd(nr, nc);
			int dis_next = cur.dis + 1;
			que.push(V(nr, nc, cur.dis + 1));

			if (brd[nr][nc] != 0 && brd[nr][nc] < robot.level) {
				if (dis_next < dis_min) {
					target = crd_next;
					dis_min = cur.dis + 1;
				}
				else if (dis_next == dis_min) {
					target = choose_crd_by_rule(target, crd_next);
				}
			}
		}
	}

	return V(target.r, target.c, dis_min);
}

int solve(Robot& robot) {
	int ret = 0;

	while (true) {
		V monster = find_closest_monster(robot);
		// 없앨 수 있는 몬스터 없음
		if (monster.r == -1) break;

		// 몬스터 위치로 이동
		robot.crd = Crd(monster.r, monster.c);
		ret += monster.dis;

		// 몬스터 제거
		brd[monster.r][monster.c] = 0;

		// 경험치 상승
		robot.cnt++;
		if (robot.cnt == robot.level) {
			robot.level++;
			robot.cnt = 0;
		}
	}

	return ret;
}


int main() {
	Robot robot;

	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			if (brd[i][j] == 9) {
				robot = Robot(Crd(i, j));
				brd[i][j] = 0;
			}
		}
	}

	int ans = solve(robot);
	printf("%d", ans);
}