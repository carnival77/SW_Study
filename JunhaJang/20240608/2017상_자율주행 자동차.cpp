#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#define ENDL '\n'

using namespace std;
using ll = long long;

#define CAR_ROAD false
#define PED_ROAD true

enum Direction {
	UP,
	LEFT,
	DOWN,
	RIGHT,
	NONE
};

int boardRow, boardCol;
vector<vector<bool>>board;
vector<vector<bool>>visited;

int turnLeft(int direction) {
	direction++;
	if (direction == NONE) direction = UP;
	return direction;
}

pair<int, int> goBackward(int row, int col, int direction) {
	switch (direction) {
	case UP:
		row++;
		break;
	case LEFT:
		col++;
		break;
	case DOWN:
		row--;
		break;
	case RIGHT:
		col--;
		break;
	default:
		cout << "Direction error!\n";
		exit(-1);
	}
	return { row, col };
}

tuple<int, int, int> getLeftPosAndDirection(int row, int col, int direction) {

	switch (direction) {
	case UP:
		col--;
		break;
	case LEFT:
		row++;
		break;
	case DOWN:
		col++;
		break;
	case RIGHT:
		row--;
		break;
	default:
		cout << "Direction error!\n";
		exit(-1);
	}
	direction = turnLeft(direction);

	return { row, col, direction };
}

void driveCar(int startRow, int startCol, int startDirection) {

	int tmpRow = startRow;
	int tmpCol = startCol;
	int tmpDirection = startDirection;

	int count4 = 4;

	while (1) {
		visited[tmpRow][tmpCol] = true;

		auto [nextRow, nextCol, nextDirection] = getLeftPosAndDirection(tmpRow, tmpCol, tmpDirection);
		if (board[nextRow][nextCol] == CAR_ROAD && visited[nextRow][nextCol] == false) {
			tmpRow = nextRow;
			tmpCol = nextCol;
			tmpDirection = nextDirection;
			count4 = 4;
			continue;
		}
		count4--;
		tmpDirection = turnLeft(tmpDirection);

		if (count4 == 0) {
			auto [nextRow, nextCol] = goBackward(tmpRow, tmpCol, tmpDirection);
			count4 = 4;
			if (board[nextRow][nextCol] == PED_ROAD) {
				break; //gameover
			}
			tmpRow = nextRow;
			tmpCol = nextCol;
		}
	}
}

int main(void) {
	ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	cin >> boardRow >> boardCol;

	int startRow, startCol, startDirection;
	cin >> startRow >> startCol >> startDirection;

	switch (startDirection) {
	case 0:
		startDirection = UP;
		break;
	case 1:
		startDirection = RIGHT;
		break;
	case 2:
		startDirection = DOWN;
		break;
	case 3:
		startDirection = LEFT;
		break;
	default:
		cout << "Direction error!\n";
		exit(-1);
	}

	board.resize(boardRow, vector<bool>(boardCol));
	visited.resize(boardRow, vector<bool>(boardCol, false));

	for (int i = 0; i < boardRow; i++) {
		for (int j = 0; j < boardCol; j++) {
			int n; cin >> n;
			if (n == 1) board[i][j] = PED_ROAD;
			else board[i][j] = CAR_ROAD;
		}
	}

	driveCar(startRow, startCol, startDirection);

	int answer = 0;
	for (auto v1 : visited) {
		for (bool isVisited : v1) {
			if (isVisited) answer++;
		}
	}
	cout << answer;

	return 0;
}