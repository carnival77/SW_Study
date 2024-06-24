#include <iostream>
#include <queue>
#define ENDL '\n'

using namespace std;
using ll = long long;

typedef struct Virus {
	int row;
	int col;
	int age;
	Virus(int r, int c, int a) : row(r), col(c), age(a) {}
};

struct cmp {
	bool operator() (Virus a, Virus b) {
		return a.age > b.age;
	}
};

int dRow[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dCol[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

int boardSize;
vector<vector<int>>board;
vector<vector<int>>addBoard;

priority_queue<Virus, vector<Virus>, cmp>survive;
queue<Virus>dead;
queue<Virus>newVirus; 
//max case에서 마지막 cycle에 새롭게 생성되는 바이러스 양이 너무 많아서 시간복잡도는 동일함에도 시간초과가 발생
//새롭게 생성되는 바이러스는 priority_queue에 바로 삽입하는 것이 아닌 newVirus queue에 삽입함으로써 해결

bool isValidRange(int row, int col) {
	if (0 <= row && row < boardSize && 0 <= col && col < boardSize) return true;
	return false;
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	int m, cycle;
	cin >> boardSize >> m >> cycle;
	board.resize(boardSize, vector<int>(boardSize, 5));
	addBoard.resize(boardSize, vector<int>(boardSize));

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			cin >> addBoard[i][j];
		}
	}

	for (int i = 0; i < m; i++) {
		int row, col, age;
		cin >> row >> col >> age;
		survive.push(Virus(row - 1, col - 1, age));
	}

	while (cycle-->0) {
		queue<Virus>nextSurvive;
		while (!newVirus.empty()) {
			Virus v = newVirus.front();
			newVirus.pop();

			auto [row, col, age] = v;

			if (age > board[row][col]) {
				dead.push(v);
				continue;
			}

			board[row][col] -= age;
			(v.age)++;
			nextSurvive.push(v);
		}

		while (!survive.empty()) {
			Virus v = survive.top();
			survive.pop();
			
			auto [row, col, age] = v;

			if (age > board[row][col]) {
				dead.push(v);
				continue;
			}

			board[row][col] -= age;
			(v.age)++;
			nextSurvive.push(v);
		}

		while (!dead.empty()) {
			auto [row, col, age] = dead.front();
			dead.pop();
			board[row][col] += age / 2;
		}

		while (!nextSurvive.empty()) {
			Virus v = nextSurvive.front();
			nextSurvive.pop();

			auto [row, col, age] = v;

			if (age % 5 == 0) {
				for (int i = 0; i < 8; i++) {
					int nextRow = row + dRow[i];
					int nextCol = col + dCol[i];
					if (isValidRange(nextRow, nextCol)) {
						newVirus.push(Virus(nextRow, nextCol, 1));
					}
				}
			}
			survive.push(v);
		}

		for (int i = 0; i < boardSize; i++) {
			for (int j = 0; j < boardSize; j++) {
				board[i][j] += addBoard[i][j];
			}
		}
	}

	cout << survive.size() + newVirus.size();
	return 0;
}