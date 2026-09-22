#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using i128 = __int128;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;
const int N = 1e5 + 5;
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

void init(){
    
}

void solve(){
    int n;
    cin >> n;

    vector<int> a(n);

    // 初始化牌堆：1 ~ n
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }

    // Fisher-Yates 洗牌
    for (int i = n - 1; i > 0; i--) {
        uniform_int_distribution<int> dis(0, i);
        int j = dis(gen);
        swap(a[i], a[j]);
    }

    cout << "洗牌结果：";
    for (int x : a) {
        cout << x << " ";
    }
    cout << endl;
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