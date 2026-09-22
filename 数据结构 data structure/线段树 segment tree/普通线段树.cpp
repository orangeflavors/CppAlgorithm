#include<bits/stdc++.h>
using namespace std;

// 普通线段树
/*
实现区间加,区间赋值
查询区间和,区间最大值,区间最小值
作为计数线段树时可查询全局第 k 小
该线段树实现功能多且使用递归式写法,递归调用的函数开销,build递归访问所有4*n个节点,处理懒标记和pushdown,结构体字段访问等开销巨大
在对时间复杂度要求高,常数优化要求高的情况下不利
使用时可只实现需要功能,对于需要多次清空重建的情况可以一次性建多棵线段树,用空间换时间
或者直接使用迭代式线段树
*/
template<class T>
class SegTree{
public:
    struct node{
        int l, r;// 该节点代表的区间
        T sum;// 区间和
        T mx;// 区间最大值
        T mn;// 区间最小值
        T add_lazy;// 区间加懒标记
        T set_lazy;// 区间赋值懒标记
        bool has_set;// 是否被赋值
    };

    int n;// 线段树规模
    vector<T> data;// 原始数据,1-indexed
    vector<node> tree;// 线段树节点数组,1-indexed,1为根节点
    
    SegTree(): n(0){};

    // 区间 [1, n]
    SegTree(int n_): n(n_){
        init();
    }

    // 接收一个 1-indexed 数组建树
    SegTree(const vector<T> &arr){
        n = arr.size() - 1;
        data.resize(n + 1);
        for(int i = 0; i <= n; i++)
            data[i] = arr[i];
        init();
    }

    inline int ls(int cur){return (cur << 1);}
    inline int rs(int cur){return (cur << 1) + 1;}

    // 初始化
    void init(){
        data.resize(n + 1);
        tree.resize(4 * n + 1);
        build(1, n);
    }

    // 清空数据,不改变空间
    void clear(){
        fill(data.begin(), data.end(), T(0));// 清零原始数据
        build(1, n);// 重建线段树
    }

    // 递归建树
    void build(int l, int r, int cur = 1){
        if(l == r){
            tree[cur] = {l, r, data[l], data[l], data[l], 0, 0, false};
            return;
        }
        tree[cur] = {l, r, 0, 0, 0, 0, 0, false};
        int mid = (l + r) >> 1;
        build(l, mid, ls(cur));
        build(mid + 1, r, rs(cur));
        pushup(cur);
    }

    // 上传
    void pushup(int cur){
        int left = ls(cur), right = rs(cur);
        tree[cur].sum = tree[left].sum + tree[right].sum;
        tree[cur].mx = max(tree[left].mx, tree[right].mx);
        tree[cur].mn = min(tree[left].mn, tree[right].mn);
    }

    // 为 cur 添加赋值懒标记并更新节点信息
    void applySet(int cur, T val){
        tree[cur].sum = (tree[cur].r - tree[cur].l + 1) * val;
        tree[cur].mx = val;
        tree[cur].mn = val;
        tree[cur].set_lazy = val;// 叶子节点理应不该有lazy tag,不过不影响结果
        tree[cur].has_set = true;
        tree[cur].add_lazy = 0;// 清除旧加法
    }

    // 为 cur 添加区间和懒标记并更新节点信息
    void applyAdd(int cur, T lazy){
        tree[cur].sum += (tree[cur].r - tree[cur].l + 1) * lazy;
        tree[cur].mx += lazy;
        tree[cur].mn += lazy;
        tree[cur].add_lazy += lazy;// 叶子节点理应不该有lazy tag,不过不影响结果
    }

    // 下传
    // 将 cur 的懒标记下传到子节点
    void pushdown(int cur){
        if(tree[cur].has_set){// 先传赋值再传加法,覆盖操作会重置状态,加法操作在覆盖之后进行
            applySet(ls(cur), tree[cur].set_lazy);
            applySet(rs(cur), tree[cur].set_lazy);
            tree[cur].has_set = false;
        }
        if(tree[cur].add_lazy != 0){// 0 做加法哨兵
            applyAdd(ls(cur), tree[cur].add_lazy);
            applyAdd(rs(cur), tree[cur].add_lazy);
            tree[cur].add_lazy = 0;
        }
    }

