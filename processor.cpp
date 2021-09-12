#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e5 + 5;
map<string, int > id;
string name[maxn];
typedef pair<int, int > pii;
const int INF = 10000;
struct edge {
    int v, a, b, next;
    char c;
    int alr[2], blr[2], lgt;
} e[maxn << 1];
int head[maxn], cnt, idcnt;
int oud[maxn];
void adde(const int &u, const int &v, const char &c, const int &a, const int &b) {
    e[++cnt] = (edge) {v, a, b, head[u], c, -INF, INF, -INF, INF, 0};
    head[u] = cnt;
}

void adde(const int &u, const int &v, const char &c, const int &a, const int &b, int al, int ar, int bl, int br, int lgt) {
    e[++cnt] = (edge) {v, a, b, head[u], c, al, ar, bl, br, lgt};
    head[u] = cnt;
}

char cu[25], cv[25], cchs[25];
void readGraph() {
    freopen("data_ansi.txt", "r", stdin);
    while(1) {
    	int ok = scanf("%s", cu);
    	string u, v, chs;
   		int ui, vi, a, b, al = -INF, ar = INF, bl = -INF, br = INF, l = 0;
    	if(ok == -1) break;
        if(cu[0] != '#') 
			if(cu[0] != '=') scanf("%s%s%d,%d", cchs, cv, &a, &b);
			else scanf("%s%s%s%d,%d%d%d%d%d%d", cu, cchs, cv, &a, &b, &al, &ar, &bl, &br, &l);
        else continue;
        u = cu, v = cv, chs = cchs;
        if(id.count(u)) ui = id[u];
        else {
            ui = id[u] = ++idcnt;
            name[idcnt] = u;
        }
        if(id.count(v)) vi = id[v];
        else {
            vi = id[v] = ++idcnt;
            name[idcnt] = v;
        }
        oud[ui]++;
        adde(ui, vi, chs[0], a, b, al, ar, bl, br, l);
    }
    fclose(stdin);
}

void debug_print_endpoint() {
    for(int i = 1; i <= idcnt; ++i)
        if(oud[i] == 0) 
            printf("%s\n", name[i].c_str());
}


const int B = 42;
int vis[1005][85][85][2];
char prec[1005][85][85][2];
int preu[1005][85][85][2];
int prea[1005][85][85][2];
int preb[1005][85][85][2];
int prel[1005][85][85][2];
void dfs(int u, int a, int b, int l) {
	vis[u][a][b][l] = 1;
	for(int i = head[u]; i; i = e[i].next) {
		if(a - B < e[i].alr[0] || a - B > e[i].alr[1])
			continue;
		if(b - B < e[i].blr[0] || b - B > e[i].blr[1])
			continue;
		if(e[i].lgt > 0 && !l)
			continue;
		int v = e[i].v, na = a + e[i].a, nb = b + e[i].b, nl = 0; // lgt = -1时给一个护符 
		if((e[i].lgt == -1) || l)
			nl = 1;
		if(e[i].lgt == 1 && l == 1)//闪电护符为消耗品 
			nl = 0;
		if(vis[v][na][nb][nl]) continue;
		prec[v][na][nb][nl] = e[i].c; 
		preu[v][na][nb][nl] = u; 
		prea[v][na][nb][nl] = a; 
		preb[v][na][nb][nl] = b; 
		prel[v][na][nb][nl] = l; 
		dfs(v, na, nb, nl);
	}
}

void find_way(int u, int a, int b, int l) {
	if(prec[u][a][b][l]) {
		find_way(preu[u][a][b][l], prea[u][a][b][l], preb[u][a][b][l], prel[u][a][b][l]);
		putchar(prec[u][a][b][l]);
	}
}

void find_way(string u, int a, int b, int l) {
	find_way(id[u], a + B, b + B, l);
}

void debug_endpoint_methods() {
    for(int i = 1; i <= idcnt; ++i)
        if(oud[i] == 0) {
        	printf("###终点：%s\n", name[i].c_str());
        	int icnt = 0;
        	for(int l = 0; l < 2; ++l)
				for(int j = 0; j <= 80; ++j)
					for(int k = 0; k <= 80; ++k) {
						if(!vis[i][j][k][l]) continue;
						int ra = j - B, rb = k - B;
						++icnt;
						if(ra >= 13 && rb >= 13)
						printf("#%d: 自我:%d 爱:%d 护符:%d\n", icnt, ra, rb, l);
						//find_way(i, j, k);
						//putchar('\n');
					}
		}
}

