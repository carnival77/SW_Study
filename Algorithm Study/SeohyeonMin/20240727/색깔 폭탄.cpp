#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory.h>
using namespace std;

const int MAXN = 20;
const int MAXM = 5;
const int INF = 1e9;

const int EMPTY = 0;
const int STONE = -1;
const int RED = MAXM + 1;

int n, m;
vector<vector<int>> brd;

// 폭탄
struct B {
	int color;
	int r, c;
	B() {}
	B(int _color, int _r, int _c) : color(_color), r(_r), c(_c) {}
};

// 폭탄 묶음
struct G {
	vector<B> bombs;
	int cnt_red;
	// 기준 행, 기준 열
	int r, c;

	G() {}
	G(vector<B> _bombs) {
		bombs = _bombs;
		int r_max = -1, c_min = INF;
		int cnt = 0;
		for (B& b : _bombs) {
			if (b.color == RED) {
				cnt++;
				continue;
			}
			r_max = max(r_max, b.r);
			c_min = min(c_min, b.c);
		}
		r = r_max, c = c_min;
		cnt_red = cnt;
	}
};

//==bfs용==//
struct V {
	int r, c;
	V() {};
	V(int _r, int _c) :r(_r), c(_c) {}
};
bool visited[MAXN][MAXN];
const int dr[] = { -1, 0, 1, 0 };
const int dc[] = { 0, -1, 0, 1 };

bool is_in_range(int r, int c) {
	if (r < 0 || r >= n || c < 0 || c >= n) return false;
	return true;
}

// O(n^4 * m)
int find_and_erase_bomb_group() {
	// vector<B> bombs의 크기로 비교하므로 g는 초기화 안해줘도 런타임에러 발생x
	G g;

	// 빨간색 + color 혹은 color로만 이루어진 폭탄 그룹 탐색
	for (int color = 1; color <= m; color++) {
		// (i, j)를 포함하는 가장 큰 폭탄 그룹 탐색
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (brd[i][j] != color) continue;
				memset(visited, false, sizeof(visited));
				vector<B> bombs;

				queue<V> que;
				que.push(V(i, j));
				
				while (!que.empty()) {
					V cur = que.front();
					que.pop();
					if (visited[cur.r][cur.c]) continue;
					visited[cur.r][cur.c] = true;
					bombs.push_back(B(brd[cur.r][cur.c], cur.r, cur.c));

					for (int d = 0; d < 4; d++) {
						int nr = cur.r + dr[d];
						int nc = cur.c + dc[d];
						if (!is_in_range(nr, nc)) continue;
						if (visited[nr][nc]) continue;
						if (brd[nr][nc] == EMPTY) continue;
						if (brd[nr][nc] != RED && brd[nr][nc] != color) continue;
					
						que.push(V(nr, nc));
					}
				}

				// 폭탄 개수는 2개 이상이어야 함
				if (bombs.size() < 2) {
					continue;
				}

				G g_cur = G(bombs);

				// 크기가 큰 폭탄 묶음 선택
				if (g_cur.bombs.size() > g.bombs.size()) {
					g = g_cur;
				}
				else if (g_cur.bombs.size() == g.bombs.size()) {
					// 1. 빨간색 폭탄이 가장 적게 포함된 것
					if (g_cur.cnt_red < g.cnt_red) {
						g = g_cur;
					}
					else if (g_cur.cnt_red == g.cnt_red) {
						// 2. 행 기준 가장 큰 칸 선택
						if (g_cur.r > g.r) {
							g = g_cur;
						}
						else if (g_cur.r == g.r) {
							// 3. 열 기준 가장 작은 칸 선택
							if (g_cur.c < g.c) {
								g = g_cur;
							}
						}
					}
				}

			}
		}
	}

	for (B& b : g.bombs) {
		brd[b.r][b.c] = EMPTY;
	}
	return g.bombs.size() * g.bombs.size();
}

// O(n^2)
void apply_gravity() {
	vector<vector<int>> new_brd = vector<vector<int>>(n, vector<int>(n, 0));

	for (int c = 0; c < n; c++) {
		vector<int> tmp;
		for (int r = 0; r < n; r++) {
			if (brd[r][c] != STONE && brd[r][c] != EMPTY) {
				tmp.push_back(brd[r][c]);
			}
			
			if (brd[r][c] == STONE) {
				new_brd[r][c] = STONE;

				int sz = tmp.size();
				for (int i = sz - 1, nr = r - 1; i >= 0; i--, nr--) {
					new_brd[nr][c] = tmp[i];
				}
				tmp.clear();
			}
		}
		if (tmp.size()) {
			int sz = tmp.size();
			for (int i = sz - 1, nr = n - 1; i >= 0; i--, nr--) {
				new_brd[nr][c] = tmp[i];
			}
		}
	}

	brd = new_brd;
}

// O(n^2)
void rotate_counter_clockwise() {
	vector<vector<int>> new_brd = vector<vector<int>>(n, vector<int>(n, 0));
	for (int c = n - 1, nr = 0; c >= 0; c--, nr++) {
		for (int r = 0, nc = 0; r < n; r++, nc++) {
			new_brd[nr][nc] = brd[r][c];
		}
	}
	brd = new_brd;
}

int main() {
	scanf("%d %d", &n, &m);
	brd = vector<vector<int>>(n, vector<int>(n, 0));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
			// RED는 0이 아닌 MAXM + 1로 치환.
			// 편의상 빈칸을 0으로 사용하기 위함
			if (brd[i][j] == 0) brd[i][j] = RED;
		}
	}

	/*
	폭탄 묶음이란
	1. 상하좌우로 인접한 2개 이상의 폭탄 집합
	2. 같은 집합 내의 폭탄
		2.1. 모두 같은 색의 폭탄이거나
		2.2. 빨간색 폭탄을 포함하여 2개의 색깔로만 이루어진 폭탄이어야 함
	*/
	
	int score = 0;
	bool is_end = false;
	while (!is_end) {
		// 폭탄 묶음 탐색 및 제거
		int score_cur = find_and_erase_bomb_group();
		score += score_cur;
		if (score_cur == 0) break;

		// 중력 작용
		apply_gravity();

		// 90도 회전
		rotate_counter_clockwise();
		
		// 중력 작용
		apply_gravity();
	}
	
	printf("%d", score);
}