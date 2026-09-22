#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

class Graph {
private:
    int V; // 顶点数
    vector<vector<int>> adj; // 邻接表
    
public:
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
    }
    
    // 添加有向边 u->v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }
    
    // 方法1：Kahn算法（基于队列的BFS方法）
    vector<int> topologicalSortKahn() {
        vector<int> inDegree(V, 0); // 存储每个顶点的入度
        vector<int> result; // 存储拓扑排序结果
        
        // 计算所有顶点的入度
        for (int u = 0; u < V; u++) {
            for (int v : adj[u]) {
                inDegree[v]++;
            }
        }
        
        // 将所有入度为0的顶点加入队列
        queue<int> q;
        for (int i = 0; i < V; i++) {
            if (inDegree[i] == 0) {
                q.push(i);
            }
        }
        
        // 处理队列中的顶点
        int count = 0; // 记录已处理的顶点数
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            result.push_back(u);
            count++;
            
            // 减少所有邻接顶点的入度
            for (int v : adj[u]) {
                inDegree[v]--;
                if (inDegree[v] == 0) {
                    q.push(v);
                }
            }
        }
        
        // 检查是否存在环
        if (count != V) {
            cout << "图中存在环，无法进行拓扑排序！" << endl;
            return vector<int>(); // 返回空向量表示失败
        }
        
        return result;
    }
    
    // 方法2：基于DFS的拓扑排序
    vector<int> topologicalSortDFS() {
        vector<int> result;
        vector<bool> visited(V, false);
        vector<bool> inStack(V, false); // 用于检测环
        stack<int> st;
        
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                if (!dfs(i, visited, inStack, st)) {
                    cout << "图中存在环，无法进行拓扑排序！" << endl;
                    return vector<int>();
                }
            }
        }
        
        // 将栈中的元素弹出得到拓扑排序
        while (!st.empty()) {
            result.push_back(st.top());
            st.pop();
        }
        
        return result;
    }
    
private:
    // DFS辅助函数
    bool dfs(int u, vector<bool>& visited, vector<bool>& inStack, stack<int>& st) {
        visited[u] = true;
        inStack[u] = true; // 将当前节点加入递归栈
        
        // 递归访问所有邻接顶点
        for (int v : adj[u]) {
            if (!visited[v]) {
                if (!dfs(v, visited, inStack, st)) {
                    return false;
                }
            } else if (inStack[v]) {
                // 如果遇到已经在递归栈中的顶点，说明存在环
                return false;
            }
        }
        
        inStack[u] = false; // 从递归栈中移除当前节点
        st.push(u); // 将当前节点压入栈
        return true;
    }
};

// 测试函数
int main() {
    // 创建有向图
    Graph g(6);
    g.addEdge(5, 2);
    g.addEdge(5, 0);
    g.addEdge(4, 0);
    g.addEdge(4, 1);
    g.addEdge(2, 3);
    g.addEdge(3, 1);
    
    cout << "使用Kahn算法得到的拓扑排序: ";
    vector<int> result1 = g.topologicalSortKahn();
    for (int v : result1) {
        cout << v << " ";
    }
    cout << endl;
    
    cout << "使用DFS算法得到的拓扑排序: ";
    vector<int> result2 = g.topologicalSortDFS();
    for (int v : result2) {
        cout << v << " ";
    }
    cout << endl;
    
    // 测试有环图
    Graph g2(3);
    g2.addEdge(0, 1);
    g2.addEdge(1, 2);
    g2.addEdge(2, 0); // 形成环
    
    cout << "\n测试有环图:" << endl;
    vector<int> result3 = g2.topologicalSortKahn();
    if (result3.empty()) {
        cout << "检测到环，拓扑排序失败" << endl;
    }
    
    return 0;
}