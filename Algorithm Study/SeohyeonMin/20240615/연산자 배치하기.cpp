#include <cstdio>
#include <algorithm>
using namespace std;

#define ADDITION 0
#define SUBSTRACTION 1
#define MULTIPLICATION 2

const int MAXN = 11;

int n;
int A[MAXN];
int operator_cnt[3];
int pos[MAXN-1];
int mi = 2e9;
int mx = -2e9;

void dfs(int pi) {
	if (pi == n-1) {
		int ret = A[0];
		for (int i = 0; i < n - 1; i++) {
			if (pos[i] == ADDITION) {
				ret += A[i + 1];
			}
			else if (pos[i] == SUBSTRACTION) {
				ret -= A[i + 1];
			}
			else {
				ret *= A[i + 1];
			}
		}
		mi = min(ret, mi);
		mx = max(ret, mx);
		return;
	}

	for (int i = 0; i < 3; i++) {
		if (operator_cnt[i] > 0) {
			pos[pi] = i;
			operator_cnt[i]--;
			dfs(pi + 1);
			operator_cnt[i]++;
		}
	}
}

int main() {
	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &A[i]);
	}
	for (int i = 0; i < 3; i++) {
		scanf("%d", &operator_cnt[i]);
	}

	dfs(0);

	printf("%d %d", mi, mx);
}