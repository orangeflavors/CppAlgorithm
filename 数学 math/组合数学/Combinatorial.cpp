#include <bits/stdc++.h>

using namespace std;
using ll = long long;
const int MOD = 1e9 + 7;

// 直接利用公式计算（适合 n 较小）
ll C(ll n, ll m){
    if(m > n)
        return 0;

    m = min(m, n - m); // 利用组合数对称性
    ll ans = 1;

    for(ll i = 1; i <= m; i++){
        ans = ans * (n - i + 1) / i;
    }

    return ans;
}


// 动态规划计算组合数
ll c[1005][1005];
void init(){
    int n;
    // 初始化
    for(int i = 0; i <= n; i++){
        c[i][0] = 1;
        c[i][i] = 1;
    }

    // 杨辉三角递推
    for(int i = 1; i <= n; i++){
        for(int j = 1; j < i; j++){
            c[i][j] = (c[i - 1][j - 1] + c[i - 1][j]) % MOD;
        }
    }
}


// 预处理阶乘 + 逆元
const int N = 1e6 + 5;
ll fact[N]; // factorial 阶乘

// 预处理
void getFact(){
    fact[0] = 1;
    for (int i = 1; i < N; i++)
    {
        fact[i] = fact[i - 1] * i % MOD;
    }
}

// 快速幂取模
// 计算 base^exp % mod
ll fastPowMod(ll base, ll exp, ll mod){
    ll ans = 1;
    base %= mod;
    while(exp > 0){
        if(exp & 1)
            ans = (ans * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return ans;
}

// 费马小定理求单个逆
ll modInverse(ll a, ll mod){
    return fastPowMod(a, mod - 2, mod);
}

// 求组合数 C_n^m
ll Comb(ll n, ll m, ll mod){
    if(m < 0 || m > n) return 0;
    return fact[n] 
    * modInverse(fact[m], mod) % mod 
    * modInverse(fact[n - m], mod) % mod;
}