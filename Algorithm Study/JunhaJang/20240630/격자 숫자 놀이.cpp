#include <iostream>
#include <algorithm>
#include <vector>
#define ENDL '\n'

using namespace std;
using ll = long long;

int maxRow, maxCol;
int targetRow, targetCol, targetNum;
int timeCnt = 0;

int board[100][100] = { 0 };

bool targetFound() {
	if (board[targetRow][targetCol] == targetNum) return true;
	return false;
}

vector<int>getSortedLine(vector<int>& line) {
	vector<int>numCnt(101, 0);
	for (int num : line) {
		numCnt[num]++;
	}
	vector<pair<int, int>>pairVec;
	for (int num = 1; num < 101; num++) {
		if (numCnt[num] == 0) continue;
		pairVec.push_back({ numCnt[num], num });
	}

	sort(pairVec.begin(), pairVec.end(), [](const pair<int, int>& a, const pair<int, int>& b) -> bool {
		if (a.first == b.first) {
			return a.second < b.second;
		}
		return a.first < b.first;
	});

	vector<int>sortedVec;
	for (pair<int, int>p : pairVec) {
		sortedVec.push_back(p.second);
		sortedVec.push_back(p.first);
	}
	return sortedVec;
}

void gamePlay() {
	if (maxRow >= maxCol) {
		for (int row = 0; row < maxRow; row++) {
			vector<int>line;
			for (int col = 0; col < maxCol; col++) {
				if (board[row][col] == 0) continue;
				line.push_back(board[row][col]);
			}

			vector<int>sortedLine = getSortedLine(line);

			if (sortedLine.size() > maxCol) maxCol = sortedLine.size();
			if (maxCol > 100) maxCol = 100;

			for (int col = 0; col < 100; col++) {
				if (col < sortedLine.size()) {
					board[row][col] = sortedLine[col];
					continue;
				}
				board[row][col] = 0;
			}
		}
	}
	else {
		for (int col = 0; col < maxCol; col++) {
			vector<int>line;
			for (int row = 0; row < maxCol; row++) {
				if (board[row][col] == 0) continue;
				line.push_back(board[row][col]);
			}

			vector<int>sortedLine = getSortedLine(line);

			if (sortedLine.size() > maxRow) maxRow = sortedLine.size();
			if (maxRow > 100) maxRow = 100;

			for (int row = 0; row < 100; row++) {
				if (row < sortedLine.size()) {
					board[row][col] = sortedLine[row];
					continue;
				}
				board[row][col] = 0;
			}
		}
	}
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> targetRow >> targetCol >> targetNum;
	targetRow--; targetCol--;
	maxRow = 3; maxCol = 3;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cin >> board[i][j];
		}
	}

	while (timeCnt <= 100 && !targetFound()) {
		gamePlay();
		timeCnt++;
	}

	if (timeCnt > 100) cout << -1;
	else cout << timeCnt;

	return 0;
}