#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

const int INF = 0x3f3f3f3f; // 表示无穷大

/**
 * Dinic算法实现类
 * 用于求解网络最大流问题
 */
class Dinic {
private:
    struct Edge {
        int to;      // 边的终点
        int cap;     // 边的容量
        int flow;    // 边的当前流量
        int rev;     // 反向边在邻接表中的索引
    };

    int n;                              // 节点数量
    vector<vector<Edge>> graph;         // 邻接表表示图
    vector<int> level;                  // 节点的层次（距离源点的最短距离）
    vector<int> iter;                   // 当前弧优化，记录当前遍历到哪条边
    int source, sink;                   // 源点和汇点

    /**
     * BFS构建层次图
     * @return 如果汇点可达返回true，否则返回false
     */
    bool bfs() {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        q.push(source);
        level[source] = 0;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (const Edge& e : graph[u]) {
                if (e.cap > e.flow && level[e.to] < 0) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        
        return level[sink] >= 0;
    }

    /**
     * DFS寻找增广路
     * @param u 当前节点
     * @param flow 当前路径上的最小剩余容量
     * @return 实际增加的流量
     */
    int dfs(int u, int flow) {
        if (u == sink) return flow;
        
        for (int &i = iter[u]; i < graph[u].size(); i++) {
            Edge &e = graph[u][i];
            
            if (e.cap > e.flow && level[u] < level[e.to]) {
                int d = dfs(e.to, min(flow, e.cap - e.flow));
                if (d > 0) {
                    e.flow += d;
                    graph[e.to][e.rev].flow -= d;
                    return d;
                }
            }
        }
        
        return 0;
    }

public:
    /**
     * 构造函数
     * @param n 节点数量（包括源点和汇点）
     * @param s 源点索引
     * @param t 汇点索引
     */
    Dinic(int n, int s, int t) : n(n), source(s), sink(t) {
        graph.resize(n);
        level.resize(n);
        iter.resize(n);
    }

    /**
     * 添加边
     * @param from 边的起点
     * @param to 边的终点
     * @param cap 边的容量
     */
    void addEdge(int from, int to, int cap) {
        // 正向边
        graph[from].push_back({to, cap, 0, (int)graph[to].size()});
        // 反向边，初始容量为0
        graph[to].push_back({from, 0, 0, (int)graph[from].size() - 1});
    }

    /**
     * 计算最大流
     * @return 从源点到汇点的最大流量
     */
    int maxFlow() {
        int flow = 0;
        
        // 不断构建层次图并寻找增广路
        while (bfs()) {
            fill(iter.begin(), iter.end(), 0);
            int f;
            while ((f = dfs(source, INF)) > 0) {
                flow += f;
            }
        }
        
        return flow;
    }

    /**
     * 获取最小割（源点所在集合）
     * @return 布尔向量，true表示节点在源点所在的集合中
     */
    vector<bool> minCut() {
        vector<bool> visited(n, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            
            for (const Edge& e : graph[u]) {
                if (e.cap > e.flow && !visited[e.to]) {
                    visited[e.to] = true;
                    q.push(e.to);
                }
            }
        }
        
        return visited;
    }

    /**
     * 打印当前网络状态（用于调试）
     */
    void printNetwork() {
        cout << "Network State:" << endl;
        for (int u = 0; u < n; u++) {
            for (const Edge& e : graph[u]) {
                if (e.cap > 0) { // 只打印正向边
                    cout << u << " -> " << e.to << " (cap: " << e.cap 
                         << ", flow: " << e.flow << ")" << endl;
                }
            }
        }
    }
};

/**
 * 示例1：简单网络流
 */
void example1() {
    cout << "=== Example 1: Simple Network ===" << endl;
    
    // 创建包含6个节点的图，源点为0，汇点为5
    Dinic dinic(6, 0, 5);
    
    // 添加边
    dinic.addEdge(0, 1, 16);
    dinic.addEdge(0, 2, 13);
    dinic.addEdge(1, 2, 10);
    dinic.addEdge(1, 3, 12);
    dinic.addEdge(2, 1, 4);
    dinic.addEdge(2, 4, 14);
    dinic.addEdge(3, 2, 9);
    dinic.addEdge(3, 5, 20);
    dinic.addEdge(4, 3, 7);
    dinic.addEdge(4, 5, 4);
    
    // 计算最大流
    int maxFlow = dinic.maxFlow();
    cout << "Maximum Flow: " << maxFlow << endl;
    
    // 打印网络状态
    dinic.printNetwork();
    
    // 获取最小割
    vector<bool> cut = dinic.minCut();
    cout << "Minimum Cut (S set): ";
    for (int i = 0; i < cut.size(); i++) {
        if (cut[i]) cout << i << " ";
    }
    cout << endl;
}

/**
 * 示例2：二分图匹配
 */
void example2() {
    cout << "\n=== Example 2: Bipartite Matching ===" << endl;
    
    // 创建二分图匹配的流网络
    // 节点0: 源点
    // 节点1-3: 左边节点
    // 节点4-6: 右边节点
    // 节点7: 汇点
    Dinic dinic(8, 0, 7);
    
    // 连接源点到左边节点
    dinic.addEdge(0, 1, 1);
    dinic.addEdge(0, 2, 1);
    dinic.addEdge(0, 3, 1);
    
    // 连接左边节点到右边节点（表示匹配关系）
    dinic.addEdge(1, 4, 1);
    dinic.addEdge(1, 5, 1);
    dinic.addEdge(2, 5, 1);
    dinic.addEdge(2, 6, 1);
    dinic.addEdge(3, 4, 1);
    dinic.addEdge(3, 6, 1);
    
    // 连接右边节点到汇点
    dinic.addEdge(4, 7, 1);
    dinic.addEdge(5, 7, 1);
    dinic.addEdge(6, 7, 1);
    
    // 计算最大流（即最大匹配数）
    int maxFlow = dinic.maxFlow();
    cout << "Maximum Matching: " << maxFlow << endl;
}

/**
 * 示例3：自定义网络流问题
 */
void example3() {
    cout << "\n=== Example 3: Custom Network ===" << endl;
    
    // 用户自定义网络
    int n = 5;  // 节点数量
    int s = 0;  // 源点
    int t = 4;  // 汇点
    
    Dinic dinic(n, s, t);
    
    // 添加边
    dinic.addEdge(0, 1, 10);
    dinic.addEdge(0, 2, 5);
    dinic.addEdge(1, 2, 15);
    dinic.addEdge(1, 3, 10);
    dinic.addEdge(2, 3, 10);
    dinic.addEdge(3, 4, 10);
    
    int maxFlow = dinic.maxFlow();
    cout << "Maximum Flow: " << maxFlow << endl;
    
    // 打印最终网络状态
    dinic.printNetwork();
}

int main() {
    cout << "Dinic Algorithm for Maximum Flow" << endl;
    cout << "=================================" << endl;
    
    // 运行示例
    example1();
    example2();
    example3();
    
    return 0;
}