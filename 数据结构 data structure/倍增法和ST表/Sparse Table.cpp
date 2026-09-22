#include<bits/stdc++.h>
using namespace std;
using ll = long long;

void solve(){
    int n, q;
    cin >> n >> q;
    vector<ll> a(n + 1, 0);
    for(int i = 1; i <= n; i++){
        cin >> a[i];
    }

    // ST表解决RMQ
    // ST表适用于解决静态数据
    // 预处理ST表 O(nlogn)
    int power = floor(log2(n)) + 1;
    vector<vector<ll>> stmax(n + 1, vector<ll>(power, 0)), stmin(n + 1, vector<ll>(power, 0)),
    stgcd(n + 1, vector<ll>(power, 0));
    for(int i = 1; i <= n; i++){
        stmax[i][0] = a[i];
        stmin[i][0] = a[i];
        stgcd[i][0] = a[i];
    }
    for(int p = 1; p < power; p++){
        for(int i = 1; i + (1 << p) - 1 <= n; i++){
            stmax[i][p] = max(stmax[i][p - 1], stmax[i + (1 << (p - 1))][p - 1]);
            stmin[i][p] = min(stmin[i][p - 1], stmin[i + (1 << (p - 1))][p - 1]);
            stgcd[i][p] = __gcd(stgcd[i][p - 1], stgcd[i + (1 << (p - 1))][p - 1]);
        }
    }

    // 查询 O(1)
    for(int i = 0; i < q; i++){
        ll l, r;
        cin >> l >> r;
        int p = floor(log2(r - l + 1));
        ll ma = max(stmax[l][p], stmax[r - (1 << p) + 1][p]);
        ll mi = min(stmin[l][p], stmin[r - (1 << p) + 1][p]);
        ll g = __gcd(stgcd[l][p], stgcd[r - (1 << p) + 1][p]);
    }
}