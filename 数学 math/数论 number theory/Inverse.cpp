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
// const int MOD = 1e9 + 7;
const int MOD = 998244353;

// random_device rd;
// mt19937 gen(rd());
// uniform_int_distribution<int> dis(1, 1000000);

// template<typename T>
// T randint(T l, T r) {
//     static random_device rd;
//     static mt19937 gen(rd());
//     uniform_int_distribution<T> dis(l, r);
//     return dis(gen);
// }

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

ll n, l, r;
string s;
vector<ll> prefix;
ll D;

// 计算无限重复字符串的前k位组成的整数模MOD
ll calPrifix(ll k){
    ll q = k / n;
    ll remain = k % n;

    if(q == 0){
        return prefix[remain];
    }

    ll nqsum = D % MOD * (fastPowMod(10, (q % (MOD - 1) * n) % (MOD - 1), MOD) - 1) % MOD * modInverse(fastPowMod(10, n, MOD) - 1, MOD) % MOD * fastPowMod(10, remain, MOD) % MOD;
    return (nqsum + prefix[remain - 1]) % MOD;
}

void solve(){
    cin >> n >> l >> r;
    cin >> s;
    s = "@" + s;

    prefix.resize(n + 1, 0);
    for(int i = 1; i <= n; i++){// 预处理
        prefix[i] = (prefix[i - 1] * 10 % MOD + (s[i] - '0')) % MOD;
    }

    D = prefix[n];

    ll prefix_l = calPrifix(l - 1);
    ll prefix_r = calPrifix(r);
    ll len = r - l + 1;
    ll ans = (prefix_r - prefix_l * fastPowMod(10, len, MOD) % MOD) % MOD;
    cout << ans << "\n";
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int T = 1;
    // cin >> T;
    while(T--){
        solve();
    }
    return 0;
}