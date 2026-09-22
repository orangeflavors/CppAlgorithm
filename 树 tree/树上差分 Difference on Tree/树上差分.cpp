#include<bits/stdc++.h>
using namespace std;

/*
树上差分
离线树上路径加法，支持点差分与边差分
1-indexed
0 为虚拟节点作根节点的父亲
*/
template<class T>
class TreeDiff{
public:
    int n;// 节点数
    int root;// 根节点
    vector<vector<int>> tree;// 邻接表存树

    // 树上倍增求 LCA
    int power;// st表层数[0, power],最多要跳的步数,log2(n) 下取整
    vector<vector<int>> stjump;// st表,st[u][p] 节点 u 的第 2^k 个祖先
    vector<int> deep;// deep[u] 节点 u 的深度
    vector<int> order;// dfs 序

    // 树上差分
    vector<int> fa;// fa[u] 节点 u 的父节点
    vector<T> v_diff;// 点差分数组
    vector<T> e_diff;// 边差分数组

    // 手写 log2(x) 下取整
    inline int log2(unsigned long long x){
        int power = 0;
        while((1ULL << power) <= (x >> 1)){
            power++;
        }
        return power;
    }

    TreeDiff(int n_): n(n_), root(1), 
        tree(n + 1), deep(n + 1){
        power = log2(n);
        stjump.assign(n + 1, vector<int>(power + 1, 0));
    }

    // 加双向边
    void add(int u, int v){
        tree[u].emplace_back(v);
        tree[v].emplace_back(u);
    }

    // dfs 收集信息
    void dfs(int u, int fa_){
        fa[u] = fa_;
        stjump[u][0] = fa_;// u 向上跳 1 步是 fa
        for(int p = 1; p <= power; p++){
            // u 向上跳 2^p 步等价于 u 向上跳 2^(p - 1) 步的点再向上跳 2^(p - 1) 步
            stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
        }
        order.emplace_back(u);

        for(auto v : tree[u]){
            if(v == fa_) continue;
            deep[v] = deep[u] + 1;
            dfs(v, u);
        }
    }

    // 预处理
    // 默认 root = 1
    void work(int rt = 1){
        root = rt;
        fill(deep.begin(), deep.end(), 0);
        fill(fa.begin(), fa.end(), 0);

        deep[root] = 0;
        fa[root] = 0;
        order.clear();
        dfs(root, 0);
    }

    // 返回节点 u 的第 k 个祖先，即向上跳 k 步
    // O(log n)
    int getKthAncestor(int u, int k) const{
        if(k < 0 || k > deep[u])// 越界
            return 0;
        
        for(int bit = 0; bit <= power; bit++){// 对 k 二进制分解
            if((k >> bit) & 1)
                u = stjump[u][bit];
        }
        return u;
    }

    // 返回节点 u, v 的 lca
    // O(log n)
    int getLCA(int u, int v) const{
        if(deep[u] < deep[v]) swap(u, v);// u 作深度大的节点

        // 1.u 跳至与 v 同深度
        u = getKthAncestor(u, deep[u] - deep[v]);

        if(u == v) return u;// 特判 v 是 u 的祖先
        // 2.u,v 同时往上跳
        for(int p = power; p >= 0; p--){// 二进制提升
            if(stjump[u][p] != stjump[v][p]){
                u = stjump[u][p];
                v = stjump[v][p];
            }
        }
        return stjump[u][0];
    }

    // 路径 u 到 v 的所有点权加 d
    void addV(int u, int v, T d){
        int lca = getLCA(u, v);
        v_diff[u] += d;
        v_diff[v] += d;
        v_diff[lca] -= d;
        v_diff[fa[lca]] -= d;
    }

    // 路径 u 到 v 的所有边权加 d
    void addE(int u, int v, T d){
        int lca = getLCA(u, v);
        e_diff[u] += d;
        e_diff[v] += d;
        e_diff[lca] -= d;
        e_diff[lca] -= d;
    }

    vector<T> askV() const {
        vector<T> res(n + 1, T{});
        for(int i = order.size() - 1, u; i >= 0; i++){
            u = order[i];
            res[u] = v_diff[u];
            res[fa[u]] += res[u];
        }
        return res;
    }

    vector<T> askE() const {
        vector<T> res(n + 1, T{});
        for(int i = order.size() - 1, u; i >= 0; i++){
            u = order[i];
            res[u] = e_diff[u];
            res[fa[u]] += res[u];
        }
        res[root] = T{};
        return res;
    }
};