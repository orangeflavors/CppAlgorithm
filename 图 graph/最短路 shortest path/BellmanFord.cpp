#include <iostream>
#include <vector>
#include <limits.h>
#include <algorithm>

using namespace std;

// 表示图中的边
struct Edge {
    int source;      // 边的起点
    int destination; // 边的终点
    int weight;      // 边的权重
    
    Edge(int s, int d, int w) : source(s), destination(d), weight(w) {}
};

class Graph {
private:
    int V;                     // 顶点数量
    int E;                     // 边数量
    vector<Edge> edges;        // 存储所有边
    vector<int> dist;          // 存储源点到各顶点的最短距离
    vector<int> predecessor;   // 存储最短路径的前驱节点
    
public:
    // 构造函数
    Graph(int vertices) : V(vertices) {
        E = 0;
        dist.resize(V, INT_MAX);
        predecessor.resize(V, -1);
    }
    
    // 添加边
    void addEdge(int source, int destination, int weight) {
        edges.push_back(Edge(source, destination, weight));
        E++;
    }
    
    // Bellman-Ford算法主函数
    // source: 源点
    // 返回值: true表示没有负权环，false表示存在负权环
    bool bellmanFord(int source) {
        // 初始化距离数组
        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            predecessor[i] = -1;
        }
        
        // 源点到自身的距离为0
        dist[source] = 0;
        
        // 进行V-1次松弛操作
        for (int i = 1; i <= V - 1; i++) {
            bool updated = false;
            
            // 遍历所有边
            for (const Edge& edge : edges) {
                int u = edge.source;
                int v = edge.destination;
                int weight = edge.weight;
                
                // 如果u可达，并且通过u到v的距离更短，则更新v的距离
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    predecessor[v] = u;
                    updated = true;
                }
            }
            
            // 如果在某轮迭代中没有更新，可以提前结束
            if (!updated) {
                break;
            }
        }
        
        // 检测负权环：再进行一次松弛操作，如果能继续更新，说明存在负权环
        for (const Edge& edge : edges) {
            int u = edge.source;
            int v = edge.destination;
            int weight = edge.weight;
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                // 存在负权环
                return false;
            }
        }
        
        // 没有负权环
        return true;
    }
    
    // 打印最短路径结果
    void printShortestPaths(int source) {
        cout << "从顶点 " << source << " 到各顶点的最短距离：" << endl;
        for (int i = 0; i < V; i++) {
            cout << "到顶点 " << i << ": ";
            if (dist[i] == INT_MAX) {
                cout << "不可达" << endl;
            } else {
                cout << "距离 = " << dist[i];
                
                // 打印路径
                if (i != source) {
                    cout << ", 路径: ";
                    vector<int> path;
                    int current = i;
                    
                    // 回溯获取路径
                    while (current != -1) {
                        path.push_back(current);
                        current = predecessor[current];
                    }
                    
                    // 反转路径
                    reverse(path.begin(), path.end());
                    
                    // 打印路径
                    for (size_t j = 0; j < path.size(); j++) {
                        cout << path[j];
                        if (j != path.size() - 1) {
                            cout << " -> ";
                        }
                    }
                }
                cout << endl;
            }
        }
    }
    
    // 获取从源点到目标顶点的最短距离
    int getDistance(int destination) {
        return dist[destination];
    }
    
    // 获取从源点到目标顶点的最短路径
    vector<int> getPath(int destination) {
        vector<int> path;
        
        if (dist[destination] == INT_MAX) {
            return path; // 空路径表示不可达
        }
        
        int current = destination;
        while (current != -1) {
            path.push_back(current);
            current = predecessor[current];
        }
        
        reverse(path.begin(), path.end());
        return path;
    }
};

// 示例用法
int main() {
    // 创建一个有5个顶点的图
    Graph graph(5);
    
    // 添加边
    graph.addEdge(0, 1, -1);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 2, 3);
    graph.addEdge(1, 3, 2);
    graph.addEdge(1, 4, 2);
    graph.addEdge(3, 2, 5);
    graph.addEdge(3, 1, 1);
    graph.addEdge(4, 3, -3);
    
    // 从顶点0开始运行Bellman-Ford算法
    int source = 0;
    bool noNegativeCycle = graph.bellmanFord(source);
    
    if (noNegativeCycle) {
        cout << "图中没有从源点可达的负权环" << endl;
        graph.printShortestPaths(source);
        
        // 获取到特定顶点的路径
        int target = 3;
        vector<int> path = graph.getPath(target);
        if (!path.empty()) {
            cout << "\n到顶点 " << target << " 的最短路径: ";
            for (size_t i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i != path.size() - 1) {
                    cout << " -> ";
                }
            }
            cout << ", 距离: " << graph.getDistance(target) << endl;
        }
    } else {
        cout << "图中存在从源点可达的负权环！" << endl;
    }
    
    cout << "\n--- 测试存在负权环的情况 ---" << endl;
    
    // 创建一个有负权环的图
    Graph graphWithCycle(4);
    graphWithCycle.addEdge(0, 1, 1);
    graphWithCycle.addEdge(1, 2, -1);
    graphWithCycle.addEdge(2, 3, -1);
    graphWithCycle.addEdge(3, 1, -1); // 创建负权环
    
    bool noNegativeCycle2 = graphWithCycle.bellmanFord(0);
    if (noNegativeCycle2) {
        cout << "图中没有从源点可达的负权环" << endl;
        graphWithCycle.printShortestPaths(0);
    } else {
        cout << "图中存在从源点可达的负权环！" << endl;
    }
    
    return 0;
}