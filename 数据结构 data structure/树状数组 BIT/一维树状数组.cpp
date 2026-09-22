#include<bits/stdc++.h>

using namespace std;
using ll = long long;
const int N = 1e5 + 5;

/*
一维树状数组
静态数组定义在栈内存上容易超限,建议使用动态数组分配堆内存,或者把静态数组放到函数外面
*/

//单点修改+区间查询
/*
树状数组
1-indexed
单点加+区间和
*/
class BinaryIndexedTreeSingleAddIntervalQuery{
public:
    int tree[N];
    inline int lowbit(int x){return x & -x;}
    //单点增加
    void add(int x, int v){
        while(x < N){
            tree[x] += v;
            x += lowbit(x);
        }
    }
    //查询前缀和
    int query(int x){
        int ans = 0;
        while(x > 0){
            ans += tree[x];
            x -= lowbit(x);
        }
        return ans;
    }
    //区间查询
    int intervalquery(int l, int r){
        return query(r) - query(l - 1);
    }
};

//区间修改+单点查询
class BinaryIndexedTreeIntervalAddSingleQuery{
public:
    int a[N], tree[N];
    //树状数组不维护原数组的信息而是维护原数组的差分数组的信息
    inline int lowbit(int x){return x & -x;}
    //单点增加
    void add(int x, int v){
        while(x < N){
            tree[x] += v;
            x += lowbit(x);
        }
    }
    //初始化
    void init(int n){
        a[0] = 0; tree[0] = 0;
        for(int i = 1; i <= n; i++){
            cin >> a[i];
            add(i, a[i] - a[i - 1]);//按差分初始化
        }
    }
    //区间修改
    void intervaladd(int l, int r, int v){
        add(l, v);
        add(r + 1, -v);
    }
    //单点查询
    int query(int x){
        int ans = 0;
        while(x > 0){
            ans += tree[x];
            x -= lowbit(x);
        }
        return ans;
    }
};

//区间修改+区间查询
class BinaryIndexedTreeIntervalAddIntervalQuery{
public:
    int a[N], tree1[N], tree2[N];
    //tree1维护原数组的差分数组的信息Di,tree2维护差分数组的加工信息(i-1)*Di 
    inline int lowbit(int x){return x & -x;}
    //单点增加
    void add(int tree[], int x, int v){
        while(x < N){
            tree[x] += v;
            x += lowbit(x);
        }
    }
    //初始化
    void init(int n){
        a[0] = 0; tree1[0] = 0; tree2[0] = 0;
        for(int i = 1; i <= n; i++){
            cin >> a[i];
            add(tree1, i, a[i] - a[i - 1]);
            add(tree2, i, (i - 1) * (a[i] - a[i - 1]));//按差分初始化
        }
    }
    //区间修改
    void intervaladd(int l, int r, int v){
        add(tree1, l, v);
        add(tree1, r + 1, -v);
        add(tree2, l, (l - 1) * v);
        add(tree2, r + 1, -(r * v));
    }
    //单点查询
    int query(int tree[], int x){
        int ans = 0;
        while(x > 0){
            ans += tree[x];
            x -= lowbit(x);
        }
        return ans;
    }
    //区间查询
    int intervalquery(int l, int r){
        return (r * query(tree1, r) - query(tree2, r)) - ((l - 1) * query(tree1, l - 1) - query(tree2, l - 1));
    }
};
