#include<bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
const int N = 1e5 + 5;

// 一维字符串哈希
ull base = 499;
// 433, 499, 599, 31, 131, 1313, 13131, 131313, 1000000007
ull power[N], hashs[N];

void build(string s){
    int n = s.size();
    power[0] = 1;
    for(int i = 1; i < n; i++){
        power[i] = power[i - 1] * base;
    }
    hashs[0] = s[0] - 'a' + 1;
    for(int i = 1; i < n; i++){
        hashs[i] = hashs[i - 1] * base + (s[i] - 'a' + 1);
    }
}

ull Hash(string s){
    ull H = 0;
    int n = s.size();
    for(int i = 0; i < n; i++){
        H = H * base + (s[i] - 'a' + 1);
    }
    return H;
}

// 获得 s 的子串 [l, r] 的哈希值
ull substringHash(int l, int r){
    ull ans = hashs[r];
    if(l > 0)
        ans -= hashs[l - 1] * power[r - l + 1];
    return ans;
}
