#include<bits/stdc++.h>

using namespace std;
using ll = long long;

// 试除法
// O(sqrt(n)),适用于 n <= 1e12
bool isPrime(ll n){
    if(n <= 1) return false;
    for(ll i = 2; i * i <= n; i++){
        if(n % i == 0) return false;
    }
    return true;
}

// 素数筛
vector<int> prime, minp;// prime 当前已筛出的质数表,minp[x] x的最小质因子

// 埃氏筛
// O(nloglogn)
vector<int> visit;
void E_sieve(int n = 1e7){
    visit.resize(n + 1, 0);
    for(int i = 2; i * i <= n; i++){
        if(!visit[i])
            for(int j = i * i; j <= n; j += i){
                visit[j] = 1;
                prime.emplace_back(i);
            }
    }
}

// 欧拉筛
// O(n)
// 一个合数肯定有一个最小质因数,让每个合数只被其最小质因数筛一次,避免重复筛
void Euler_sieve(int n = 1e7){
    minp.resize(n + 1, 0);
    for(int i = 2; i <= n; i++){
        if(!minp[i]){// 没有筛过,是素数
            minp[i] = i;
            prime.emplace_back(i);
        }
        for(auto j : prime){// 用已经得到的素数去筛后面的数
            if(j > minp[i] || j > n / i) break;
            // j > minp[i] 控制只用最小质因子生成, j > n / i 防止越界
            minp[i * j] = j;
        }
    }
}

bool isPrime(int n){
    return minp[n] == n;
}

// 素性测试
// Miller-Rabin素性测试
ll mul(ll a, ll b, ll mod){
    a %= mod, b %= mod;
    ll r = a * b - mod * ll(1.L / mod * a * b);// long double做浮点近似
    return r - mod * (r >= mod) + mod * (r < 0);
}

// 快速幂 + 快速乘
ll fastPowMod(ll base, ll exp, ll mod){
    ll ans = 1 % mod;
    for(; exp; exp >>= 1, base = mul(base, base, mod)){
        if(exp & 1) ans = mul(ans, base, mod);
    }
    return ans;
}

// 随机化验证，非严谨计算的平均复杂度约为 O(3.5 * logx)。对于某些强力质数，可能会退化至约 O(35 * logx)。有常数优化版本可以再快五倍。
bool Miller_Rabin(ll n){
    if(n < 2 || n % 6 % 4 != 1){
        return (n | 1) == 3;
    }
    ll s = __builtin_ctzll(n - 1);
    ll d = (n - 1) >> s;
    for(ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}){
        ll p = fastPowMod(a % n, d, n), i = s;
        while(p != 1 && p != n - 1 && a % n && i--){
            p = mul(p, p, n);
        }
        if(p != n - 1 && i != s) return false;
    }
    return true;
}