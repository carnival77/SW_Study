#include <cstdio>

struct S {
    int num;
    int priv[4];
    S() {}
    S(int n0, int n1, int n2, int n3, int n4) {
        num = n0;
        priv[0] = n1;
        priv[1] = n2;
        priv[2] = n3;
        priv[3] = n4;
    }
};

const int MAXN = 20;
// 상 좌 우 하 순서
const int dr[] = { -1, 0, 0, 1 };
const int dc[] = { 0, -1, 1, 0 };
const int score_unit[] = { 0, 1, 10, 100, 1000 };

int n;
// 0 base
int brd[MAXN][MAXN];
// 1 base
S students[MAXN * MAXN + 1];

bool is_in_range(int r, int c) {
    if (r < 0 || r >= n || c < 0 || c >= n) return false;
    return true;
}

bool is_in_priv(int priv[4], int pnum) {
    bool ret = false;
    for (int i = 0; i < 4; i++) {
        ret |= (priv[i] == pnum);
    }
    return ret;
}

void put_proper_place(int sidx) {
    int r = -1, c = -1;

    int cnt_max = -1;
    int cnt_empty_max = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (brd[i][j]) continue;

            int cnt_priv = 0;
            int cnt_empty = 0;
            for (int d = 0; d < 4; d++) {
                int nr = i + dr[d];
                int nc = j + dc[d];
                if (!is_in_range(nr, nc)) continue;
                if (!brd[nr][nc]) {
                    cnt_empty++;
                }
                else if (is_in_priv(students[sidx].priv, students[brd[nr][nc]].num)) {
                    cnt_priv++;
                }
            }
            // 좋아하는 친구의 수가 많은 위치 선택
            if (cnt_max < cnt_priv) {
                cnt_max = cnt_priv;
                cnt_empty_max = cnt_empty;
                r = i, c = j;
            }
            else if (cnt_max == cnt_priv) {
                // 비어있는 칸이 많은 곳 선택
                if (cnt_empty_max < cnt_empty) {
                    cnt_empty_max = cnt_empty;
                    r = i, c = j;
                }
            }
        }
    }

    brd[r][c] = sidx;
}


int main() {
    scanf("%d", &n);
    int buf[5];
    for (int i = 1; i <= n * n; i++) {
        for (int j = 0; j < 5; j++) {
            scanf("%d", &buf[j]);
        }
        students[i] = S(buf[0], buf[1], buf[2], buf[3], buf[4]);
    }

    for (int i = 1; i <= n * n; i++) {
        put_proper_place(i);
    }

    int score = 0;
    // 점수 계산
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sidx = brd[i][j];
            int cnt_priv = 0;

            for (int d = 0; d < 4; d++) {
                int nr = i + dr[d];
                int nc = j + dc[d];
                if (!is_in_range(nr, nc)) continue;
                if (is_in_priv(students[sidx].priv, students[brd[nr][nc]].num)) {
                    cnt_priv++;
                }
            }
            score += score_unit[cnt_priv];
        }
    }
    printf("%d", score);

    return 0;
}