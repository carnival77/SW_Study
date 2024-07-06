/*
* 종전
* 시간복잡도: O(n^5)
*/
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int INF = 1e9;
const int MAXN = 20;

int n;
int brd[MAXN + 1][MAXN + 1];
int sum_r[MAXN + 1][MAXN + 1], sum_c[MAXN + 1][MAXN + 1];

bool is_in_range(int r, int c) {
	if (r < 1 || r > n || c < 1 || c > n) return false;
	return true;
}


int main() {
	int total = 0;
	scanf("%d", &n);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			scanf("%d", &brd[i][j]);
			total += brd[i][j];

			sum_r[i][j] = sum_r[i][j - 1] + brd[i][j];
		}
	}

	for (int c = 1; c <= n; c++) {
		for (int r = 1; r <= n; r++) {
			sum_c[r][c] = sum_c[r - 1][c] + brd[r][c];
		}
	}

	int ans = INF;
	for (int r_top = 1; r_top <= n; r_top++) {
		for (int c_top = 1; c_top <= n; c_top++) {
			// l1: 좌측아래 방향 길이
			// l2: 우측아래 방향 길이
			for (int l1 = 1; l1 <= n - 1; l1++) {
				for (int l2 = 1; l2 <= n - 1; l2++) {
					
					int r_left = r_top + l1;
					int c_left = c_top - l1;

					int r_right = r_top + l2;
					int c_right = c_top + l2;

					int r_bottom = r_top + l1 + l2;
					int c_bottom = c_top - l1 + l2;

					if (!is_in_range(r_left, c_left) 
						|| !is_in_range(r_right, c_right)
						|| !is_in_range(r_bottom, c_bottom)) {
						continue;
					}

					vector<int> sum(6, 0);
					// 2번 부족: 좌측 상단
					for (int r = r_left - 1, add = 0; r >= 1; r--, add++) {
						sum[2] += sum_r[r][min(c_top, c_left + add)];
						/*for (int c = min(c_top, c_left + add); c >= 0; c--) {
							sum[2] += brd[r][c];
						}*/
					}

					// 3번 부족: 우측 상단
					for (int c = c_top + 1, add = 0; c <= n; c++, add++) {
						sum[3] += sum_c[min(r_right, r_top + add)][c];
						/*for (int r = min(r_right, r_top + add); r >= 0; r--) {
							sum[3] += brd[r][c];
						}*/
					}

					// 4번 부족: 좌측 하단
					for (int c = c_bottom - 1, add = 0; c >= 1; c--, add--) {
						sum[4] += sum_c[n][c]- sum_c[max(r_left, r_bottom + add)-1][c];
						/*for (int r = max(r_left, r_bottom + add); r < n; r++) {
							sum[4] += brd[r][c];
						}*/
					}
					
					// 5번 부족: 우측 하단
					for (int r = r_right + 1, add = 0; r <= n; r++, add--) {
						sum[5] += sum_r[r][n] - sum_r[r][max(c_bottom, c_right + add)-1];
						/*for (int c = max(c_bottom, c_right + add); c < n; c++) {
							sum[5] += brd[r][c];
						}*/
					}

					// 1번 부족
					sum[1] = total - (sum[2] + sum[3] + sum[4] + sum[5]);
					sort(sum.begin(), sum.end());
					int cand = (sum.back() - sum[1]);

					ans = min(ans, cand);
				}
			}
		}
	}

	printf("%d", ans);
}