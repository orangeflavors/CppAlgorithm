#include<bits/stdc++.h>
using namespace std;

/*
tarjan + 并查集求 LCA
离线查询
work() 执行Tarjan，如需多次work需清空数组
*/
template<class DSU>
class LCA{
public:
    int n;// 节点数
    int q;// 查询数
    vector<vector<int>> tree;// 邻接表建树
    vector<vector<pair<int, int>>> query;// query[u] = {v, id} 询问 (u, v) 编号为 id
    vector<int> ans;// 询问的答案
    vector<int> ancestor;// ancestor[find(x)] 当前并查集集合所对应的 Tarjan 祖先
    vector<int> visited;// 0：尚未访问，1：已经进入 DFS，2：已经完成 DFS
    DSU dsu;// 并查集

    // 树及查询均为 1-indexed
    explicit LCA(int n_): n(n_), q(0), tree(n + 1), query(n + 1), dsu(n){
        ans.clear();
        ancestor.assign(n + 1, 0);
        visited.assign(n + 1, 0);
    }

    // 加双向边
    void addEdge(int u, int v){
        tree[u].emplace_back(v);
        tree[v].emplace_back(u);
    }

    // 加询问，返回该询问的 id，1-indexed
    int addQuery(int u, int v){
        q++;
        int id = q;
        ans.emplace_back(0);
        query[u].emplace_back(v, id);
        query[v].emplace_back(u, id);
        return id;
    }

    void dfs(int u, int fa){
        visited[u] = 1;// DFS 进入 u
        ancestor[dsu.findRoot(u)] = u;// 当前集合的祖先就是自己
        for(auto v : tree[u]){// 枚举儿子
            if(v == fa) continue;
            dfs(v, u);
            // v 的整棵子树处理完成后，把 v 所在集合并入 u 所在集合
            // 注意此处合并时 u 所在集合的祖先应该设置为 u
            dsu.merge(u, v);
            ancestor[dsu.findRoot(u)] = u;
        }
        visited[u] = 2;// u 的 DFS 已经完成

        // 处理所有与 u 有关的询问
        for(auto [v, id] : query[u]){
            // 只有当 v 已经 DFS 完成后，才能确定 LCA(u, v)
            // 此时 ancestor[find(v)] 就是 LCA(u, v)
            if(visited[v] == 2){
                ans[id] = ancestor[dsu.findRoot(v)];
            }
        }
    }

    // 执行Tarjan
    void work(int rt = 1){
        // fill(ancestor.begin(), ancestor.end(), 0);
        // fill(visited.begin(), visited.end(), 0);
        // dsu.init(n);

        dfs(rt, 0);
    }

    // 获取全部答案
    const vector<int>& getAnswers() const {
        return ans;
    }
};