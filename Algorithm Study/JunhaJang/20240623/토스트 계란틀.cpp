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

#define EMPTY 0
#define ENEMY 6
#define MARK true
#define ERASE false

enum direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

int minUnreached = INT_MAX;
int boardRow, boardCol;
vector<vector<int>>board;
vector<vector<bool>>reached;
vector<tuple<int, int, int>>pieces; //row, col, kind

direction getLeft(direction dir) {
	if (dir == UP) return LEFT;
	return dir - 1;
}
direction getRight(direction dir) {
	if (dir == LEFT) return UP;
	return dir + 1;
}
direction getBackward(direction dir) {
	if (dir == DOWN) return UP;
	if (dir == LEFT) return RIGHT;
	return dir + 2;
}

pair<int, int> getNextPos(int row, int col, direction dir) {
	switch (dir) {
	case UP:
		row--;
		break;
	case RIGHT:
		col++;
		break;
	case DOWN:
		row++;
		break;
	case LEFT:
		col--;
		break;
	}
	return { row, col };
}

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardRow && 0 <= col && col < boardCol) return true;
	return false;
}

void affect(int row, int col, int kind, direction dir, bool mode) {
	vector<direction>availableDir;
	switch (kind) {
	case 1:
		availableDir.push_back(dir);
		break;
	case 2:
		availableDir.push_back(getLeft(dir));
		availableDir.push_back(getRight(dir);
		break;
	case 3:
		availableDir.push_back(dir);
		availableDir.push_back(getRight(dir);
		break;
	case 4:
		availableDir.push_back(dir);
		availableDir.push_back(getLeft(dir));
		availableDir.push_back(getRight(dir);
		break;
	case 5:
		availableDir.push_back(dir);
		availableDir.push_back(getLeft(dir));
		availableDir.push_back(getRight(dir);
		availableDir.push_back(getBackward(dir));
		break;
	}

	for (direction dir : availableDir) {
		int tmpRow = row;
		int tmpCol = col;

		while (isValidRange(tmpRow, tmpCol)) {
			if (board[tmpRow][tmpCol] == ENEMY) break;

			reached[tmpRow][tmpCol] = mode;

			auto [nextRow, nextCol] = getNextPos(tmpRow, tmpCol, dir);
			tmpRow = nextRow;
			tmpCol = nextCol;
		}
	}
}

void markReachable(int row, int col, int kind, direction dir) {
	affect(row, col, kind, dir, MARK);
}

void eraseReachable(int row, int col, int kind, direction dir) {
	affect(row, col, kind, dir, ERASE);
}

void dfs(int pieceIdx) {
	if (pieceIdx == pieces.size()) {
		int cnt = 0;
		for (int i = 0; i < boardRow; i++) {
			for (int j = 0; j < boardCol; j++) {
				if (reached[i][j]) cnt++;
			}
		}
		int emptyCnt = boardRow * boardCol - cnt;
		minUnreached = min(minUnreachedm, emptyCnt);
		return;
	}

	auto [row, col, kind] = pieces[pieceIdx];
	for (direction dir = UP; dir <= LEFT; dir++) {
		markReachable(row, col, kind, dir);
		dfs(idx + 1);
		eraseReachable(row, col, kind, dir);

		if (kind == 5) return;
	}
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardRow >> boardCol;
	board.resize(boardRow, vector<int>(boardCol));
	reached.resize(boardRow, vector<int>(boardCol, false));
	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			int piece; cin >> board[i][j];
			if (0 < piece && piece < 6) pieces.push_back({i, j, piece});
		}
	}

	dfs(0);

	cout << minUnreached;
	return 0;
}