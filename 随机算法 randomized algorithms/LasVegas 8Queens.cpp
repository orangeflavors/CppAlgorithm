#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using i128 = __int128;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;

const int inf = 0x3f3f3f3f;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
//const int MOD = 998244353;
const double pi = acos(-1.0);
const long double pi_ld = acosl(-1.0L);
#define endl "\n"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 1000000);

// template<typename T>
// T randint(T l, T r) {
//     static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
//     uniform_int_distribution<T> dis(l, r);
//     return dis(gen);
// }

const int N = 8;
int col[N]; // col[i]表示第i行皇后所在列
// mt19937 rng((unsigned)chrono::steady_clock::now().time_since_epoch().count());

// 判断(row, c)是否安全
bool safe(int row, int c) {
    for (int i = 0; i < row; i++) {
        if (col[i] == c) return false; // 同列
        if (abs(col[i] - c) == abs(i - row)) return false; // 对角线
    }
    return true;
}

// Las Vegas算法
bool LasVegas() {
    memset(col, -1, sizeof(col));

    // 逐行放置皇后
    for (int row = 0; row < N; row++) {
        vector<int> candidates;// 当前行所有合法列

        // 枚举所有列，找可行位置
        for (int c = 0; c < N; c++) {
            if (safe(row, c)) {
                candidates.push_back(c);
            }
        }

        // 无解则失败重来
        if (candidates.empty()) return false;

        // 在合法位置中随机选择一个
        uniform_int_distribution<int> dist(0, candidates.size() - 1);
        col[row] = candidates[dist(gen)];
    }

    return true;
}

// 打印棋盘
void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (col[i] == j) cout << "Q ";
            else cout << ". ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void init(){
    
}

void solve(){
    int cnt = 0;

    while (true) {
        cnt++;
        if (LasVegas()) {
            cout << "成功,尝试次数: " << cnt << "\n";
            printBoard();
            // break;
        }
        if(cnt > 100) break;
    }
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int T = 1;
    // cin >> T;
    init();
    while(T--){
        solve();
    }
    return 0;
}