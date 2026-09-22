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
const int MOD = 1e9 + 7;
//const int MOD = 998244353;
const double pi = acos(-1.0);
const long double pi_ld = acosl(-1.0L);
#define endl "\n"

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 1000000);

// template<typename T>
// T randint(T l, T r) {
//     static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
//     uniform_int_distribution<T> dis(l, r);
//     return dis(gen);
// }

void init(){
    
}

vector<int> sherwoodQuickSort(vector<int> arr) {

    // 基线条件：数组长度小于等于1时，直接返回（已经有序）
    if (arr.size() <= 1) return arr;

    
    // 1. 舍伍德方法核心：随机选择 pivot
    int pivotIndex = uniform_int_distribution<int>(0, arr.size() - 1)(gen); // 随机生成 pivot 下标
    int pivot = arr[pivotIndex];          // 选取 pivot 元素

    // 定义三个数组用于划分
    vector<int> left;   // 存放小于 pivot 的元素
    vector<int> right;  // 存放大于 pivot 的元素
    vector<int> equal;  // 存放等于 pivot 的元素

    
    // 2. 根据 pivot 对数组进行划分
    for (int x : arr) {
        if (x < pivot)
            left.push_back(x);   // 放入左子数组
        else if (x > pivot)
            right.push_back(x);  // 放入右子数组
        else
            equal.push_back(x);  // 与 pivot 相等
    }

    
    // 3. 递归排序左右子数组
    vector<int> leftSorted = sherwoodQuickSort(left);
    vector<int> rightSorted = sherwoodQuickSort(right);

    
    // 4. 合并结果：左 + 等于 + 右
    vector<int> result;

    result.insert(result.end(), leftSorted.begin(), leftSorted.end());
    result.insert(result.end(), equal.begin(), equal.end());
    result.insert(result.end(), rightSorted.begin(), rightSorted.end());

    return result;
}

void solve(){
    int n;
    cin >> n;
    vector<int> data(n);
    for(int i = 0; i < n; i++){
        cin >> data[i];
    }

    vector<int> sorted = sherwoodQuickSort(data);

    cout << "排序结果：\n";
    for(int x : sorted) cout << x << " ";
    cout << endl;
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int T = 1;
    // cin >> T;
    init();
    while(T--){
        solve();
    }
    return 0;
}