#include <iostream>
#include <climits>
#include <vector>
#define ENDL '\n'

using namespace std;
using ll = long long;

ll minGap = LLONG_MAX;
int numOfTask;
int task[21][21];
bool taskSelected[21] = { false, };

ll llMin(ll a, ll b) {
	if (a < b) return a;
	return b;
}

ll llabs(ll a) {
	if (a < 0) return a * -1;
	return a;
}

void dfs(int taskIdx, ll sum, int cnt) {
	if (cnt == numOfTask / 2) {
		ll otherSum = 0;
		for (int i = 1; i <= numOfTask; i++) {
			if (taskSelected[i] == false) {
				for (int j = 1; j < i; j++) {
					if (taskSelected[j] == false) {
						otherSum += task[i][j] + task[j][i];
					}
				}
			}
		}
		minGap = llMin(minGap, llabs(sum - otherSum));
		return;
	}

	if (taskIdx == numOfTask + 1) return;

	ll tmpSum = sum;
	for (int i = 1; i <= taskIdx; i++) {
		if (taskSelected[i]) {
			tmpSum += task[taskIdx][i] + task[i][taskIdx];
		}
	}

	taskSelected[taskIdx] = true;
	dfs(taskIdx + 1, tmpSum, cnt + 1);

	if (taskIdx == 1) return; //pruning

	taskSelected[taskIdx] = false;
	dfs(taskIdx + 1, sum, cnt);
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> numOfTask;
	for (int i = 1; i <= numOfTask; i++) {
		for (int j = 1; j <= numOfTask; j++) {
			cin >> task[i][j];
		}
	}

	dfs(1, 0, 0);
	cout << minGap;

	return 0;
}