#include <cstdio>
#include <vector>
using namespace std;
const int NODE_SIZE = 33;

const int SOURCE = 0;
const int SINK = 21;

int move_cnt[10];
vector<int> g[NODE_SIZE];
int score[NODE_SIZE];

void build_score() {
	for (int i = 1; i <= 20; i++) {
		score[i] = i * 2;
	}
	score[22] = 13;
	score[23] = 16;
	score[24] = 19;

	score[25] = 22;
	score[26] = 24;
	score[27] = 25;

	score[28] = 28;
	score[29] = 27;
	score[30] = 26;

	score[31] = 30;
	score[32] = 35;
}

void build_graph() {
	// 직선 코스
	for (int i = 0; i <= 20; i++) {
		if (i == 5 || i == 10 || i == 15) continue;
		g[i] = vector<int>{ i + 1, min(SINK, i + 2), min(SINK, i + 3), min(SINK, i + 4), min(SINK, i + 5) };
	}

	// 빨간 간선
	g[5] = { 22, 23, 24, 27, 31 };
	g[22] = { 23, 24, 27, 31, 32 };
	g[23] = { 24, 27, 31, 32, 20 };
	g[24] = { 27, 31, 32, 20, 21 };

	g[10] = { 25, 26, 27, 31, 32 };
	g[25] = { 26, 27, 31, 32, 20 };
	g[26] = { 27, 31, 32, 20, 21 };
	
	g[15] = { 28, 29, 30, 27, 31 };
	g[28] = { 29, 30, 27, 31, 32 };
	g[29] = { 30, 27, 31, 32, 20 };
	g[30] = { 27, 31, 32, 20, 21 };
	
	g[27] = { 31, 32, 20, 21, 21 };
	g[31] = { 32, 20, 21, 21, 21 };
	g[32] = { 20, 21, 21, 21, 21 };
}

int brd[NODE_SIZE];
int horses[5];
int ans = -1;

void dfs(int mi, int score_cur) {
	if (mi == 10) {
		ans = max(ans, score_cur);
		return;
	}
	// 모두 도착한 경우
	int sink_cnt = 0;
	for (int hi = 0; hi < 4; hi++) {
		if (horses[hi] == SINK) sink_cnt++;
	}
	if (sink_cnt == 4) {
		ans = max(ans, score_cur);
		return;
	}

	for (int hi = 1; hi <= 4; hi++) {
		if (horses[hi] == SINK) continue;

		int prev = horses[hi];
		int idx_nxt = g[prev][move_cnt[mi] - 1];
		
		// SINK는 겹쳐도 됨
		if (idx_nxt!= SINK && brd[idx_nxt]) continue;

		horses[hi] = idx_nxt;
		brd[prev] = 0;
		brd[idx_nxt] = hi;

		dfs(mi + 1, score_cur + score[idx_nxt]);

		horses[hi] = prev;
		brd[prev] = hi;
		brd[idx_nxt] = 0;
	}
}

int main() {
	for (int i = 0; i < 10; i++) {
		scanf("%d", &move_cnt[i]);
	}

	build_score();
	build_graph();
	dfs(0, 0);
	printf("%d", ans);
}