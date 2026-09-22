#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

/*
一维树状数组
静态数组定义在栈内存上容易超限,建议使用动态数组分配堆内存,或者把静态数组放到函数外面
*/

//二维单点修改+区间查询
class TwoDimensionSingleAddIntervalQuery{
public:
    int tree[N][N];
    int nums[N][N];
    //入参二维数组下标从0开始
    //二维树状数组下标从1开始

    inline int lowbit(int x){return x & -x;}

    //单点增加
    void add(int x, int y, int v){
        for(int i = x; i <= N; i += lowbit(x)){
            for(int j = y; j <= N; j += lowbit(y)){
                tree[i][j] += v;
            }
        }
    }

    //单点更新
    //实际二维数组中的位置是(x, y)
    //二维树状数组中的位置是(x + 1, y + 1)
    //单点更新 -> 单点增加(新值 - 老值)
    void update(int x, int y, int v){
        add(x + 1, y + 1, v - nums[x][y]);
        nums[x][y] = v;
    }

    //单点查询
    int query(int x, int y){
        int ans = 0;
        for(int i = x; i > 0; i -= lowbit(x)){
            for(int j = y; j > 0; j -= lowbit(y)){
                ans += tree[i][j];
            }
        }
        return ans;
    }

    //区间查询
    //实际二维数组中的位置是(x, y)
    //二维树状数组中的位置是(x + 1, y + 1)
    int internalquery(int a, int b, int c, int d){
        return query(c + 1, d + 1) - query(c + 1, b) - query(a, d + 1) + query(a, b);
    }
};

//二维区间修改+区间查询
class TwoDimensionIntervalAddIntervalQuery1{
public:
    int n, m;
    int tree1[N][N];
    //维护d[i][j]
    int tree2[N][N];
    //维护d[i][j] * i
    int tree3[N][N];
    //维护d[i][j] * j
    int tree4[N][N];
    //维护d[i][j] * i * j

    inline int lowbit(int x){return x & -x;}

    //单点增加
    void add(int x, int y, int v){
        int v1 = v;
        int v2 = v * x;
        int v3 = v * y;
        int v4 = v * x * y;
        for(int i = x; i <= n; i += lowbit(i)){
            for(int j = y; j <= m; j += lowbit(j)){
                tree1[i][j] += v1;
                tree2[i][j] += v2;
                tree3[i][j] += v3;
                tree4[i][j] += v4;
            }
        }
    }

    //区间增加
    void intervaladd(int a, int b, int c, int d, int v){
        add(a, b, v);
        add(a, d + 1, -v);
        add(c + 1, b, -v);
        add(c + 1, d + 1, v);
    }

    //单点查询
    int query(int x, int y){
        int ans = 0;
        for(int i = x; i > 0; i -= lowbit(i)){
            for(int j = y; j > 0; j -= lowbit(j)){
                ans += (x + 1) * (y + 1) * tree1[i][j] - (y + 1) * tree2[i][j] - (x + 1) * tree3[i][j] + tree4[i][j];
            }
        }
        return ans;
    }

    //区间查询
    int intervalquery(int a, int b, int c, int d){
        return query(c, d) - query(a - 1, d) - query(c, b - 1) + query(a - 1, b - 1);
    }
};