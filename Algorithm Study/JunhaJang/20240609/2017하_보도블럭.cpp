#include <iostream>
#include <vector>
#define ENDL '\n'

using namespace std;
using ll = long long;

int boardSize;
int lengthOfSlide;
vector<vector<int>>board;

bool isNotValidRange(int idx) {
	if (idx < 0 || boardSize <= idx) return true;
	return false;
}

int checkTwoLines(int line) {
	int availableRoadCount = 0;

	/****************** start check row ******************/
	int sameCount = 1;
	bool isAvailable = true;
	for (int col = 1; col < boardSize; col++) {
		int tmp = board[line][col];
		int before = board[line][col - 1];

		if (tmp == before) {
			sameCount++;
			continue;
		}

		if (tmp == before + 1) {
			if (sameCount < lengthOfSlide) {
				isAvailable = false;
				break;
			}
			sameCount = 1;
			continue;
		}

		if (tmp == before - 1) {
			if (isNotValidRange(col + lengthOfSlide - 1)) {
				isAvailable = false;
				break;
			}

			int nextCol = col;
			while (nextCol <= col + lengthOfSlide - 1) {
				if (tmp != board[line][nextCol]) {
					isAvailable = false;
				}
				nextCol++;
			}
			if (isAvailable == false) break;

			col = nextCol - 1;
			sameCount = 0;
			continue;
		}

		//gap is bigger than 2
		isAvailable = false;
		break;
	}
	if (isAvailable) availableRoadCount++;
	/****************** end check row ******************/

	/****************** start check col ******************/
	sameCount = 1;
	isAvailable = true;
	for (int row = 1; row < boardSize; row++) {
		int tmp = board[row][line];
		int before = board[row - 1][line];

		if (tmp == before) {
			sameCount++;
			continue;
		}

		if (tmp == before + 1) {
			if (sameCount < lengthOfSlide) {
				isAvailable = false;
				break;
			}
			sameCount = 1;
			continue;
		}

		if (tmp == before - 1) {
			if (isNotValidRange(row + lengthOfSlide - 1)) {
				isAvailable = false;
				break;
			}

			int nextRow = row;
			while (nextRow <= row + lengthOfSlide - 1) {
				if (tmp != board[nextRow][line]) {
					isAvailable = false;
				}
				nextRow++;
			}
			if (isAvailable == false) break;

			row = nextRow - 1;
			sameCount = 0;
			continue;
		}

		//gap is bigger than 2
		isAvailable = false;
		break;
	}
	if (isAvailable) availableRoadCount++;

	/****************** end check col ******************/

	return availableRoadCount;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardSize >> lengthOfSlide;
	board.resize(boardSize, vector<int>(boardSize));

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cin >> board[i][j];
		}
	}

	int answer = 0;
	for (int line = 0; line < boardSize; line++) {
		answer += checkTwoLines(line);
	}
	cout << answer;

	return 0;
}