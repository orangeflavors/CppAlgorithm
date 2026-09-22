#include<bits/stdc++.h>
using namespace std;

/*
迭代式线段树
迭代式线段树在时间和空间上与普通线段树相比均具有极大优势

对于维护区间 [1, n]
递归式线段树需要开 4*n 的空间,因为其自上而下按满二叉树组织,最坏情况下最右侧分支深度最大,导致有大量空间未被使用,数组需要开 4n 安全上界保证不会越界

*/
class SegTree{
public:
    int n;// 叶节点数
    vector<int> tree;

    SegTree(int n_): n(n_){
        tree.assign(2 * n, -1);
    }

    void clear(){
        fill(tree.begin(), tree.end(), -1);
    }

    // 单点赋值
    void set(int p, int val){
        p += n - 1;
        tree[p] = val;
        for(p = p >> 1; p != 0; p >>= 1){
            int mx = max(tree[2 * p], tree[2 * p + 1]);
            if(tree[p] == mx) break;// 未变化则提前退出
            tree[p] = mx;
        }
    }

    // 查询区间最大值
    int queryMax(int l, int r){
        l += n - 1; r += n - 1;
        int res = -1;
        while(l <= r){
            if(l & 1){
                res = max(res, tree[l]);
                l++;
            }
            if(!(r & 1)){
                res = max(res, tree[r]);
                r--;
            }
            l >>= 1; r >>= 1;
        }
        return res;
    }
};