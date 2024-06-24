#include <cstdio>
#include <vector>
using namespace std;

const int NONE = 0;
const int LEFT = 1;
const int RIGHT = 2;

const int MAXH = 30, MAXN = 10;
int n, m, h;
// ladder[i][j]=1: 왼쪽방향 사다리가 있다는 뜻
// ladder[i][j]=2: 오른쪽으로 사다리가 있다는 뜻
// ladder: h x n
int ladder[MAXH][MAXN];
vector<int> result;

bool is_in_range(int r, int c) {
	if (r<0 || r>h || c<0 || c>n) return false;
	return true;
}

int ans = 4;
void dfs(int r, int c, int remain) {
	if (c == n - 1) {
		dfs(r + 1, 0, remain);
		return;
	}

	if (r == h) {
		bool valid = true;
		for (int i = 0; i < n; i++) {
			if (result[i] != i) {
				valid = false; 
				return;
			}
		}

		ans = min(ans, 3 - remain);
		return;
	}

	if (remain>0 && ladder[r][c] == NONE) {
		// 오른쪽방향 사다리 놓기 가능 여부 확인
		if (is_in_range(r, c + 1) && ladder[r][c + 1] == NONE) {
			ladder[r][c] = RIGHT;
			ladder[r][c + 1] = LEFT;
			swap(result[c], result[c + 1]);

			dfs(r, c + 1, remain-1);

			ladder[r][c] = NONE;
			ladder[r][c + 1] = NONE;
			swap(result[c], result[c + 1]);
		}
	}
	
	if (ladder[r][c] == RIGHT) {
		swap(result[c], result[c + 1]);
		dfs(r, c + 1, remain);
		swap(result[c], result[c + 1]);
	}
	else {
		dfs(r, c + 1, remain);
	}
}

int main() {
	scanf("%d %d %d", &n, &m, &h);
	//ladder.resize(h, vector<int>(n, 0));

	for (int i = 0, a, b; i < m; i++) {
		// 취약지점 a에서 사람 b->b+1 유실
		scanf("%d %d", &a, &b);
		a--, b--;

		ladder[a][b] = 2;
		ladder[a][b+1] = 1;
	}

	for (int i = 0; i < n; i++) {
		result.push_back(i);
	}
	// 가로 사다리는 최대 3개 추가 가능
	// 가로 사다리 개수는 최대 (n-1)*h개
	// (n-1)*h 개 사다리 중에 3개 선택
	dfs(0, 0, 3);
	printf("%d", ans > 3 ? -1 : ans);
}