void impl(string s) {
	int now = 1, a = 0, b = 0, l = 0;
	int err = 0;
	for(auto i : s) {
		err = 1;
		for(int j = head[now]; j; j = e[j].next) {
			if(e[j].c == i) {
				l -= e[j].lgt;
				a += e[j].a;
				b += e[j].b;
				now = e[j].v;
				err = 0;
			}
		}
		if(err)
			exit(233);
	}
	printf("位于:%s 自我:%d 爱:%d 护符:%d\n", name[now].c_str(), a, b, l);
}

void debug_keymethods(int endpoint) {
    for(int i = 1; i <= idcnt; ++i)
        if(oud[i] == 0 || !endpoint) {
        	pii a_max, b_max, minmax, maxmin;
        	int al = 0, bl = 0, minmaxl = 0, maxminl = 0;
        	a_max = b_max = minmax = make_pair(-INF, -INF);
        	maxmin = make_pair(INF, INF);
        	printf("###事件：%s\n", name[i].c_str());
        	for(int l = 0; l < 2; ++l)
				for(int j = 0; j <= 80; ++j)
					for(int k = 0; k <= 80; ++k) {
						if(!vis[i][j][k][l]) continue;
						int ra = j - B, rb = k - B;
						if(a_max < make_pair(ra, rb))
							a_max = make_pair(ra, rb), al = l;
						if(b_max < make_pair(rb, ra))
							b_max = make_pair(rb, ra), bl = l;
						if(min(minmax.first, minmax.second) < min(ra, rb))
							if(minmax.first + minmax.second < ra + rb)
								minmax = make_pair(ra, rb), minmaxl = l;
						if(max(maxmin.first, maxmin.second) > max(ra, rb))
							if(maxmin.first + maxmin.second > ra + rb)
								maxmin = make_pair(ra, rb), maxminl = l;
						//find_way(i, j, k);
						//putchar('\n');
					}
			swap(b_max.first, b_max.second);
			printf("自我优先: 自我:%d 爱:%d 护符:%d\n", a_max.first, a_max.second, al);
			find_way(i, a_max.first + B, a_max.second + B, al);
			puts("");
			printf("爱优先: 自我:%d 爱:%d 护符:%d\n", b_max.first, b_max.second, bl);
			find_way(i, b_max.first + B, b_max.second + B, bl);
			puts("");
			printf("最小值最大: 自我:%d 爱:%d 护符:%d\n", minmax.first, minmax.second, minmaxl);
			find_way(i, minmax.first + B, minmax.second + B, minmaxl);
			puts("");
			printf("最大值最小: 自我:%d 爱:%d 护符:%d\n", maxmin.first, maxmin.second, maxminl);
			find_way(i, maxmin.first + B, maxmin.second + B, maxminl);
			puts("");
		}
}

int main() {
    //freopen("ans.txt", "w", stdout);
    readGraph();
    dfs(1, B + 0, B + 0, 0);
    
	int now, a = -10000, b, nl;
    for(int i = 1; i <= idcnt; ++i) {
		for(int l = 0; l < 2; ++l)
			for(int j = 0; j <= 80; ++j)
				for(int k = 0; k <= 80; ++k) {
					if(!vis[i][j][k][l]) continue;
					if(j == B && k == B)
						now = i, a = j, b = k, nl = l;
				}
	}
	find_way(now, a, b, nl);
	impl("DCBACAABDCAABABABBABABABABABAABBBBABBABBABAAAAAAACA");
	//dfs(id["伊甸之门"], B + 2, B + 11);
	//impl("AAAAACAAAA");
	//impl("AAAAACAAAACAAABAAABAAAAB");
	//impl("CAAAACAAAACAAABAAABAAAABABAAADAAABBBBABBAAABAAAAAACAACADB");
	//impl("AAAAACAAAACAAABAAABAAAABABAAADAAABBBBABBA");
	//impl("CAAAACAAAACAAABAAABAAAABABAAADAAABBBBABBAAABAAAAAACABCADB");
	//impl("AAAAACAAAACAAABAAABAAAABABAAADAAABBBBABBAAA");
	//debug_print_endpoint();
	//debug_keymethods(1);
	//debug_endpoint_methods();
	//impl("CAAAACAAAACAAABAAABAAAABABAAADAAABBBBABBAAA");
	//find_way("心海的星空_虫巢之心", 15, 15, 0);
	
	//差值最大 
	/*
	int now, a = 0, b = 0, nl = 0;
	for(int u = 1; u <= idcnt; ++u)
		for(int l = 0; l < 2; ++l)
			for(int i = 0; i <= 80; ++i) {
				for(int j = 0; j <= 80; ++j) {
					if(!vis[u][i][j][l]) continue;
					if(j - i >= b - a)
						a = i, b = j, nl = l, now = u;
				}
			}
	find_way(now, a, b, nl);
	impl("DAAAACCABBBAABAAABAABABAAAAAAAACAAA");
	*/
    return 0;
}
