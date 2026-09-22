#include<bits/stdc++.h>
using namespace std;

/*
整数边权树求树的直径
树形 DP
只能得到直径的长度
代码中允许空路径（单点路径），也就是 dist[u] >= 0
如果不允许则需修改 dist 数组的初始化
*/
template<class T>
class Tree{
public:
    int n;// 节点数
    int root;// 根节点
    vector<vector<pair<int, T>>> tree;// 邻接表建树，带边权
    vector<T> dist;// dist[u] 从 u 开始必须往下走能走出的最大距离，可以不走
    vector<T> dp;// dp[u] 路径必须包含 u 的情况下的最大路径和
    T diameter;// 树的直径

    Tree(int n_): n(n_), root(1), tree(n + 1), dist(n + 1), dp(n + 1) {}

    // 加双向带权边，边权默认为 1
    void add(int u, int v, T w = 1){
        tree[u].emplace_back(v, w);
        tree[v].emplace_back(u, w);
    }

    // 预处理
    void work(int rt = 1){
        root = rt;
        dfs(root, root);
        diameter = dp[1];
        for(int u = 2; u <= n; u++){
            diameter = max(diameter, dp[u]);
        }
    }

    // dfs 收集信息
    void dfs(int u, int fa){
        for(auto [v, w] : tree[u]){
            if(v == fa) continue;
            dfs(v, u);
        }

        for(auto [v, w] : tree[u]){
            if(v == fa) continue;
            // 先更新 dp[u]，再更新 dist[u]
            // 保证 dist[u] 中还没有包含当前儿子 v
            // 所以两条链一定来自不同的儿子，不会把同一条链重复使用
            dp[u] = max(dp[u], dist[u] + dist[v] + w);
            dist[u] = max(dist[u], dist[v] + w);
        }
    }
};