#include<bits/stdc++.h>
using namespace std;

/*
带非负整数边权带非负整数点权树求树的重心
递归版
1-indexed
如果均为正点权则最多只有 2 个重心，如果有 0 点权则可能有多个重心
注意点权必须手动设置
*/
template<class T>
class Tree{
public:
    int n;// 节点数
    int root;// 根节点
    vector<vector<pair<int, T>>> tree;// 邻接表建树，带边权
    vector<T> vertex_w;// 点权
    T vw_sum;// 整棵树所有节点的点权之和
    vector<T> size;// size[u] root 作根节点，以 u 为根的子树大小，即该子树的点权之和
    vector<T> path;// path[u] 从重心到节点 u 的边权之和
    vector<T> maxsub;// maxsub[u] u 的最大子树的大小，即 u 的重量
    vector<int> centroids;// 记录树的重心（存的是节点编号）
    T pathweight;// 从选定的一个重心出发，所有节点到该重心的“带点权距离和”

    Tree(int n_): n(n_), root(1), tree(n + 1), 
        vertex_w(n + 1), vw_sum(0), 
        size(n + 1), path(n + 1), 
        maxsub(n + 1), pathweight(0) {}

    // 加双向带权边，边权默认为 1
    void addE(int u, int v, T w = 1){
        tree[u].emplace_back(v, w);
        tree[v].emplace_back(u, w);
    }

    // 加点权，点权默认为 1
    void addV(int u, T w = 1){
        vertex_w[u] += w;
        vw_sum += w;
    }

    // 设置点权，1-indexed
    void setV(const vector<T> &vw){
        vw_sum = 0;
        for(int u = 1; u <= n; u++){
            vertex_w[u] = vw[u];
            vw_sum += vertex_w[u];
        }
    }

    // 预处理
    void work(int rt = 1){
        root = rt;
        dfs(root, 0);
        findCentroIds();

        // 任选一个重心计算带权路径和
        // 所有重心得到的 pathweight 相同
        pathweight = 0;
        path[centroids[0]] = 0;// 所选重心到自身的距离为 0
        findPathWeight(centroids[0], centroids[0]);
    }

    // dfs 收集信息
    void dfs(int u, int fa){
        size[u] = vertex_w[u];
        maxsub[u] = 0;
        for(auto [v, w] : tree[u]){
            if(v == fa) continue;
            dfs(v, u);
            size[u] += size[v];
            maxsub[u] = max(maxsub[u], size[v]);
        }
        maxsub[u] = max(maxsub[u], vw_sum - size[u]);// u 上方的树也可做 u 的子树
    }

    // 找到树的重心
    void findCentroIds(){
        centroids.clear();
        for(int u = 1; u <= n; u++){
            // 删除 u 后，最大的连通块点权不超过整棵树总点权的一半
            if(maxsub[u] <= vw_sum / 2){
                centroids.emplace_back(u);
            }
        }
    }

    // 从选定重心出发 dfs 计算所有节点到重心的带点权距离和
    void findPathWeight(int u, int fa){
        for(auto [v, w] : tree[u]){
            if(v == fa) continue;
            path[v] = path[u] + w;
            pathweight += path[v] * vertex_w[v];
            findPathWeight(v, u);
        }
    }
};