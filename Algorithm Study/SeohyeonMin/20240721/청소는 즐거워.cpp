#include <cstdio>
#include <vector>
using namespace std;

const int MAXN = 500;

int n;
int brd[MAXN][MAXN];

// 좌, 하, 우, 상 순서
int dr[] = { 0, 1, 0, -1 };
int dc[] = { -1, 0, 1, 0 };

// 왼쪽으로 이동 시 퍼지는 비율
const vector<vector<int>> dust_map_left = {
	{0, 0, 2, 0, 0},
	{0, 10, 7, 1, 0},
	{5, 0, 0, 0, 0},
	{0, 10, 7, 1, 0},
	{0, 0, 2, 0, 0}
};

vector<vector<int>> rotate_counter_clockwise(vector<vector<int>>& p) {
	vector<vector<int>> ret(p.size(), vector<int>(p.size(), 0));
	int psz = p.size();

	// r축 기준 양의 방향 90도 회전
	// (c -s) (r)
	// (s c)  (c)
	// (0, -1)(r)
	// (1, 0) (c) 
	for (int i = 0; i < psz; i++) {
		for (int j = 0; j < psz; j++) {
			int r = i - psz / 2;
			int c = j - psz / 2;

			// (r, c) -> (-c, r)
			ret[-c + psz / 2][r + psz / 2] = p[i][j];
		}
	}
	return ret;
}

vector<vector<int>> rotate_by_direction(int d) {
	vector<vector<int>> ret = dust_map_left;
	while (d--) {
		ret = rotate_counter_clockwise(ret);
	}
	return ret;
}

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int r_cur = n / 2, c_cur = n / 2;
	int d_cur = 0;
	int step = 1;
	int step_remain = step;
	int step_count = 0;
	vector<vector<int>> diff(n, vector<int>(n, 0));

	int ans = 0;
	while (r_cur != 0 || c_cur != 0) {
		int r_next = r_cur + dr[d_cur];
		int c_next = c_cur + dc[d_cur];

		// 청소기 (r_cur, c_cur) -> (r_next, c_next) 이동
		// x: 청소할 먼지양
		int x = brd[r_next][c_next];	
		vector<vector<int>> dust_map = rotate_by_direction(d_cur);
		int w_sum = 0;

		// 흩날릴 먼지양 계산
		for (int i = 0; i < dust_map.size(); i++) {
			for (int j = 0; j < dust_map.size(); j++) {
				int dr = i - dust_map.size() / 2;
				int dc = j - dust_map.size() / 2;

				int r_taget = r_next + dr;
				int c_target = c_next + dc;
				int w_cur = x * dust_map[i][j] / 100;
				w_sum += w_cur;

				if (is_in_range(r_taget, c_target)) {
					diff[r_taget][c_target] = w_cur;
				}
				else {
					// 격자 밖으로 떨어진 먼지
					ans += w_cur;
				}
			}
		}

		// a% 칸에 해당하는 먼지양 계산
		int w_rest = x - w_sum;
		int r_a = r_next + dr[d_cur];
		int c_a = c_next + dc[d_cur];
		if (is_in_range(r_a, c_a)) {
			diff[r_a][c_a] = w_rest;
		}
		else {
			// 격자 밖으로 떨어진 먼지
			ans += w_rest;
		}

		// 먼지양 갱신
		brd[r_next][c_next] = 0;
		for (int i = 0; i < dust_map.size(); i++) {
			for (int j = 0; j < dust_map.size(); j++) {
				int dr = i - dust_map.size() / 2;
				int dc = j - dust_map.size() / 2;

				int r_target = r_next + dr;
				int c_target = c_next + dc;
				if (is_in_range(r_target, c_target)) {
					brd[r_target][c_target] += diff[r_target][c_target];
					// diff 초기화
					diff[r_target][c_target] = 0;
				}
			}
		}

		// 이동 정보 갱신
		r_cur = r_next, c_cur = c_next;
		step_remain--;
		if (step_remain == 0) {
			step_count++;
			d_cur = (d_cur + 1) % 4;

			if (step_count == 2) {
				step++;
				step_count = 0;
			}
			step_remain = step;
		}
	}

	printf("%d", ans);
}