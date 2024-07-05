#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <string>
#include <cmath>
#include <stack>
#include <queue>
#include <tuple>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#define ENDL '\n'

using namespace std;
using ll = long long;

int dRow[4] = { -1, 1, 0, 0 };
int dCol[4] = { 0, 0, -1, 1 };

int boardRow, boardCol;
int stormRow, stormCol;
vector<vector<int>>board;
vector<pair<int, int>>downChain;
vector<pair<int, int>>upChain;

void printBoard() {
	cout << ENDL;
	for (int row = 0; row < boardRow; row++) {
		for (int col = 0; col < boardCol; col++) {
			cout << board[row][col] << " ";
		}
		cout << ENDL;
	}
	cout << ENDL;
}

void makeChain() {
	int downRow = stormRow;
	int downCol = stormCol;

	int tmpRow = downRow, tmpCol = downCol;
	for (tmpRow = tmpRow + 1; tmpRow < boardRow; tmpRow++) {
		downChain.push_back({ tmpRow, tmpCol });
	}
	tmpRow--;
	for (tmpCol = tmpCol + 1; tmpCol < boardCol; tmpCol++) {
		downChain.push_back({ tmpRow, tmpCol });
	}
	tmpCol--;
	for (tmpRow = tmpRow - 1; tmpRow >= downRow; tmpRow--) {
		downChain.push_back({ tmpRow, tmpCol });
	}
	tmpRow++;
	for (tmpCol = tmpCol - 1; tmpCol > downCol; tmpCol--) {
		downChain.push_back({ tmpRow, tmpCol });
	}

	int upRow = stormRow - 1;
	int upCol = stormCol;

	tmpRow = upRow, tmpCol = upCol;

	for (tmpRow = tmpRow - 1; tmpRow >= 0; tmpRow--) {
		upChain.push_back({ tmpRow, tmpCol });
	}
	tmpRow++;
	for (tmpCol = tmpCol + 1; tmpCol < boardCol; tmpCol++) {
		upChain.push_back({ tmpRow, tmpCol });
	}
	tmpCol--;
	for (tmpRow = tmpRow + 1; tmpRow <= upRow; tmpRow++) {
		upChain.push_back({ tmpRow, tmpCol });
	}
	tmpRow--;
	for (tmpCol = tmpCol - 1; tmpCol > 0; tmpCol--) {
		upChain.push_back({ tmpRow, tmpCol });
	}
}

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardRow && 0 <= col && col < boardCol) return true;
	return false;
}

void spreadDust() {
	vector<vector<int>>spreadBoard(boardRow, vector<int>(boardCol, 0));

	for (int row = 0; row < boardRow; row++) {
		for (int col = 0; col < boardCol; col++) {
			if (board[row][col] == -1) continue;

			int amountOfSpread = board[row][col] / 5;
			int spreadCount = 0;

			for (int i = 0; i < 4; i++) {
				int nextRow = row + dRow[i];
				int nextCol = col + dCol[i];
				if (isValidRange(nextRow, nextCol) && board[nextRow][nextCol] != -1) {
					spreadBoard[nextRow][nextCol] += amountOfSpread;
					spreadCount++;
				}
			}
			board[row][col] -= amountOfSpread * spreadCount;
		}
	}

	for (int row = 0; row < boardRow; row++) {
		for (int col = 0; col < boardCol; col++) {
			board[row][col] += spreadBoard[row][col];
		}
	}

}

void pullDust(vector<pair<int, int>>chain) {
	for (int i = 0; i < chain.size() - 1; i++) {
		auto [tmpRow, tmpCol] = chain[i];
		auto [nextRow, nextCol] = chain[i + 1];
		board[tmpRow][tmpCol] = board[nextRow][nextCol];
	}
	auto [lastRow, lastCol] = chain.back();
	board[lastRow][lastCol] = 0;
}

void cleanDust() {
	pullDust(downChain);
	pullDust(upChain);
}

int getTotalDust() {
	int sum = 0;
	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			if (board[i][j] == -1) continue;
			sum += board[i][j];
		}
	}
	return sum;
}
int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int T;
	cin >> boardRow >> boardCol >> T;
	board.resize(boardRow, vector<int>(boardCol));
	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			cin >> board[i][j];
			if (board[i][j] == -1) {
				stormRow = i;
				stormCol = j;
			}
		}
	}
	makeChain();
	while (T-- > 0) {
		spreadDust();
		cleanDust();
	}
	cout << getTotalDust();
	return 0;
}