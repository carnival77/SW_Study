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
int dCol[4] = { 0, 0, 1, -1 };

enum direction {
	UP = 1,
	DOWN,
	RIGHT,
	LEFT
};

struct V {
	int row;
	int col;
	int speed;
	int dir;
	int size;
	V() { row = 0; col = 0; speed = 0; dir = 0; size = 0; }

	V(int _row, int _col, int _speed, int _dir, int _size) 
		: row(_row), col(_col), speed(_speed), dir(_dir), size(_size) {}
};

int boardRow, boardCol;
int numOfVirus;
int answer = 0;
vector<vector<int>>board; //index of virus
vector<V>virusVec;
vector<bool>catched;

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardRow && 0 <= col && col < boardCol) return true;
	return false;
}

void getVirus(int col) {
	for (int row = 0; row < boardRow; row++) {
		int virusIdx = board[row][col];
		if (virusIdx != -1) {
			answer += virusVec[virusIdx].size;
			board[row][col] = -1;
			catched[virusIdx] = true;
			break;
		}
	}
}

int flipDir(int dir) {
	switch (dir) {
	case UP: 
		return DOWN;
	case DOWN:
		return UP;
	case RIGHT:
		return LEFT;
	case LEFT:
		return RIGHT;
	default:
		cout << "direction error!";
		exit(-1);
	}
}

tuple<int, int ,int> move(int row, int col, int speed, int dir) {
	int nextRow = row;
	int nextCol = col;
	int nextDir = dir;
	if (dir == 4) {
		int here = 3;
	}
	for (int step = 0; step < speed; step++) {
		nextRow = nextRow + dRow[nextDir - 1];
		nextCol = nextCol + dCol[nextDir - 1];
		if (isValidRange(nextRow, nextCol)) continue;

		nextRow -= dRow[nextDir - 1];
		nextCol -= dCol[nextDir - 1];
		nextDir = flipDir(nextDir);
		step--;
	}

	return { nextRow, nextCol, nextDir };
}

void moveVirus() {
	vector<vector<int>>newBoard(boardRow, vector<int>(boardCol, -1));
	for (int idx = 0; idx < virusVec.size(); idx++) {
		if (catched[idx]) continue;
		V virus = virusVec[idx];
		auto [nextRow, nextCol, nextDir] = move(virus.row, virus.col, virus.speed, virus.dir);

		board[virus.row][virus.col] = -1;
		virusVec[idx] = V(nextRow, nextCol, virus.speed, nextDir, virus.size);

		if (newBoard[nextRow][nextCol] != -1) { //바이러스가 마주칠 경우
			int alreadyExistVirusIdx = newBoard[nextRow][nextCol];
			if (virusVec[idx].size < virusVec[alreadyExistVirusIdx].size) {
				catched[idx] = true;
			}
			else {
				catched[alreadyExistVirusIdx] = true;
				newBoard[nextRow][nextCol] = idx;
			}
			continue;
		}

		newBoard[nextRow][nextCol] = idx;
	}

	board = newBoard;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardRow >> boardCol >> numOfVirus;
	board.resize(boardRow, vector<int>(boardCol, -1));
	virusVec.resize(numOfVirus);
	catched.resize(numOfVirus, false);

	for (int i = 0; i < numOfVirus; i++) {
		int row, col, speed, dir, size;
		cin >> row >> col >> speed >> dir >> size;
		row--; col--;
		virusVec[i] = V(row, col, speed, dir, size);
		board[row][col] = i;
	}

	for (int checkCol = 0; checkCol < boardCol; checkCol++) {
		getVirus(checkCol);
		moveVirus();
	}

	cout << answer;

	return 0;
}