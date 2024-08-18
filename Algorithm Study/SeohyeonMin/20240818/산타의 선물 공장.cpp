#include <cstdio>
#include <unordered_map>
using namespace std;

typedef long long ll;

const int MAXN = 1e5;
const int MAXM = 10+1;


// Product
typedef struct P {
	int bnum;	// belt number
	int id, w;
	
	int prev;	// prev product id
	int nxt;	// next product id
	P() {}
	P(int _bnum, int _id, int _w) : bnum(_bnum), id(_id), w(_w) {
		prev = -1;
		nxt = -1;
	}
};


// belt
typedef struct B {
	int front_product_id;
	int back_product_id;

	int size;
	bool is_broken;
	int merged_bnum;

	void init() {
		front_product_id = -1;
		back_product_id = -1;

		size = 0;
		is_broken = false;
	}

	void pop_front(unordered_map<int, P>& product_info) {
		if (size == 0) return;

		if (product_info[front_product_id].nxt == -1) {
			product_info.erase(front_product_id);

			front_product_id = -1;
			back_product_id = -1;
			size = 0;
		}
		else {
			int nxt_front = product_info[front_product_id].nxt;
			product_info[nxt_front].prev = -1;
			
			product_info.erase(front_product_id);

			front_product_id = nxt_front;
			size--;
		}
	}

	// update prev, nxt
	void push_back(int new_product_id, unordered_map<int, P>& product_info) {
		if (size == 0) {
			front_product_id = new_product_id;
			back_product_id = new_product_id;
			size++;
			return;
		}
		product_info[new_product_id].prev = back_product_id;
		product_info[new_product_id].nxt = -1;

		product_info[back_product_id].nxt = new_product_id;

		back_product_id = new_product_id;
		size++;
	}
};


int n, m, q;
B belts[MAXM];
// <product id, product>
unordered_map<int, P> product_info;

int get_bnum(P& p) {
	int ret = p.bnum;
	// max depth: 10
	while (belts[ret].is_broken) {
		ret = belts[ret].merged_bnum;
	}
	return ret;
}

// 200
ll put_down_product(int w_max) {
	ll sum = 0;
	for (int bnum = 0; bnum < m; bnum++) {
		if (belts[bnum].is_broken || belts[bnum].size == 0) continue;
		int front_product_id = belts[bnum].front_product_id;

		// 하차
		if (product_info[front_product_id].w <= w_max) {
			sum += product_info[front_product_id].w;
			belts[bnum].pop_front(product_info);
		}
		// 뒤로 이동
		else {
			if (belts[bnum].size == 1) continue;

			int old_front = belts[bnum].front_product_id;
			int new_front = product_info[belts[bnum].front_product_id].nxt;

			int old_back = belts[bnum].back_product_id;
			int new_back = old_front;

			product_info[new_back].nxt = -1;

			product_info[new_back].prev = old_back;
			product_info[old_back].nxt = new_back;
			
			product_info[new_front].prev = -1;

			belts[bnum].front_product_id = new_front;
			belts[bnum].back_product_id = new_back;
		}
	}
	return sum;
}

// 300
int remove_prodcut(int r_id) {
	if (product_info.find(r_id) == product_info.end()) {
		return -1;
	}

	int bnum = get_bnum(product_info[r_id]);
	int prev_product_id = product_info[r_id].prev;
	int nxt_product_id = product_info[r_id].nxt;

	// prev, nxt 둘 다 존재하는 경우
	if (prev_product_id != -1 && nxt_product_id != -1) {
		product_info[prev_product_id].nxt = nxt_product_id;
		product_info[nxt_product_id].prev = prev_product_id;
	}
	// prev만 있는 경우 (r_id가 back인 경우)
	else if (prev_product_id != -1) {
		product_info[prev_product_id].nxt = -1;
		belts[bnum].back_product_id = prev_product_id;
	}
	// nxt만 있는 경우 (r_id가 front인 경우)
	else if(nxt_product_id != -1) {
		product_info[nxt_product_id].prev = -1;
		belts[bnum].front_product_id = nxt_product_id;
	}
	// nxt, prev 둘다 없는 경우 (r_id가 front이자 back인 경우)
	else {
		belts[bnum].front_product_id = -1;
		belts[bnum].back_product_id = -1;
	}

	belts[bnum].size--;
	product_info.erase(r_id);
	
	return r_id;
}

// 400
int find_product(int f_id) {
	if (product_info.find(f_id) == product_info.end()) {
		return -1;
	}

	int bnum = get_bnum(product_info[f_id]);
	int old_front_product_id = belts[bnum].front_product_id;
	int new_front_product_id = f_id;

	int old_back_product_id = belts[bnum].back_product_id;
	int new_back_product_id = product_info[new_front_product_id].prev;

	product_info[old_back_product_id].nxt = old_front_product_id;
	product_info[old_front_product_id].prev = old_back_product_id;

	product_info[new_front_product_id].prev = -1;
	product_info[new_back_product_id].nxt = -1;

	belts[bnum].back_product_id = new_back_product_id;
	belts[bnum].front_product_id = new_front_product_id;
	
	return bnum + 1;
}

// 500
int break_belt(int bnum) {
	if (belts[bnum].is_broken) {
		return -1;
	}

	int target_bnum = 0;
	for (int i = 1; i < m; i++) {
		int bi = (bnum + i) % m;
		if (belts[bi].is_broken) continue;
		target_bnum = bi;
		break;
	}

	if (belts[target_bnum].size == 0 && belts[bnum].size == 0) {
		// nothing to do
	}
	else if (belts[target_bnum].size == 0) {
		belts[target_bnum].front_product_id = belts[bnum].front_product_id;
		belts[target_bnum].back_product_id = belts[bnum].back_product_id;
	}
	else {
		int target_back = belts[target_bnum].back_product_id;
		int broke_front = belts[bnum].front_product_id;
		int broke_back = belts[bnum].back_product_id;

		product_info[target_back].nxt = broke_front;
		product_info[broke_front].prev = target_back;

		belts[target_bnum].back_product_id = broke_back;		
	}

	belts[target_bnum].size += belts[bnum].size;
	belts[bnum].is_broken = true;
	belts[bnum].merged_bnum = target_bnum;
	return bnum + 1;
}


int id[MAXN], w[MAXN];
int main() {
	scanf("%d", &q);

	int cmd;
	while (q--) {
		scanf("%d", &cmd);
		if (cmd == 100) {
			scanf("%d %d", &n, &m);

			for (int i = 0; i < n; i++) {
				scanf("%d", &id[i]);
			}
			for (int i = 0; i < n; i++) {
				scanf("%d", &w[i]);
			}

			for (int i = 0; i < m; i++) {
				belts[i].init();
			}

			for (int i = 0; i < n; i++) {
				// bnum: 0 base
				int bnum = i / (n / m);
				P product = P(bnum, id[i], w[i]);
				product_info[id[i]] = product;
				belts[bnum].push_back(id[i], product_info);
			}
		}
		else if (cmd == 200) {
			int w_max;
			scanf("%d", &w_max);
			
			printf("%lld\n", put_down_product(w_max));
		}
		else if (cmd == 300) {
			int r_id;
			scanf("%d", &r_id);
		
			printf("%d\n", remove_prodcut(r_id));
		}
		else if (cmd == 400) {
			int f_id;
			scanf("%d", &f_id);
			// 벨트 번호
			printf("%d\n", find_product(f_id));
		}
		else if (cmd == 500) {
			int b_num;
			scanf("%d", &b_num);
			// to 0 base
			b_num--;
			// 벨트 번호
			printf("%d\n", break_belt(b_num));
		}
	}
}