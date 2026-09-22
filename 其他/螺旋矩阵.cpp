#include<bits/stdc++.h>

using namespace std;
using ll = long long;
const int N = 1e5 + 5;
const int inf = 0x3f3f3f3f;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
//const int MOD = 998244353;

//暴力生成螺旋矩阵
//O(n**2)
void solve1(){
    int n, x, y; cin >> n >> x >> y;
    vector<vector<int>> a(n + 1, vector<int>(n + 1));
    int top = 1, bottom = n, left = 1, right = n, cnt = 1;
    while(top <= bottom && left <= right){
        for(int j = left; j <= right; j++){
            a[top][j] = cnt++;
        }
        top++;

        for(int i = top; i <= bottom; i++){
            a[i][right] = cnt++;
        }
        right--;

        for(int j = right; j >= left; j--){
            a[bottom][j] = cnt++;
        }
        bottom--;

        for(int i = bottom; i >= top; i--){
            a[i][left] = cnt++;
        }
        left++;
    }
    // for(int i = 1; i <= n; i++){
    //     for(int j = 1; j <= n; j++){
    //         cout << a[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    cout << a[x][y] << endl;
}

//数学计算
//O(1)
void solve2(){
    int n, x, y; cin >> n >> x >> y;
    x -= 1;//0-base
    y -= 1;
    int k = min(min(x, y), min(n - x - 1, n - y - 1));//层数
    int l = n - 2 * k;//第k层边长
    int start = 1 + 4 * k * (n - k);//第k层左上角起始值
    int offset;//偏移量
    //上边
    if(x == k && k <= y && y <= k + l - 2)
        offset = y - k;
    //右边
    else if(y == k + l - 1 && k + 1 <= y && y <= k + l - 1)
        offset = l - 1 + (x - k);
    //下边
    else if(x == k + l - 1 && k + 1 <= y && y <= k + l - 1)
        offset = 2 * (l - 1) + (k + l - 1 - y);
    //左边
    else if(y == k && k + 1 <= x && x <= k + l - 1)
        offset = 3 * (l - 1) + (k + l - 1 - x);
    //中心
    else
        offset = 0;
    cout << start + offset << endl;
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    // solve1();
    solve2();
    return 0;
}