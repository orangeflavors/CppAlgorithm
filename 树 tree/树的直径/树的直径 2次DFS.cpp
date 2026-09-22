#include<bits/stdc++.h>
using namespace std;

/*
非负整数边权树求树的直径
两次 DFS
1-indexed
得到直径长度，直径上所有点
*/
template<class T>
class Tree{
public:
    int n;// 节点数
    int root;// 根节点
    vector<vector<pair<int, T>>> tree;// 邻接表建树，带边权
    vector<T> dist;// dist[u] 当前根节点到 u 的距离
    vector<int> last;// last[u] 当前根节点到 u 的路径上 u 节点的上一个节点
    T diameter;// 树的直径
    int start, end;// 直径的端点

    Tree(int n_): n(n_), root(1), tree(n + 1), dist(n + 1), last(n + 1) {}

    // 加双向带权边，边权默认为 1
    void add(int u, int v, T w = 1){
        tree[u].emplace_back(v, w);
        tree[v].emplace_back(u, w);
    }

    // 预处理
    void work(int rt = 1){
        root = rt;
        start = 1;
        dist[root] = 0;
        last[root] = root;
        dfs(root, root);
        for(int u = 2; u <= n; u++){
            if(dist[u] > dist[start]){
                start = u;
            }
        }

        dist[start] = 0;
        last[start] = start;
        end = 1;
        dfs(start, start);
        for(int u = 2; u <= n; u++){
            if(dist[u] > dist[end]){
                end = u;
            }
        }
        diameter = dist[end];
    }

    // dfs 收集信息
    void dfs(int u, int fa){
        for(auto [v, w] : tree[u]){
            if(v == fa) continue;
            last[v] = u;
            dist[v] = dist[u] + w;
        }
    }
};
