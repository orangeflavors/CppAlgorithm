#include<bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
const int N = 1e5 + 5;

// 二维字符串哈希
int n, m;// n行m列
vector<vector<char>> a(N, vector<char>(N));// 二维字符串
vector<vector<ull>> hash1(N, vector<ull>(N)), hash2(N, vector<ull>(N));
vector<ull> power1(N, 0), power2(N, 0);
vector<ull> target;// 目标二维字符串的哈希值,可预处理硬编码在程序中
const ull base1 = 13131, base2 = 131313;
// 433, 499, 599, 31, 131, 1313, 13131, 131313, 1000000007

// 预处理二维哈希值
void Hash2D(vector<vector<char>> &a){
    power1[0] = power2[0] = 1;
    for(int i = 1; i <= max(n, m); i++){
        power1[i] = power1[i - 1] * base1;
        power2[i] = power2[i - 1] * base2;
    }

    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            ull val = (ull)a[i][j] + 1007; // +1007 防止负数或小值干扰

            hash1[i][j] = hash1[i][j - 1] * base1 + val;
            hash2[i][j] = hash2[i - 1][j] * base2 + hash1[i][j];
        }
    }
}

// 获得 a 中矩形 [x1, x2] x [y1, y2] 的哈希值
ull get(int x1, int y1, int x2, int y2){
    ull res = hash2[x2][y2]
        - hash2[x1 - 1][y2] * power2[x2 - x1 + 1]
        - (hash2[x2][y1 - 1] - hash2[x1 - 1][y1 - 1] * power2[x2 - x1 + 1]) * power1[y2 - y1 + 1];
    return res;
}