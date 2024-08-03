#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 15;
const int MAXM = 100;
// 오른쪽 시작
const int dr[] = { 0 ,-1, -1, -1, 0, 1, 1, 1 };
const int dc[] = { 1, 1, 0, -1, -1, -1, 0, 1 };

// vertex
struct V {
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {}
};

// diff
struct D {
	int r, c, add;
	D() {}
	D(int _r, int _c, int _add) :r(_r), c(_c), add(_add) {}
};

int n, m;
int brd[MAXN][MAXN];

vector<V> specials;
bool special_map[MAXN][MAXN];

void move_special(int d, int p) {
	for (V& s : specials) {
		int nr = (s.r + dr[d] * p % n + n) % n;
		int nc = (s.c + dc[d] * p % n + n) % n;
		special_map[s.r][s.c] = false;
		s.r = nr;
		s.c = nc;
	}

	for (V& s : specials) {
		special_map[s.r][s.c] = true;
	}
}

void eat_special() {
	for (V& s : specials) {
		brd[s.r][s.c]++;
	}
}

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

void additional_growth() {
	vector<D> diff;
	for (V& s : specials) {
		int cnt_add = 0;
		for (int d = 1; d < 8; d += 2) {
			int nr = s.r + dr[d];
			int nc = s.c + dc[d];

			if (!is_in_range(nr, nc)) continue;
			if (brd[nr][nc] >= 1) {
				cnt_add++;
			}
		}
		diff.push_back(D(s.r, s.c, cnt_add));
	}

	// diff 적용
	for (D& d : diff) {
		brd[d.r][d.c] += d.add;
	}
}

void put_special() {
	vector<V> new_specials;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (special_map[i][j]) continue;

			if (brd[i][j] >= 2) {
				brd[i][j] -= 2;
				new_specials.push_back(V(i, j));
			}
		}
	}

	// 기존 영양제 제거
	for (V& s : specials) {
		special_map[s.r][s.c] = false;
	}
	specials = new_specials;
	for (V& s : specials) {
		special_map[s.r][s.c] = true;
	}
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}
	// 초기 특수영양제 (좌하단 4개칸)
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			special_map[n - 1 - i][j] = true;
			specials.push_back(V(n - 1 - i, j));
		}
	}


	for (int i = 0, d, p; i < m; i++) {
		scanf("%d %d", &d, &p);
		d--;

		// 특수 영양제 이동 (d, p)
		move_special(d, p);
		
		// 이동시킨 특수영양제 투입
		eat_special();

		// 추가 성장(대각선으로 인접한 방향 높이1인 리브로수 개수 만큼)
		additional_growth();

		// 특수영양제 투입된 곳 제외 높이 2이상인 곳 특수 영양제 올려두기
		put_special();
	}

	int ans = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ans += brd[i][j];
		}
	}
	printf("%d", ans);
}