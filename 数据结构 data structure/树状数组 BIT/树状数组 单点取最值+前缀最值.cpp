#include<vector>
#include<cassert>
using namespace std;

/*
树状数组
1-indexed
单点取最大值，查询前缀最大值
*/
template<class T>
class BIT{
public:
    int n;
    T default_val;
    vector<T> tree;
    inline int lowbit(int x){return x & -x;}

    BIT() = default;
    BIT(int n_){
        init(n_);
    }

    void init(int n_, T val = numeric_limits<T>::lowest()){
        n = n_;
        default_val = val;
        tree.assign(n + 1, val);
    }

    // 单点取最大值
    void modify(int x, T v){
        assert(1 <= x && x <= n);
        for(; x <= n; x += lowbit(x)){
            tree[x] = max(tree[x], v);
        }
    }

    // 查询前缀最大值 [1, r]
    int queryMax(int r){
        assert(0 <= r && r <= n);

        T ans = default_val;

        for(; r > 0; r -= lowbit(r)){
            ans = max(ans, tree[r]);
        }

        return ans;
    }
};