    // 区间加法
    // 区间 [l, r] 加 val 当前处理的节点为 cur
    void add(int l, int r, T val, int cur = 1){
        if(l <= tree[cur].l && tree[cur].r <= r){
            applyAdd(cur, val);
            return;
        }
        pushdown(cur);// 下传懒标记
        int mid = (tree[cur].l + tree[cur].r) >> 1;
        if(l <= mid) add(l, r, val, ls(cur));
        if(mid < r) add(l, r, val, rs(cur));
        pushup(cur);// 上传更新
    }

    // 区间赋值
    // 区间 [l, r] 设为 val 当前处理的节点为 cur
    void set(int l, int r, T val, int cur = 1){
        if(l <= tree[cur].l && tree[cur].r <= r){
            applySet(cur, val);
            return;
        }
        pushdown(cur);// 下传懒标记
        int mid = (tree[cur].l + tree[cur].r) >> 1;
        if(l <= mid) set(l, r, val, ls(cur));
        if(mid < r) set(l, r, val, rs(cur));
        pushup(cur);// 上传更新
    }

    // 查询区间和
    // 返回区间 [l, r] 的区间和
    T querySum(int l, int r, int cur = 1){
        if(l <= tree[cur].l && tree[cur].r <= r)
            return tree[cur].sum;

        pushdown(cur);// 下传懒标记
        int mid = (tree[cur].l + tree[cur].r) >> 1;
        T ans = 0;
        if(l <= mid) ans += querySum(l, r, ls(cur));
        if(mid < r) ans += querySum(l, r, rs(cur));
        return ans;// 查询不用上传更新
    }

    // 查询区间最大值
    // 返回区间 [l, r] 的区间最大值
    T queryMax(int l, int r, int cur = 1){
        if(l <= tree[cur].l && tree[cur].r <= r)
            return tree[cur].mx;

        pushdown(cur);// 下传懒标记
        int mid = (tree[cur].l + tree[cur].r) >> 1;
        T ans = numeric_limits<T>::min();
        if(l <= mid) ans = max(ans, queryMax(l, r, ls(cur)));
        if(mid < r) ans = max(ans, queryMax(l, r, rs(cur)));
        return ans;
    }

    // 查询区间最小值
    // 返回区间 [l, r] 的区间最小值
    T queryMin(int l, int r, int cur = 1){
        if(l <= tree[cur].l && tree[cur].r <= r)
            return tree[cur].mn;

        pushdown(cur);// 下传懒标记
        int mid = (tree[cur].l + tree[cur].r) >> 1;
        T ans = numeric_limits<T>::max();
        if(l <= mid) ans = min(ans, queryMin(l, r, ls(cur)));
        if(mid < r) ans = min(ans, queryMin(l, r, rs(cur)));
        return ans;
    }

    // 全局第 k 小(1-indexed)
    // 仅当线段树为计数线段树,即每个叶子节点存储的是该值出现多少次的计数时能够工作
    T kth(int k, int cur = 1){
        if(tree[cur].l == tree[cur].r) return tree[cur].l;
        pushdown(cur);
        if(tree[ls(cur)].sum >= k)
            return kth(k, ls(cur));
        else
            return kth(k - tree[ls(cur)].sum, rs(cur));
    }

    // 输出所有节点
    void debug(int cur = 1){
        cout << "[" << tree[cur].l << ", " << tree[cur].r << "]: ";
        cout << "sum = " << tree[cur].sum << ", ";
        cout << "max = " << tree[cur].mx << ", ";
        cout << "min = " << tree[cur].mn << ", ";
        cout << "add_lazy = " << tree[cur].add_lazy << ", ";
        cout << "set_lazy = " << tree[cur].set_lazy << ", ";
        cout << "has_set = " << tree[cur].has_set << ", ";
        cout << endl;
        if (tree[cur].l == tree[cur].r) return;
        debug(ls(cur)), debug(rs(cur));
    }
};
