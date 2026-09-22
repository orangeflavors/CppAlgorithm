#include<bits/stdc++.h>

using namespace std;
using ll = long long;

// 快速幂
// 计算 base^exp
int fastPow(int base, int exp){
    int ans = 1;
    while(exp > 0){
        if(exp & 1)
            ans *= base;
        base *= base;
        exp >>= 1;
    }
    return ans;
}

// 快速幂取模
// 计算 base^exp % mod
ll fastPowMod(ll base, ll exp, ll mod){
    ll ans = 1 % mod;
    base %= mod;
    while(exp > 0){
        if(exp & 1)
            ans = (ans * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return ans;
}

// 长整型 + 防爆乘法
// 浮点优化乘法,速度快
ll mul(ll a, ll b, ll mod){
    a %= mod, b %= mod;
    ll r = a * b - mod * ll(1.L / mod * a * b);// long double做浮点近似
    return r - mod * (r >= mod) + mod * (r < 0);
}

// (__int128)安全乘法
ll mul(ll a, ll b, ll mod){
    return (__int128)a * b % mod;
}

// 快速幂 + 快速乘
ll fastPowMod2(ll base, ll exp, ll mod){
    ll ans = 1 % mod;
    for(; exp; exp >>= 1, base = mul(base, base, mod)){
        if(exp & 1) ans = mul(ans, base, mod);
    }
    return ans;
}

// 还可考虑使用Barrett Reduction或Montgomery Multiplication进行大整数取模运算