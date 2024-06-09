#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#define ENDL '\n'

using namespace std;
using ll = long long;

int maxDays;
int maxProfit = INT_MIN;
vector<pair<int, int>>task;

void dfs(int day, int profit) {
	if (day > maxDays + 1) return;
	if (day == maxDays + 1) {
		maxProfit = max(maxProfit, profit);
		return;
	}
	dfs(day + task[day].first, profit + task[day].second);
	dfs(day + 1, profit);
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> maxDays;
	task.resize(maxDays + 1);
	for (int i = 1; i <= maxDays; i++) {
		int cost, profit; cin >> cost >> profit;
		task[i] = { cost, profit };
	}
	dfs(1, 0);
	cout << maxProfit;
	return 0;
}