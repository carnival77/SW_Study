#include <cstdio>
#include <vector>
using namespace std;

const int ROW_SIZE = 6;
const int COL_SIZE = 4;

// T1: 한개, T2: 가로, T3: 세로
const int T1 = 1, T2 = 2, T3 = 3;

void put_type1(int x, int y, int bnum, vector<vector<int>>& brd) {
	int row = ROW_SIZE;
	for (int r = 2; r < ROW_SIZE; r++) {
		// 내려가다 블록 마주침
		if (brd[r][y]) {
			row = r;
			break;
		}
	}

	brd[row - 1][y] = bnum;
}

// 가로 (1 x 2)
void put_type2(int x, int y, int bnum, vector<vector<int>>& brd) {
	int row = ROW_SIZE;
	for (int r = 2; r < ROW_SIZE; r++) {
		// 내려가다 블록 마주침
		if (brd[r][y] || brd[r][y+1]) {
			row = r;
			break;
		}
	}

	brd[row - 1][y] = bnum;
	brd[row - 1][y + 1] = bnum;
}

void put_type3(int x, int y, int bnum, vector<vector<int>>& brd) {
	int row = ROW_SIZE;
	for (int r = 2; r < ROW_SIZE; r++) {
		// 내려가다 블록 마주침
		if (brd[r][y]) {
			row = r;
			break;
		}
	}
	brd[row - 2][y] = bnum;
	brd[row - 1][y] = bnum;
}

int get_score(vector<vector<int>>& brd) {
	int score = 0;
	bool chk[ROW_SIZE] = {};

	for (int i = 2; i < ROW_SIZE; i++) {
		int cnt = 0;
		for (int j = 0; j < COL_SIZE; j++) {
			cnt += (brd[i][j] != 0);
		}

		if (cnt == COL_SIZE) {
			chk[i] = true;
			score++;
		}
	}

	int push_cnt = score;
	vector<vector<int>> new_brd;
	for (int i = 0; i < push_cnt; i++) {
		new_brd.push_back(vector<int>(COL_SIZE, 0));
	}

	for (int i = 0; i < ROW_SIZE; i++) {
		if (!chk[i]) {
			new_brd.push_back(brd[i]);
		}
	}

	brd = new_brd;
	return score;
}

void remove_upper(vector<vector<int>>& brd) {
	int pop_cnt = 0;
	for (int i = 0; i < 2; i++) {
		int cnt = 0;
		for (int j = 0; j < COL_SIZE; j++) {
			if (brd[i][j] != 0) cnt++;
		}

		if (cnt > 0) {
			pop_cnt++;
		}
	}

	for (int i = 0; i < pop_cnt; i++) {
		brd.pop_back();
	}

	for (int i = 0; i < pop_cnt; i++) {
		brd.insert(brd.begin(), vector<int>(COL_SIZE, 0));
	}
}

int put_brd(int t, int x, int y, int bnum, vector<vector<int>>& brd) {
	if (t == T1) {
		put_type1(x, y, bnum, brd);
	}
	else if (t == T2) {
		put_type2(x, y, bnum, brd);
	}
	else if (t == T3) {
		put_type3(x, y, bnum, brd);
	}

	int score = get_score(brd);
	remove_upper(brd);

	return score;
}

int main() {
	vector<vector<int>> yellow(ROW_SIZE, vector<int>(COL_SIZE, 0));
	vector<vector<int>> red(ROW_SIZE, vector<int>(COL_SIZE, 0));
	int k;

	scanf("%d", &k);
	int score = 0;
	for (int bnum = 1, t, x, y; bnum <= k; bnum++) {
		scanf("%d %d %d", &t, &x, &y);
		// yellow
		score += put_brd(t, x, y, bnum, yellow);

		// red
		// type1 (x, y) -> type1 (y, 3-x)
		// type2 (x, y) -> type3 (y, 3-x)
		// type3 (x, y) -> type2 (y, 3-x-1)
		int t_red, x_red, y_red;
		if (t == T1) {
			t_red = T1, x_red = y, y_red = 3 - x;
		}
		else if (t == T2) {
			t_red = T3, x_red = y, y_red = 3 - x;
		}
		else {
			t_red = T2, x_red = y, y_red = 3 - x - 1;
		}
		score += put_brd(t_red, x_red, y_red, bnum, red);
	}

	int cnt_remain = 0;
	for (int i = 2; i < ROW_SIZE; i++) {
		for (int j = 0; j < COL_SIZE; j++) {
			cnt_remain += (yellow[i][j] != 0) + (red[i][j] != 0);
		}
	}
	printf("%d\n%d", score, cnt_remain);
}