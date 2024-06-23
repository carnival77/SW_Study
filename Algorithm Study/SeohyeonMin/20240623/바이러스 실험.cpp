#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

struct Virus {
	int r, c;
	int age;
	bool alive;
	Virus(int _r, int _c, int _age, bool _alive) : r(_r), c(_c), age(_age), alive(_alive) {}
};

const int MAXN = 10;
const int dr[] = {-1, -1, -1, 0, 1, 1, 1, 0};
const int dc[] = {-1, 0, 1, 1, 1, 0, -1, -1};

int n, m, k;
int brd[MAXN][MAXN];
int add_amount[MAXN][MAXN];	// 추가되는 양분의 양

vector<Virus> do_cycle(vector<Virus>& vs) {
	vector<Virus> new_vs;

	// age순 오름차순 정렬
	sort(vs.begin(), vs.end(),
		[](const Virus& x, const Virus& y) -> bool {
			return x.age < y.age;
		});
	
	// 양분 섭취
	for (Virus& v : vs) {
		if (brd[v.r][v.c] >= v.age) {
			brd[v.r][v.c] -= v.age;
			v.age++;
			new_vs.push_back(v);
		}
		else {
			// 아사
			v.alive = false;
		}
	}
	
	// 죽은 바이러스는 양분으로
	for (Virus& v : vs) {
		if (v.alive) continue;
		brd[v.r][v.c] += v.age / 2;
	}

	// 번식 진행
	for (Virus& v : vs) {
		if (!v.alive) continue;
		if (v.age % 5 != 0) continue;

		// 살아있는 바이러스 중에 나이가 5의 배수인 경우, 번식
		for (int d = 0; d < 8; d++) {
			int nr = v.r + dr[d];
			int nc = v.c + dc[d];
			if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
			new_vs.push_back(Virus(nr, nc, 1, true));
		}
	}

	// 양분 추가
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] += add_amount[i][j];
		}
	}
	return new_vs;
}

int main() {
	scanf("%d %d %d", &n, &m, &k);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			brd[i][j] = 5;
			scanf("%d", &add_amount[i][j]);
		}
	}

	vector<Virus> vs;
	for (int i = 0, r, c, age; i < m; i++) {
		scanf("%d %d %d", &r, &c, &age);
		r--, c--;
		vs.push_back(Virus(r, c, age, true));
	}
	
	while (k--) {
		vs = do_cycle(vs);
	}
	printf("%d", vs.size());
}