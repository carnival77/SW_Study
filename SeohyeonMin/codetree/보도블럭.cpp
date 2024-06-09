#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 100;

int n;
int L;
int brd[MAXN][MAXN];

bool is_available(vector<int>& line) {
	int lsz = line.size();
	// 인접한 보도블록의 경사 차이가 2 이상인 경우 불가능
	for (int i = 1; i < lsz; i++) {
		if (abs(line[i - 1] - line[i]) > 1) return false;
	}

	// 연속된 같은 높이의 보도블록 개수 누적합 계산
	vector<int> psum(lsz, 0);
	psum[0] = 1;
	for (int i = 1; i < lsz; i++) {
		if (line[i] == line[i - 1]) {
			psum[i] = psum[i - 1] + 1;
		}
		else {
			psum[i] = 1;
		}
	}

	vector<int> psum_reverse(lsz, 0);
	psum_reverse[lsz - 1] = 1;
	for (int i = lsz - 2; i >= 0; i--) {
		if (line[i] == line[i + 1]) {
			psum_reverse[i] = psum_reverse[i + 1] + 1;
		}
		else {
			psum_reverse[i] = 1;
		}
	}

	
	// 판별
	vector<bool> chk(lsz, false);
	for (int i = 1; i < lsz; i++) {
		if (psum[i - 1] >= psum[i]) {
			// 높아진 경우
			if (line[i - 1] < line[i]) {
				// 너무 짧아서 경사로를 놓을 수 없는 경우
				if (psum[i - 1] < L) return false;

				for (int ci = 0; ci < L; ci++) {
					// 이미 경사로가 놓여있는데 또 놓아야하는 경우
					if (chk[i - 1 - ci]) return false;

					chk[i - 1 - ci] = true;
				}
			}
			// 낮아진 경우
			else if(line[i-1] > line[i]) {
				// 너무 짧아서 경사로를 놓을 수 없는 경우
				if (psum_reverse[i] < L) return false;

				for (int ci = 0; ci < L; ci++) {
					// 이미 경사로가 놓여있는데 또 놓아야하는 경우
					if (chk[i + ci]) return false;

					chk[i + ci] = true;
				}
			}
		}
	}

	return true;
}

int main() {
	scanf("%d %d", &n, &L);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &brd[i][j]);
		}
	}

	int cnt = 0;
	for (int r = 0; r < n; r++) {
		vector<int> cand;
		for (int c = 0; c < n; c++) {
			cand.push_back(brd[r][c]);
		}
		cnt += is_available(cand);
	}

	for (int c = 0; c < n; c++) {
		vector<int> cand;
		for (int r = 0; r < n; r++) {
			cand.push_back(brd[r][c]);
		}
		cnt += is_available(cand);
	}

	printf("%d", cnt);
}