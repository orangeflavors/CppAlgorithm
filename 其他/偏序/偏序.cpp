#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e5 + 5;


/*
一维偏序
逆序对数量(值域树状数组)
给定一个长度为n的数组arr
如果 i < j 且 arr[i] > arr[j]
那么(i,j)就是一个逆序对
求arr中逆序对的数量
1 <= n <= 5 * 10^5
1 <= arr[i] <= 10^9
模板luoguP1908
*/
class NumberOfReversePair{
public:
    //原数组,下标1~n
    int arr[N];
    //记录原数值离散化后的排名
    int asort[N];
    //求词频数组的前缀和,使用树状数组维护词频数组
    //值域树状数组,下标i是值
    int tree[N];
    //n为原数组长度,m为离散后数值范围
    int n, m;

    //树状数组单点修改+区间查询
    inline int lowbit(int x){return x & -x;}
    //单点增加
    void add(int x, int v){
        while(x < N){
            tree[x] += v;
            x += lowbit(x);
        }
    }
    //查询前缀和
    ll query(int x){
        ll ans = 0;
        while(x > 0){
            ans += tree[x];
            x -= lowbit(x);
        }
        return ans;
    }

    //二分查找原始值v在离散化后数组中的排名
    int get_rank(int v){
        int l = 1, r = m, mid;
        int ans = 0;
        while(l <= r){
            mid = (l + r) / 2;
            if(asort[mid] >= v){
                ans = mid;
                r = mid - 1;
            }
            else l = mid + 1;
        }
        return ans;
    }

    //计算
    ll compute(){
        //离散化
        sort(asort + 1, asort + n + 1);
        m = 1;//离散后数值范围
        //去重,asort的下标为离散后的值,asort的值为离散前的值
        //asort记录原数值的排名
        for(int i = 2; i <= n; i++){
            if(asort[m] != asort[i]){
                asort[++m] = asort[i];
            }
        }
        //将arr转换为离散化后的数值
        for(int i = 1; i <= n; i++){
            arr[i] = get_rank(arr[i]);
        }
        ll ans = 0;
        //倒序处理，当前数字的前一个数的前缀和即为以该数为较大数的逆序对的个数
        for(int i = n; i >= 1; i--){
            //增加当前数字词频
            add(arr[i], 1);
            //查找右边有多少数字<=当前数字-1
            ans += query(arr[i] - 1);
        }
        //也可正序处理，当前已经处理的数字个数减掉当前数字的前缀和即为以该数为较小数的逆序对个数
        // for(int i  =1; i <= n; i++){
        //     add(arr[i], 1);
        //     ans += i - query(arr[i]);
        // }
        return ans;
    }

    void solve(){
        cin >> n;
        for(int i = 1; i <= n; i++){
            cin >> arr[i];
            asort[i] = arr[i];
        }
        cout << compute() << endl;
    }
};