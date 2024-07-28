#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 25;
const int MAXM = 100;
// 우 하 좌 상 순서
const int dr[] = { 0, 1, 0, -1 };
const int dc[] = { 1, 0, -1, 0 };

// vertex
struct V {
	int r, c;
	V() {}
	V(int _r, int _c) : r(_r), c(_c) {}
};

int n, m;
int monster_map[MAXN][MAXN];

// 달팽이 배열 정보
vector<int> snail;
int snail_idx[MAXN][MAXN];

bool is_in_range(int r, int c) {
	return !(r < 0 || r >= n || c < 0 || c >= n);
}


int attack_monster(int d, int p) {
	int score = 0;

	int r_player = n / 2, c_player = n / 2;
	for (int i = 1; i <= p; i++) {
		int nr = r_player + dr[d] * i;
		int nc = c_player + dc[d] * i;

		int sidx = snail_idx[nr][nc];
		// 몬스터 제거
		if (sidx < snail.size()) {
			score += snail[sidx];
			snail[sidx] = 0;
		}
	}

	return score;
}

void remove_empty_space() {
	vector<int> new_snail;
	for (int m : snail) {
		if (m != 0) {
			new_snail.push_back(m);
		}
	}

	snail = new_snail;
}

struct T {
	int num, cnt;
	T() {}
	T(int _n, int _c) : num(_n), cnt(_c) {}
};

int remove_consecutive(int cnt) {
	int score = 0;

	vector<int> new_snail;

	/*
	미로 타워 디펜스 스택 풀이 반례
	snail = {1 1 3 1 1 1 1 1 3 3 3 3 2 3}
	-> 정답: {1 1 3 2 3}
	-> 스택 풀이(오답): { 1 1 2 3 } (snail[2]의 3이 합쳐지면 안되는데 합쳐짐. 연속 4개 삭제하는 로직 최적화 불가. 무한 루프 돌리는 수밖에 없음)
	*/
	// stack
	vector<T> st;
	for (int i = 0; i < snail.size(); i++) {
		if (i == 100) {
			int  yy = 1;
		}
		if (st.empty()) {
			st.push_back(T(snail[i], 1));
			continue;
		}

		T& prev = st.back();
		if (prev.num == snail[i]) {
			prev.cnt++;
			continue;
		}

		if (prev.cnt >= cnt) {
			// 제거
			score += prev.cnt * prev.num;
			st.pop_back();
		}

		if (st.empty()) {
			st.push_back(T(snail[i], 1));
			continue;
		}

		if (st.back().num == snail[i]) {
			st.back().cnt++;
		}
		else {
			st.push_back(T(snail[i], 1));
		}
	}

	if (!st.empty()) {
		T& prev = st.back();
		if (prev.cnt >= cnt) {
			// 제거
			score += prev.cnt * prev.num;
			st.pop_back();
		}
	}

	for (int i = 0; i < st.size(); i++) {
		for (int j = 0; j < st[i].cnt; j++) {
			new_snail.push_back(st[i].num);
		}
	}

	snail = new_snail;
	return score;
}

void convert_to_freq() {
	if (snail.size() == 0) return;

	vector<int> new_snail;

	int cnt_cur = 1;
	int num_cur = snail[0];
	for (int i = 1; i < snail.size(); i++) {
		if (num_cur == snail[i]) {
			cnt_cur++;
		}
		else {
			new_snail.push_back(cnt_cur);
			new_snail.push_back(num_cur);

			cnt_cur = 1;
			num_cur = snail[i];
		}

		if (i == snail.size() - 1) {
			new_snail.push_back(cnt_cur);
			new_snail.push_back(num_cur);
		}
	}
	if (new_snail.size() > n * n - 1) {
		new_snail.erase(new_snail.begin() + n * n - 1, new_snail.end());
	}

	snail = new_snail;
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &monster_map[i][j]);
		}
	}

	// 좌표 flatten
	int r_cur = 0, c_cur = 0;
	int d_cur = 0;	// 오른쪽
	int sidx = n * n - 2;
	vector<V> snail_inverse;
	snail_idx[r_cur][c_cur] = sidx;
	snail_inverse.push_back(V(r_cur, c_cur));

	while (sidx > 0) {
		int r_next = r_cur + dr[d_cur];
		int c_next = c_cur + dc[d_cur];
		if (!is_in_range(r_next, c_next) || snail_idx[r_next][c_next]) {
			d_cur = (d_cur + 1) % 4;
			r_next = r_cur + dr[d_cur];
			c_next = c_cur + dc[d_cur];
		}

		r_cur = r_next, c_cur = c_next;
		sidx--;
		snail_idx[r_cur][c_cur] = sidx;
		snail_inverse.push_back(V(r_cur, c_cur));
	}
	reverse(snail_inverse.begin(), snail_inverse.end());
	for (V& v : snail_inverse) {
		if (!monster_map[v.r][v.c]) continue;
		snail.push_back(monster_map[v.r][v.c]);
	}

	int score = 0;
	for (int round = 0, d, p; round < m; round++) {
		scanf("%d %d", &d, &p);

		// (d, p) 공격
		score += attack_monster(d, p);

		// 빈 공간 제거
		remove_empty_space();

		// 4번 이상 반복된 구간 삭제
		score += remove_consecutive(4);

		// 빈도수 변환
		convert_to_freq();

	}
	printf("%d", score);
}