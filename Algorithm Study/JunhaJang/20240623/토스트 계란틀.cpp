#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#define ENDL '\n'

using namespace std;
using ll = long long;

int dRow[4] = { -1, 1, 0, 0 };
int dCol[4] = { 0, 0, -1, 1 };

int answer = 0;
int boardSize;
int minBound, maxBound;
vector<vector<int>>board;
vector<vector<int>>basicCellNum;

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardSize && 0 <= col && col < boardSize) return true;
	return false;
}

bool canMove(int row, int col, int nextRow, int nextCol) {
	int diff = abs(board[row][col] - board[nextRow][nextCol]);
	if (minBound <= diff && diff <= maxBound) return true;
	return false;
}

bool gamePlay() {
	bool isChange = false;
	vector<vector<bool>>visited(boardSize, vector<bool>(boardSize, false));
	vector<vector<int>>cellNumBoard(basicCellNum);
	
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (visited[i][j]) continue;

			int cellSum = 0;
			int cellCount = 0;
			int tmpCellNum = basicCellNum[i][j];

			queue<pair<int, int>>q;
			queue<pair<int, int>>willChange;
			q.push({ i,j });

			while (!q.empty()) {
				auto [row, col] = q.front();
				q.pop();

				if (visited[row][col]) continue;
				visited[row][col] = true;
				cellNumBoard[row][col] = tmpCellNum;

				willChange.push({ row, col });
				cellSum += board[row][col];
				cellCount++;

				for (int i = 0; i < 4; i++) {
					int nextRow = row + dRow[i];
					int nextCol = col + dCol[i];
					if (isValidRange(nextRow, nextCol) && visited[nextRow][nextCol] == false) {
						if (canMove(row, col, nextRow, nextCol)) {
							isChange = true;
							q.push({ nextRow, nextCol });
						}
					}
				}
			}

			int changeNum = cellSum / cellCount;
			while (!willChange.empty()) {
				auto [row, col] = willChange.front();
				willChange.pop();
				board[row][col] = changeNum;
			}
		}
	}
	
	return isChange;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardSize >> minBound >> maxBound;
	board.resize(boardSize, vector<int>(boardSize));
	basicCellNum.resize(boardSize, vector<int>(boardSize));

	int cellCount = 0;
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cin >> board[i][j];
			basicCellNum[i][j] = cellCount++;
		}
	}

	while (gamePlay()) answer++;

	cout << answer;

	return 0;
}