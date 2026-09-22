#include<bits/stdc++.h>

using namespace std;

void solve(){
    int n;
    cin >> n;

    // 内向基环树(森林)
    // 每个点出度为 1 的有向图
    // 每个连通块必然含有 1 个有向环和若干棵指向环的树
    vector<int> graph(n + 1, 0), indegree(n + 1, 0), remove(n + 1, 0), in_cycle(n + 1, 0), visit(n + 1, 0), cycle;
    // graph 图, indegree[u] u点入度, remove[u] u点是否是非环节点被移除
    // in_cycle[u] u点在环上, visit[u] u点在查环时有没有被访问过,避免重复, cycle 记录每个环的长度
    for(int u = 1, v; u <= n; u++){
        cin >> v;
        graph[u] = v;
        indegree[v]++;
    }

    // 拓扑删叶子
    queue<int> q;
    for(int u = 1; u <= n; u++){
        if(indegree[u] == 0) q.push(u);
    }
    while(!q.empty()){// 拓扑删去所有非环节点,环节点入度至少为1,不会被删去
        int u = q.front();
        q.pop();
        remove[u] = 1;
        int v = graph[u];
        indegree[v]--;
        if(indegree[v] == 0) q.push(v);
    }

    // 找所有环
    for(int u = 1; u <= n; u++){
        if(remove[u] == 0 && visit[u] == 0){
            int len = 0;
            while(visit[u] == 0){
                visit[u] = 1;
                in_cycle[u] = 1;
                u = graph[u];
                len++;
            }
            cycle.emplace_back(len);
        }
    }

    vector<int> dist(n + 1, -1);// dist[u] u点到环的距离
    queue<int> q2;

    for(int u = 1; u <= n; u++){
        if(in_cycle[u]){
            dist[u] = 0;// 环上点到环的距离为0
            q2.push(u);
        }
    }

    vector<vector<int>> anti_graph(n + 1);
    for(int u = 1; u <= n; u++){
        anti_graph[graph[u]].emplace_back(u);
    }

    while(!q2.empty()){// 反图跑DFS
        int u = q2.front();
        q2.pop();
        for(int v : anti_graph[u]){
            if(dist[v] == -1){
                dist[v] = dist[u] + 1;
                q2.push(v);
            }
        }
    }
}