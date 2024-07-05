#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// vertex
struct V {
	int x, y;
	V() {}
	V(int _x, int _y) :x(_x), y(_y) {}
};

bool custom_equel_to(V v1, V v2) {
	return v1.x == v2.x && v1.y == v2.y;
}

struct DragonCurve {
	int sx, sy, d, g;
	vector<V> vs;
	DragonCurve() {}
	DragonCurve(int _sx, int _sy, int _d, int _g, vector<V> _vs) : sx(_sx), sy(_sy), d(_d), g(_g), vs(_vs) {}
};

const int MAXN = 20;
const int MAXX = 100, MAXY = 100;
// 오른쪽, 위쪽, 왼쪽, 아래쪽 순
const int dx[] = { 0, -1, 0, 1 };
const int dy[] = { 1, 0, -1, 0 };

int n;
int brd[MAXX+2][MAXY+2];

V rotate90(V v) {
	/*
	* 회전 변환
	* ( c -s ) ( x )
	* ( s c  ) ( y )
	* x축 양의 방향 기준 -90도
	* ( 0  1 ) ( x )
	* ( -1 0 ) ( y )
	*/
	
	return V(v.y, -v.x);
}


DragonCurve draw_dragon_curve(int sx, int sy, int d, int g) {
	vector<V> vs;

	// 0차 시작
	vs.push_back(V(sx, sy));
	vs.push_back(V(sx + dx[d], sy + dy[d]));

	V v_far = vs[1];
	// i차 드래곤 커브 그리기
	for (int gi = 1; gi <= g; gi++) {
		vector<V> new_vs;

		V new_v_far;
		// 가장 먼점 기준으로 모든 점 -90도 회전
		for (V& v : vs) {
			new_vs.push_back(v);

			V rotated = rotate90(V(v.x - v_far.x, v.y - v_far.y));
			rotated.x += v_far.x;
			rotated.y += v_far.y;
			new_vs.push_back(rotated);

			if (v.x == sx && v.y == sy) new_v_far = rotated;
		}

		sort(new_vs.begin(), new_vs.end(), [](const V& v1, const V& v2) -> bool {
			return v1.x == v2.x ? v1.y < v2.y : v1.x < v2.x;
		});
		new_vs.erase(unique(new_vs.begin(), new_vs.end(), custom_equel_to), new_vs.end());

		vs = new_vs;
		v_far = new_v_far;
	}

	return DragonCurve(sx, sy, d, g, vs);
}


int main() {
	scanf("%d", &n);

	vector<DragonCurve> dcs;
	for (int i = 0, sx, sy, d, g; i < n; i++) {
		scanf("%d %d %d %d", &sx, &sy, &d, &g);
		dcs.push_back(draw_dragon_curve(sx, sy, d, g));
	}

	
	for (DragonCurve& dragon_curve : dcs) {
		for (V& v : dragon_curve.vs) {
			if (v.x < 0 || v.x > MAXX || v.y < 0 || v.y > MAXY) continue;
			brd[v.x][v.y] = 1;
		}
	}
	
	int cnt = 0;
	for (int i = 0; i < MAXX; i++) {
		for (int j = 0; j < MAXY; j++) {
			if (brd[i][j] && brd[i + 1][j] && brd[i][j + 1] && brd[i + 1][j + 1]) {
				cnt++;
			}
		}
	}
	printf("%d", cnt);
}