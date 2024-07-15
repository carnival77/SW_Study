#include <cstdio>
#include <queue>
#include <algorithm>
#include <memory.h>
using namespace std;

typedef long long ll;
struct Car {
	int r, c;
	ll battery;
	Car() {}
	Car(int _r, int _c, ll _b): r(_r), c(_c), battery(_b) {}
};

struct Customer {
	int r_st, c_st;
	int r_ed, c_ed;
	int num;
	bool is_arrive;
	Customer() {};
	Customer(int _rst, int _cst, int _red, int _ced, int _num, bool _f) : r_st(_rst), c_st(_cst), r_ed(_red), c_ed(_ced), num(_num), is_arrive(_f) {}
};

struct V {
	int r, c;
	V() {}
	V(int _r, int _c) : r(_r), c(_c) {}
};

const int MAXN = 20;
const int WALL = 1;
const int dr[] = { -1, 1, 0, 0 };
const int dc[] = { 0, 0, -1, 1 };

int n, m;
int brd[MAXN][MAXN];
Car car;
Customer customers[MAXN * MAXN + 1];

bool is_in_range(int r, int c) {
	if (r < 0 || r >= n || c < 0 || c >= n) return false;
	return true;
}


int dis[MAXN][MAXN] = {};
bool visited[MAXN][MAXN] = {};
int bfs(int r_st, int c_st, int r_target, int c_target) {
	queue<V> que;
	que.push(V(r_st, c_st));

	memset(visited, false, sizeof(visited));
	memset(dis, -1, sizeof(dis));
	dis[r_st][c_st] = 0;

	while (!que.empty()) {
		V cur = que.front();
		que.pop();

		if (visited[cur.r][cur.c]) continue;
		visited[cur.r][cur.c] = true;

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (!is_in_range(nr, nc)) continue;
			if (brd[nr][nc] == WALL) continue;
			if (visited[nr][nc]) continue;

			dis[nr][nc] = dis[cur.r][cur.c] + 1;
			que.push(V(nr, nc));
		}
	}
	return dis[r_target][c_target];
}

int find_customer() {
	queue<V> que;
	que.push(V(car.r, car.c));

	memset(visited, false, sizeof(visited));
	memset(dis, -1, sizeof(dis));
	dis[car.r][car.c] = 0;

	while (!que.empty()) {
		V cur = que.front();
		que.pop();

		if (visited[cur.r][cur.c]) continue;
		visited[cur.r][cur.c] = true;

		for (int d = 0; d < 4; d++) {
			int nr = cur.r + dr[d];
			int nc = cur.c + dc[d];
			if (!is_in_range(nr, nc)) continue;
			if (brd[nr][nc] == WALL) continue;
			if (visited[nr][nc]) continue;

			dis[nr][nc] = dis[cur.r][cur.c] + 1;
			que.push(V(nr, nc));
		}
	}

	vector<Customer> cands;
	for (int i = 1; i <= m; i++) {
		Customer c = customers[i];
		if (dis[c.r_st][c.c_st] == -1) continue;
		if (c.is_arrive) continue;

		cands.push_back(c);
	}
	if (cands.size() == 0) {
		return -1;
	}

	sort(cands.begin(), cands.end(),
		[&](Customer& c1, Customer& c2) {
			if (dis[c1.r_st][c1.c_st] == dis[c2.r_st][c2.c_st]) {
				if (c1.r_st == c2.r_st) return c1.c_st < c2.c_st;
				else return c1.r_st < c2.r_st;
			}
			else {
				return dis[c1.r_st][c1.c_st] < dis[c2.r_st][c2.c_st];
			}

		});

	return cands[0].num;
}

int main() {
	ll c;
	scanf("%d %d %lld", &n, &m, &c);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			// 0: 도로, 1: 벽
		}
	}
	int r_car, c_car;
	scanf("%d %d", &r_car, &c_car);
	r_car--, c_car--;
	car = Car(r_car, c_car, c);

	for (int i = 1; i <= m; i++) {
		int r_st, c_st, r_ed, c_ed;
		scanf("%d %d %d %d", &r_st, &c_st, &r_ed, &c_ed);
		r_st--, c_st--, r_ed--, c_ed--;
		customers[i] = Customer(r_st, c_st, r_ed, c_ed, i, false);
	}

	// 다음 고객 선택
	for (int i = 0; i < m; i++) {
		int c_next = find_customer();
		if (c_next == -1) {
			puts("-1");
			return 0;
		}
		Customer& c = customers[c_next];
		int dis1 = bfs(car.r, car.c, c.r_st, c.c_st);
		int dis2 = bfs(c.r_st, c.c_st, c.r_ed, c.c_ed);

		// 이동 불가
		if (dis1 == -1 || dis2 == -1) {
			puts("-1");
			return 0;
		}

		if (dis1 + dis2 <= car.battery) {
			car.battery -= (dis1 + dis2);
			car.battery += dis2 * 2ll;
			car.r = c.r_ed;
			car.c = c.c_ed;
			c.is_arrive = true;
		}
		// 연료 부족
		else {
			puts("-1");
			return 0;
		}
	}

	printf("%lld", car.battery);
}