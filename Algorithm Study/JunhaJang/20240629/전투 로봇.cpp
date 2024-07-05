#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include <tuple>
#define ENDL '\n'

using namespace std;
using ll = long long;

int dRow[4] = { -1, 0, 0, 1 };
int dCol[4] = { 0, -1, 1, 0 };

int boardSize;
int robotRow, robotCol;
int robotLevel = 2;
int levelUpCnt = 2;
int timeCnt = 0;
vector<vector<int>>board;

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardSize && 0 <= col && col < boardSize) return true;
	return false;
}

bool gamePlay() {
	bool find = false;
	int minStep = INT_MAX;
	int monsterRow = INT_MAX, monsterCol = INT_MAX;
	queue<tuple<int, int, int>>q;
	vector<vector<bool>>visited(boardSize, vector<bool>(boardSize, false));
	q.push({ robotRow, robotCol, 0 });


	while (!q.empty()) {
		auto [row, col, step] = q.front();
		q.pop();
		if (step > minStep) continue;
		if (visited[row][col]) continue;
		if (board[row][col] > robotLevel) continue;
		visited[row][col] = true;

		if (board[row][col] != 0 && board[row][col] < robotLevel) {
			find = true;
			minStep = step;
			if (row < monsterRow) {
				monsterRow = row;
				monsterCol = col;
			}
			else if (row == monsterRow && col < monsterCol) {
				monsterRow = row;
				monsterCol = col;
			}

			continue;
		}

		for (int i = 0; i < 4; i++) {
			int nextRow = row + dRow[i];
			int nextCol = col + dCol[i];
			if (isValidRange(nextRow, nextCol) && visited[nextRow][nextCol] == false) {
				q.push({ nextRow, nextCol, step + 1 });
			}
		}
	}

	if (find) {
		timeCnt += minStep;
		levelUpCnt--;
		if (levelUpCnt == 0) {
			robotLevel++;
			levelUpCnt = robotLevel;
		}

		board[monsterRow][monsterCol] = 0;
		robotRow = monsterRow;
		robotCol = monsterCol;
	}

	return find;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardSize;
	board.resize(boardSize, vector<int>(boardSize, 0));
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cin >> board[i][j];
			if (board[i][j] == 9) {
				robotRow = i;
				robotCol = j;
				board[i][j] = 0;
			}
		}
	}

	while (gamePlay()) {}
	cout << timeCnt;

	return 0;
}