#include <iostream>
#include <vector>
#include <climits>
#include <iomanip>
#include<cassert>

using namespace std;

/*
Floyd-Warshall
求任意两点间最短路，求传递闭包
适用于任何图，不管有向无向，边权正负，但是最短路必须存在（不能有负环）
*/
class FloydWarshall{
public:
    static constexpr int INF = INT_MAX;
    int V;  // 顶点数
    vector<vector<int>> dist;  // 存储最短距离
    vector<vector<int>> next;  // 存储路径信息（可选）

    FloydWarshall(int vertices) : V(vertices){
        dist.assign(V + 1, vector<int>(V + 1, INF));
        next.assign(V + 1, vector<int>(V + 1, -1));
        
        // 初始化对角线为0
        for(int i = 1; i <= V; i++){
            dist[i][i] = 0;
            next[i][i] = i;
        }
    }

    // 加有向带权边
    void addEdge(int u, int v, int weight){
        assert(u >= 1 && u <= V && v >= 1 && v <= V);

        dist[u][v] = min(dist[u][v], weight);
        next[u][v] = v;
    }

    // 执行Floyd-Warshall算法
    void floydWarshall(){
        // 三重循环更新最短路径
        for(int k = 1; k <= V; k++){// k 循环在 i 和 j 之前
            for(int i = 1; i <= V; i++){
                for(int j = 1; j <= V; j++){
                    // 避免溢出：检查INF
                    if(dist[i][k] != INF && dist[k][j] != INF){
                        if(dist[i][j] > dist[i][k] + dist[k][j]){
                            dist[i][j] = dist[i][k] + dist[k][j];
                            next[i][j] = next[i][k];
                        }
                    }
                }
            }
        }
    }

    // 计算传递闭包
    void getClosure(){
        for(int k = 1; k <= V; k++){// k 循环在 i 和 j 之前
            for(int i = 1; i <= V; i++){
                for(int j = 1; j <= V; j++){
                    dist[i][j] |= dist[i][k] & dist[k][j];
                }
            }
        }
    }

    // 打印最短路径矩阵
    void printMatrix(){
        for(int i = 1; i <= V; i++){
            for(int j = 1; j <= V; j++){
                if(dist[i][j] == INF)
                    cout << "INF";
                else
                    cout << dist[i][j];
                cout << " ";
            }
            cout << endl;
        }
    }

    // 重建路径（可选）
    vector<int> reconstructPath(int start, int end){
        vector<int> path;
        
        // 检查是否存在路径
        if(dist[start][end] == INF){
            return path;  // 空路径表示不可达
        }
        
        // 重建路径
        int at = start;
        for(; at != end; at = next[at][end]){
            if(at == -1){
                return vector<int>();  // 路径不存在
            }
            path.push_back(at);
        }
        
        if(next[at][end] == -1){
            return vector<int>();  // 路径不存在
        }
        
        path.push_back(end);
        return path;
    }

    // 打印路径（可选）
    void printPath(int start, int end){
        vector<int> path = reconstructPath(start, end);
        
        if(path.empty()){
            cout << "顶点 " << start << " 到顶点 " << end << " 之间没有路径\n";
            return;
        }
        
        cout << "顶点 " << start << " 到顶点 " << end << " 的最短路径: ";
        for(size_t i = 0; i < path.size(); i++){
            cout << path[i];
            if(i != path.size() - 1){
                cout << " -> ";
            }
        }
        cout << " (距离: " << dist[start][end] << ")" << endl;
    }

    // 检测负权环
    bool hasNegativeCycle(){
        for(int i = 0; i < V; i++){
            if(dist[i][i] < 0){
                return true;
            }
        }
        return false;
    }
};

/*
使用示例
*/
int main() {
    // 创建有4个顶点的图
    FloydWarshall fw(4);

    // 添加边
    fw.addEdge(0, 1, 5);
    fw.addEdge(0, 3, 10);
    fw.addEdge(1, 2, 3);
    fw.addEdge(2, 3, 1);
    fw.addEdge(3, 1, -2);  // 可以添加负权边

    // 执行算法
    fw.floydWarshall();

    // 打印结果
    fw.printMatrix();
    
    cout << "\n";

    // 检测负权环
    if (fw.hasNegativeCycle()) {
        cout << "警告：图中存在负权环！\n";
    } else {
        cout << "图中没有负权环。\n";
    }

    cout << "\n示例路径：\n";
    // 打印一些路径
    fw.printPath(0, 2);
    fw.printPath(0, 3);
    fw.printPath(1, 3);

    return 0;
}