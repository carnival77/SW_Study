#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include <tuple>
#define ENDL '\n'

using namespace std;
using ll = long long;

#define FIRE 2
#define WALL 1
#define EMPTY 0

int dRow[4] = { -1, 1, 0, 0 };
int dCol[4] = { 0, 0, -1, 1 };

int boardRow, boardCol;
int numOfOriginalWalls;
vector<vector<int>>board;
vector<pair<int, int>>emptyVec;
vector<pair<int, int>>fireVec;
vector<bool>choose3EmptyPos;

int minFires = INT_MAX;

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardRow && 0 <= col && col < boardCol) return true;
	return false;
}

void bfs() {
	queue<pair<int, int>>q;
	vector<vector<bool>>fireVisited(boardRow, vector<bool>(boardCol, false));
	for (pair<int, int> fire : fireVec) {
		q.push(fire);
	}

	while (!q.empty()) {
		auto [row, col] = q.front();
		q.pop();
		if (fireVisited[row][col]) continue;
		fireVisited[row][col] = true;

		for (int i = 0; i < 4; i++) {
			int nextRow = row + dRow[i];
			int nextCol = col + dCol[i];
			if (isValidRange(nextRow, nextCol) && board[nextRow][nextCol] == EMPTY) {
				if (fireVisited[nextRow][nextCol] == false) {
					q.push({ nextRow, nextCol });
				}
			}
		}
	}

	int cnt = 0;
	for (auto v : fireVisited) {
		for (bool isFireVisited : v) {
			if (isFireVisited) cnt++;
		}
	}

	minFires = min(minFires, cnt);
}

void dfs_to_choose_3Pos_and_run_bfs(int idx, int cnt) {

	if (cnt == 3) {
		vector<pair<int, int>>chosen;
		for (int i = 0; i < idx; i++) {
			if (choose3EmptyPos[i]) chosen.push_back(emptyVec[i]);
		}
		if (chosen.size() != 3) {
			cout << "choose Error!" << ENDL;
			exit(-1);
		}

		for (auto [row, col] : chosen) {
			board[row][col] = WALL;
		}

		bfs();

		for (auto [row, col] : chosen) {
			board[row][col] = EMPTY;
		}

		return;
	}

	if (idx == emptyVec.size()) return;

	choose3EmptyPos[idx] = true;
	dfs_to_choose_3Pos_and_run_bfs(idx + 1, cnt + 1);
	choose3EmptyPos[idx] = false;
	dfs_to_choose_3Pos_and_run_bfs(idx + 1, cnt);
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardRow >> boardCol;
	board.resize(boardRow, vector<int>(boardCol));

	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			cin >> board[i][j];
			if (board[i][j] == EMPTY) {
				emptyVec.push_back({ i, j });
			}
			if (board[i][j] == FIRE) {
				fireVec.push_back({ i, j });
			}
			if (board[i][j] == WALL) {
				numOfOriginalWalls++;
			}
		}
	}
	choose3EmptyPos.resize(emptyVec.size(), false);

	dfs_to_choose_3Pos_and_run_bfs(0, 0);

	int answer = boardRow * boardCol - numOfOriginalWalls - 3 - minFires;

	cout << answer;

	return 0;
}