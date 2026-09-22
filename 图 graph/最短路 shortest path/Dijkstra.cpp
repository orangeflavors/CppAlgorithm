#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// 定义边的结构体
struct Edge {
    int dest;      // 目标节点
    int weight;    // 权重
    Edge(int d, int w) : dest(d), weight(w) {}
};

// 定义图的邻接表表示
typedef vector<vector<Edge>> Graph;

// 用于优先队列的节点结构
struct Node {
    int vertex;    // 顶点
    int distance;  // 距离源点的距离
    
    // 重载 < 运算符，用于最小堆
    bool operator<(const Node& other) const {
        return distance > other.distance;  // 注意：反向比较以实现最小堆
    }
};

class Dijkstra {
private:
    Graph graph;          // 图的邻接表
    int vertices;         // 顶点数量
    
public:
    // 构造函数
    Dijkstra(int n) : vertices(n) {
        graph.resize(n);
    }
    
    // 添加边
    void addEdge(int src, int dest, int weight) {
        // 假设是无向图，添加双向边
        graph[src].push_back(Edge(dest, weight));
        graph[dest].push_back(Edge(src, weight));
    }
    
    // 执行Dijkstra算法
    vector<int> shortestPath(int source) {
        // 初始化距离数组，所有距离初始为无穷大
        vector<int> dist(vertices, INT_MAX);
        
        // 初始化优先队列（最小堆）
        priority_queue<Node> pq;
        
        // 源点距离设为0，并加入优先队列
        dist[source] = 0;
        pq.push({source, 0});
        
        // 主循环
        while (!pq.empty()) {
            // 取出当前距离最小的节点
            Node current = pq.top();
            pq.pop();
            
            int u = current.vertex;
            int currentDist = current.distance;
            
            // 如果当前距离大于已记录的距离，跳过
            if (currentDist > dist[u]) continue;
            
            // 遍历所有邻居
            for (const Edge& edge : graph[u]) {
                int v = edge.dest;
                int weight = edge.weight;
                
                // 松弛操作
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({v, dist[v]});
                }
            }
        }
        
        return dist;
    }
    
    // 输出最短路径结果
    void printShortestPaths(int source, const vector<int>& dist) {
        cout << "从顶点 " << source << " 到各顶点的最短距离：" << endl;
        for (int i = 0; i < vertices; i++) {
            if (dist[i] == INT_MAX) {
                cout << "到顶点 " << i << " : 不可达" << endl;
            } else {
                cout << "到顶点 " << i << " : " << dist[i] << endl;
            }
        }
    }
};

// 示例使用
int main() {
    // 创建一个包含5个顶点的图
    int vertices = 5;
    Dijkstra dijkstra(vertices);
    
    // 添加边
    dijkstra.addEdge(0, 1, 10);
    dijkstra.addEdge(0, 3, 5);
    dijkstra.addEdge(1, 2, 1);
    dijkstra.addEdge(1, 3, 2);
    dijkstra.addEdge(2, 4, 4);
    dijkstra.addEdge(3, 1, 3);
    dijkstra.addEdge(3, 2, 9);
    dijkstra.addEdge(3, 4, 2);
    dijkstra.addEdge(4, 0, 7);
    dijkstra.addEdge(4, 2, 6);
    
    // 计算从顶点0开始的最短路径
    int source = 0;
    vector<int> distances = dijkstra.shortestPath(source);
    
    // 输出结果
    dijkstra.printShortestPaths(source, distances);
    
    // 另一个例子：有向图
    cout << "\n=== 有向图示例 ===" << endl;
    
    // 创建有向图的Dijkstra类变体
    class DirectedDijkstra {
    private:
        Graph graph;
        int vertices;
        
    public:
        DirectedDijkstra(int n) : vertices(n) {
            graph.resize(n);
        }
        
        void addDirectedEdge(int src, int dest, int weight) {
            graph[src].push_back(Edge(dest, weight));
        }
        
        vector<int> shortestPath(int source) {
            vector<int> dist(vertices, INT_MAX);
            priority_queue<Node> pq;
            
            dist[source] = 0;
            pq.push({source, 0});
            
            while (!pq.empty()) {
                Node current = pq.top();
                pq.pop();
                
                int u = current.vertex;
                int currentDist = current.distance;
                
                if (currentDist > dist[u]) continue;
                
                for (const Edge& edge : graph[u]) {
                    int v = edge.dest;
                    int weight = edge.weight;
                    
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                        pq.push({v, dist[v]});
                    }
                }
            }
            
            return dist;
        }
    };
    
    // 使用有向图
    DirectedDijkstra directedDijkstra(6);
    
    directedDijkstra.addDirectedEdge(0, 1, 2);
    directedDijkstra.addDirectedEdge(0, 2, 4);
    directedDijkstra.addDirectedEdge(1, 2, 1);
    directedDijkstra.addDirectedEdge(1, 3, 7);
    directedDijkstra.addDirectedEdge(2, 4, 3);
    directedDijkstra.addDirectedEdge(3, 5, 1);
    directedDijkstra.addDirectedEdge(4, 3, 2);
    directedDijkstra.addDirectedEdge(4, 5, 5);
    
    vector<int> directedDist = directedDijkstra.shortestPath(0);
    
    cout << "从顶点0到各顶点的最短距离：" << endl;
    for (int i = 0; i < 6; i++) {
        if (directedDist[i] == INT_MAX) {
            cout << "到顶点 " << i << " : 不可达" << endl;
        } else {
            cout << "到顶点 " << i << " : " << directedDist[i] << endl;
        }
    }
    
    return 0;
}