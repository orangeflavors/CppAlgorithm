#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using PII = pair<int, int>;
using PLL = pair<long, long>;

// 试除法
// O(sqrt(n))
vector<PLL> factorize2(ll n){
    vector<PLL> ans;
    for(ll i = 2; i * i <= n; i++){
        if(n % i == 0){
            ll cnt = 0;
            while(n % i == 0){
                cnt++;
                n /= i;
            }
            ans.emplace_back(i, cnt);
        }
    }
    if(n > 1) ans.emplace_back(n, 1);
    return ans;
}

// 使用欧拉筛
// 预处理O(n), 单次查询O(质因子个数)约为O(logn)
vector<int> prime, minp;

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

vector<array<int, 2>> factorize(int n){
    vector<array<int, 2>> ans;
    while(n > 1){
        int now = minp[n], cnt = 0;
        while(n % now == 0){
            n /= now;
            cnt++;
        }
        ans.emplace_back(now, cnt);
    }
    return ans;
}


// Pollard-Rho启发式方法
// 以单个因子 O(logx) 的复杂度输出 x 的全部质因数
// 未设置失败重试,有待改进

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

ll Pollard_rho(ll n){
    if(!(n & 1)) return 2;
    ll x = 0, y = 0, prod = 1;
    auto f = [&](ll x) -> ll{
        return (mul(x, x, n) + 5) % n;
    };
    for(int t = 30, z = 0; t % 64 || __gcd(prod, n) == 1; t++){
        if(x == y) x = ++z, y = f(x);
        if(ll q = mul(prod, x + n - y, n)) prod = q;
        x = f(x), y = f(f(y));
    }
    return __gcd(prod, n);
}

vector<ll> factorize(ll x){
    vector<ll> ans;
    auto f = [&](auto f, ll x){
        if(x == 1) return;
        if(Miller_Rabin(x)) return ans.emplace_back(x);
        ll y = Pollard_rho(x);
        f(f, y), f(f, x / y);
    };
    f(f, x), sort(ans.begin(), ans.end());
    return ans;
}