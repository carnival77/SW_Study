#include <cstdio>
char brd[4][9];

void rotate_clockwise(char* chair) {
	char tmp = chair[7];
	for (int i = 7; i > 0; i--) {
		chair[i] = chair[i-1];
	}
	chair[0] = tmp;
}

void rotate_counter_clockwise(char* chair) {
	char tmp = chair[0];
	for (int i = 0; i < 7; i++) {
		chair[i] = chair[i + 1];
	}
	chair[7] = tmp;
}

void rotate(int i, int d) {
	if (d == -1) rotate_counter_clockwise(brd[i]);
	else rotate_clockwise(brd[i]);
}

// brd[i]를 d방향으로 회전
void rotate_leftside(int i, int d) {
	if (i <= -1) return;

	if (brd[i][2] != brd[i + 1][6]) {
		rotate_leftside(i - 1, -d);
		rotate(i, d);
	}
}

void rotate_rightside(int i, int d) {
	if (i >= 4) return;
	
	if (brd[i-1][2] != brd[i][6]) {
		rotate_rightside(i + 1, -d);
		rotate(i, d);
	}
}

int main() {
	for (int i = 0; i < 4; i++) {
		scanf("%s", brd[i]);
	}

	int k;
	scanf("%d", &k);
	for (int i = 0, n, d; i < k; i++) {
		// 반시계: -1, 시계: 1
		scanf("%d %d", &n, &d);
		n--;
		rotate_leftside(n - 1, -d);
		rotate_rightside(n + 1, -d);
		rotate(n, d);
	}

	int ans = 0;
	for (int i = 3; i >= 0; i--) {
		if (brd[i][0] == '1') ans++;
		if (i != 0) ans <<= 1;
	}
	printf("%d", ans);
}