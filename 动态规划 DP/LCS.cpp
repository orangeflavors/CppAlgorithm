#include<bits/stdc++.h>

using namespace std;
using ll = long long;
const int N = 1e5 + 5;
const int inf = 0x3f3f3f3f;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
//const int MOD = 998244353;

//DP
//O(n**2)
void solve1(){
    int n; cin >> n;
    vector<int> x(n), y(n);
    for(int i = 0; i < n; i++) cin >> x[i];
    for(int i = 0; i < n; i++) cin >> y[i];

    vector<vector<int>> dp(n + 1, vector<int>(n + 1, 0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(x[i] == y[j])
                dp[i + 1][j + 1] = dp[i][j] + 1;
            else
                dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
        }
    }
    cout << dp[n][n] << endl;
}

//DP滚动数组
//O(n**2)
void solve2(){
    int n; cin >> n;
    vector<int> x(n), y(n);
    for(int i = 0; i < n; i++) cin >> x[i];
    for(int i = 0; i < n; i++) cin >> y[i];

    vector<vector<int>> dp(2, vector<int>(n + 1, 0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(x[i] == y[j])
                dp[(i + 1) % 2][j + 1] = dp[i % 2][j] + 1;
            else
                dp[(i + 1) % 2][j + 1] = max(dp[i % 2][j + 1], dp[(i + 1) % 2][j]);
        }
    }
    cout << dp[n % 2][n] << endl;
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    solve2();
    return 0;
}