/*
* 격자 숫자 놀이
* 시간복잡도: O(n^2*log(n))
*/
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

struct V {
	int num;
	int cnt;
	V() {}
	V(int _num, int _cnt): num(_num), cnt(_cnt) {}
};

const int MAXN = 200;
const int MAXK = 100;

int r_target, c_target, k_target;
int brd[MAXN][MAXN];
int new_brd[MAXN][MAXN];
int cnt[MAXK+1];

int main() {
	scanf("%d %d %d", &r_target, &c_target, &k_target);
	r_target--, c_target--;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int n = 3, m = 3;
	int ans = -1;
	// 시간복잡도: O(100 * n * (n + 100 + n*log(n) + n)
	// = O(n * n*log(n))
	for (int t = 0; t < 100; t++) {
		if (brd[r_target][c_target] == k_target) {
			ans = t;
			break;
		}
		
		// 행 기준 연산
		if (n >= m) {
			int m_max = -1;
			for (int r = 0; r < n; r++) {
				// 빈도수 계산
				vector<V> freqs;
				for (int c = 0; c < m; c++) {
					cnt[brd[r][c]]++;
				}
				for (int num = 1; num <= MAXK; num++) {
					if (!cnt[num]) continue;
					freqs.push_back(V(num, cnt[num]));

					// initialize
					cnt[num] = 0;
				}

				// 빈도수(cnt) 오름차순 정렬, 빈도수가 같다면 숫자(num) 기준 오름차순으로 정렬
				sort(freqs.begin(), freqs.end(), 
					[](const V& v1, const V& v2) {
						if (v1.cnt == v2.cnt) return v1.num < v2.num;
						return v1.cnt < v2.cnt;
					}
				);

				// 열 크기 갱신
				m_max = max(m_max, (int)freqs.size() * 2);
				
				// 새로운 배열 갱신
				int c = 0;
				for (V& freq : freqs) {
					new_brd[r][c++] = freq.num;
					new_brd[r][c++] = freq.cnt;
				}
			}
			m = m_max;
		}
		// 열 기준 연산
		else {
			int n_max = -1;
			for (int c = 0; c < m; c++) {
				vector<V> freqs;
				for (int r = 0; r < n; r++) {
					cnt[brd[r][c]]++;
				}

				for (int num = 1; num <= MAXK; num++) {
					if (!cnt[num]) continue;
					freqs.push_back(V(num, cnt[num]));

					// initialize
					cnt[num] = 0;
				}

				sort(freqs.begin(), freqs.end(),
					[](const V& v1, const V& v2) {
						if (v1.cnt == v2.cnt) return v1.num < v2.num;
						return v1.cnt < v2.cnt;
					}
				);
				n_max = max(n_max, (int)freqs.size() * 2);

				int r = 0;
				for (V& freq : freqs) {
					new_brd[r++][c] = freq.num;
					new_brd[r++][c] = freq.cnt;
				}
			}
			n = n_max;
		}

		// 변경사항 반영
		for (int r = 0; r < n; r++) {
			for (int c = 0; c < m; c++) {
				brd[r][c] = new_brd[r][c];
				
				// initialize
				new_brd[r][c] = 0;
			}
		}
	}

	printf("%d", ans);
}