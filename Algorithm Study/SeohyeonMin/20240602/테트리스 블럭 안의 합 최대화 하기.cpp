#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int MAXN = 2e2 + 1;

typedef struct V {
	int r, c;
	V() {}
	V(int _r, int _c) :r(_r), c(_c) {};
};

int n, m;
int brd[MAXN][MAXN];
vector<vector<V>> blocks;

V rotate90(V crd) {
	return V(-crd.c, crd.r);
}

V rotate180(V crd) {
	return V(-crd.r, -crd.c);
}

V rotate270(V crd) {
	return V(crd.c, -crd.r);
}

// y축 대칭
V reflect(V crd) {
	return V(-crd.r, crd.c);
}

vector<V> rotate90all(vector<V> block) {
	vector<V> ret;
	for (int i = 0; i < block.size(); i++) {
		ret.push_back(rotate90(block[i]));
	}
	return ret;
}

vector<V> rotate180all(vector<V> block) {
	vector<V> ret;
	for (int i = 0; i < block.size(); i++) {
		ret.push_back(rotate180(block[i]));
	}
	return ret;
}

vector<V> rotate270all(vector<V> block) {
	vector<V> ret;
	for (int i = 0; i < block.size(); i++) {
		ret.push_back(rotate270(block[i]));
	}
	return ret;
}

vector<V> reflectall(vector<V> block) {
	vector<V> ret;
	for (int i = 0; i < block.size(); i++) {
		ret.push_back(reflect(block[i]));
	}
	return ret;
}

int get_tetris_value(int r, int c, vector<V> block) {
	bool valid = true;
	int sum = 0;
	for (int b = 0; b < block.size(); b++) {
		int nr = r + block[b].r;
		int nc = c + block[b].c;
		if (nr < 0 || nc < 0 || nr >= n || nc >= m) {
			valid = false;
			break;
		}
		sum += brd[nr][nc];
	}
	return valid ? sum : 0;
}

int solve(vector<V>& block) {
	int ret = -1;

	// original
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, block));
		}
	}

	// rotate90
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate90all(block)));
		}
	}

	// rotate180
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate180all(block)));
		}
	}

	// rotate270
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate270all(block)));
		}
	}

	// reflect
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, reflectall(block)));
		}
	}

	// reflect & rotate90
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate90all(reflectall(block))));
		}
	}

	// reflect & rotate180
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate180all(reflectall(block))));
		}
	}

	// reflect & rotate270
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ret = max(ret, get_tetris_value(i, j, rotate270all(reflectall(block))));
		}
	}
	return ret;
}


int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	// ㅡ
	blocks.push_back(vector<V> {V(0, 0), V(0, 1), V(0, 2), V(0, 3)});
	// ㅁ자
	blocks.push_back(vector<V> {V(0, 0), V(1, 0), V(0, 1), V(1, 1)});
	// ㄴ자
	blocks.push_back(vector<V> {V(0, 0), V(1, 0), V(2, 0), V(2, 1)});
	// ㄹ자
	blocks.push_back(vector<V> {V(0, 0), V(1, 0), V(1, 1), V(2, 1)});
	// ㅏ
	blocks.push_back(vector<V> {V(0, 0), V(1, 0), V(2, 0), V(1, 1)});

	int ans = -1;
	for (vector<V> block : blocks) {
		int val = solve(block);
		ans = max(ans, val);
	}
	printf("%d", ans);
}
