#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include <tuple>
#define ENDL '\n'

using namespace std;
using ll = long long;

int dRow[4] = { -1, 1, 0, 0 };
int dCol[4] = { 0, 0, -1, 1 };

vector<vector<int>>board;
int boardRow, boardCol;

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardRow && 0 <= col && col < boardCol) return true;
	return false;
}

bool isNotSameBlock(int beforeRow, int beforeCol, int row, int col) {
	if (row != beforeRow || col != beforeCol) return true;
	return false;
}

int bfs_to_find_max(int startRow, int startCol) {
	queue<tuple<int, int, int, int, int, int>>q;
	int maxSum = INT_MIN;

	q.push({ -1, -1, startRow, startCol, 1, board[startRow][startCol] });
	while (!q.empty()) {
		auto [beforeRow, beforeCol, row, col, cnt, sum] = q.front();
		q.pop();

		if (cnt == 4) {
			maxSum = max(maxSum, sum);
			continue;
		}


		for (int i = 0; i < 4; i++) {
			int nextRow = row + dRow[i];
			int nextCol = col + dCol[i];
			if (isValidRange(nextRow, nextCol) && startRow <= nextRow && isNotSameBlock(beforeRow, beforeCol, nextRow, nextCol)) {
				q.push({ row, col, nextRow, nextCol, cnt + 1, sum + board[nextRow][nextCol] });
			}
		}


		if (cnt == 2) {
			if (startRow == 1 && startCol == 0 && row == 1 && col == 1) {
				int here = 3;
			}

			for (int i = 0; i < 4; i++) {
				int nextRow1 = row + dRow[i];
				int nextCol1 = col + dCol[i];
				if (!isValidRange(nextRow1, nextCol1)) continue;
				if (!isNotSameBlock(beforeRow, beforeCol, nextRow1, nextCol1) || nextRow1 < startRow) continue;
				for (int j = 0; j < 4; j++) {
					int nextRow2 = row + dRow[j];
					int nextCol2 = col + dCol[j];
					if (!isValidRange(nextRow2, nextCol2)) continue;
					if (nextRow2 < startRow) continue;

					if (isNotSameBlock(beforeRow, beforeCol, nextRow2, nextCol2) && isNotSameBlock(nextRow1, nextCol1, nextRow2, nextCol2)) {
						maxSum = max(maxSum, sum + board[nextRow1][nextCol1] + board[nextRow2][nextCol2]);
					}
				}
			}
		}
	}

	return maxSum;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardRow >> boardCol;
	board.resize(boardRow, vector<int>(boardCol, 0));
	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			cin >> board[i][j];
		}
	}

	int maxValue = INT_MIN;
	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			maxValue = max(maxValue, bfs_to_find_max(i, j));
		}
	}

	cout << maxValue;

	return 0;
}