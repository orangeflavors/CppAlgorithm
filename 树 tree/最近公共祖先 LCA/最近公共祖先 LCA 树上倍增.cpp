#include<bits/stdc++.h>
using namespace std;

/*
倍增法求 LCA
1-indexed
当可能为森林时需要对每个连通块单独预处理，遍历每个节点 u，如果 deep[u] == 0 则进行 dfs(u, 0)
*/
class LCA{
public:
    int n;// 节点个数 n
    int power;// st表层数[0, power],最多要跳的步数,log2(n) 下取整
    int root;// 根节点
    vector<vector<int>> tree;// 邻接表存树
    vector<vector<int>> stjump;// st表,st[u][p] 节点 u 的第 2^k 个祖先
    vector<int> deep;// deep[u] 节点 u 的深度

    // 手写 log2(x) 下取整
    inline int log2(unsigned long long x){
        int power = 0;
        while((1ULL << power) <= (x >> 1)){
            power++;
        }
        return power;
    }

    explicit LCA(int n): n(n), root(0), tree(n + 1), deep(n + 1) {
        power = log2(n);
        stjump.assign(n + 1, vector<int>(power + 1, 0));
    }

    // 加双向边
    void add(int u, int v){
        tree[u].emplace_back(v);
        tree[v].emplace_back(u);
    }

    // dfs 收集信息
    void dfs(int u, int fa){
        stjump[u][0] = fa;// u 向上跳 1 步是 fa
        for(int p = 1; p <= power; p++){
            // u 向上跳 2^p 步等价于 u 向上跳 2^(p - 1) 步的点再向上跳 2^(p - 1) 步
            stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
        }

        for(auto v : tree[u]){
            if(v == fa) continue;
            deep[v] = deep[u] + 1;
            dfs(v, u);
        }
    }

    // 预处理
    // 默认 root = 1
    void work(int rt = 1){
        root = rt;
        fill(deep.begin(), deep.end(), 0);

        deep[root] = 0;
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

    // 返回 u, v 间距离，即边数
    int getDis(int u, int v) const{
        // u 到根距离 + v 到根距离 - 2 * lca到根距离
        return deep[u] + deep[v] - 2 * deep[getLCA(u, v)];
    }

    // 返回从 u 到 v 的路径上走第 k 步到达的点
    int getKthPoint(int u, int v, int k) const{
        int lca = getLCA(u, v);
        int l = deep[u] - deep[lca], r = deep[v] - deep[lca];
        if(k < 0 || k > l + r) return 0;// k 范围 [0, l + r]
        // 在 u 一侧从 u 向上跳，在 v 一侧从 v 向上跳
        return k <= l ? getKthAncestor(u, k) : getKthAncestor(v, l + r - k);
    }
};