#include<bits/stdc++.h>
using namespace std;

// 更完整的实现，包含原始图容量的保存
class AdvancedGraph {
    int V;
    vector<vector<int>> capacity; // 原始容量
    vector<vector<int>> residual; // 残量图
    
public:
    AdvancedGraph(int vertices) : V(vertices) {
        capacity.resize(V, vector<int>(V, 0));
        residual.resize(V, vector<int>(V, 0));
    }
    
    void addEdge(int u, int v, int cap) {
        capacity[u][v] = cap;
        residual[u][v] = cap;
    }
    
    bool bfs(int s, int t, vector<int>& parent) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(s);
        visited[s] = true;
        parent[s] = -1;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v = 0; v < V; v++) {
                if (!visited[v] && residual[u][v] > 0) {
                    if (v == t) {
                        parent[v] = u;
                        return true;
                    }
                    q.push(v);
                    parent[v] = u;
                    visited[v] = true;
                }
            }
        }
        return false;
    }
    
    int fordFulkerson(int source, int sink) {
        int max_flow = 0;
        vector<int> parent(V);
        
        while (bfs(source, sink, parent)) {
            int path_flow = INT_MAX;
            
            // 找到路径上的最小残量
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                path_flow = min(path_flow, residual[u][v]);
            }
            
            // 更新残量图
            for (int v = sink; v != source; v = parent[v]) {
                int u = parent[v];
                residual[u][v] -= path_flow;
                residual[v][u] += path_flow;
            }
            
            max_flow += path_flow;
        }
        
        return max_flow;
    }
    
    void findMinCut(int source) {
        vector<bool> visited(V, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (int v = 0; v < V; v++) {
                if (!visited[v] && residual[u][v] > 0) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        
        cout << "最小割：" << endl;
        cout << "源点侧集合 S: ";
        for (int i = 0; i < V; i++) {
            if (visited[i]) {
                cout << i << " ";
            }
        }
        cout << endl;
        
        cout << "汇点侧集合 T: ";
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                cout << i << " ";
            }
        }
        cout << endl;
        
        cout << "割边：" << endl;
        int cut_capacity = 0;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (visited[i] && !visited[j] && capacity[i][j] > 0) {
                    cout << i << " -> " << j << " (容量: " << capacity[i][j] << ")" << endl;
                    cut_capacity += capacity[i][j];
                }
            }
        }
        cout << "最小割容量: " << cut_capacity << endl;
    }
};

int main() {
    AdvancedGraph ag(8);
    ag.addEdge(0, 1, 8);
    ag.addEdge(0, 2, 7);
    ag.addEdge(0, 3, 4);
    ag.addEdge(1, 2, 2);
    ag.addEdge(2, 3, 5);
    ag.addEdge(1, 4, 3);
    ag.addEdge(1, 5, 9);
    ag.addEdge(2, 5, 6);
    ag.addEdge(3, 5, 7);
    ag.addEdge(3, 6, 2);
    ag.addEdge(5, 4, 3);
    ag.addEdge(5, 6, 4);
    ag.addEdge(4, 7, 9);
    ag.addEdge(5, 7, 5);
    ag.addEdge(6, 7, 8);
    
    int source = 0, sink = 7;
    int max_flow = ag.fordFulkerson(source, sink);
    cout << "最大流: " << max_flow << endl;
    ag.findMinCut(source);
    
    return 0;
}