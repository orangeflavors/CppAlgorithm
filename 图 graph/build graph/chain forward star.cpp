#include<bits/stdc++.h>
using namespace std;

/*
链式前向星
*/
template<class T>
class ChainForwardStar {
public:
    struct Edge {
        int to;// 去往的点
        T w;// 权值
        int nxt;// 下一条边的编号
    };

    int n;// 点数
    int m;// 最大边数
    int cnt;// 当前边数

    vector<int> head;// head[u]：u 的第一条边
    vector<Edge> edge;// 边数组

    ChainForwardStar(int n, int m)
        : n(n), m(m), cnt(1),
          head(n + 1, -1),
          edge(m + 1) {}

    void init() {
        fill(head.begin(), head.end(), -1);
        cnt = 1;
    }

    // 加单向边
    void addEdge(int u, int v, T w) {
        edge[cnt] = {v, w, head[u]};
        head[u] = cnt++;
    }

    // 返回 u 的第一条边
    int begin(int u) const {
        return head[u];
    }

    // 返回第 id 条边的下一条边
    int next(int id) const {
        return edge[id].nxt;
    }

    // 获取边
    const Edge& getEdge(int id) const {
        return edge[id];
    }

    // 访问指定边
    Edge& getEdge(int id) {
        return edge[id];
    }

    // 当前边数
    int edgeCount() const {
        return cnt - 1;
    }
};


// 链式前向星
// 无封装
// 核心思想:用数组模拟邻接表
// 把所有边按顺序存在一组数组里,并用一个 head[u] 记录 **点 u 的第一条边在数组中的编号**
// 然后每条边再记录“下一条边”的编号,形成一条链
const int N = 1000;// 点的最大数量
const int M = 1000;// 边的最大数量
int head[N];// 头数组,head[u] 表示以 u 为起点的第一条边的下标(在edge[]中的下标)
// head[u] 记录的即为 u 的所有出边,以链表形式存储
struct node{
    int to;// 去往的点
    int w;// 权值
    int nxt;// 下一条边的编号
} edge[M];// 边数组,edge[i]表示第i条边

int n, m, cnt;// n 点的数量, m 边的数量, cnt 当前已经存了多少边

// 初始化
void init(){
    memset(head, -1, sizeof(head));// 将 head 全部设为 -1, 表示没有边
    cnt = 1;// 边的编号 1-base;
}

// 加边
// 模拟链表的头插法
void addEdge(int u, int v, int w){
    edge[cnt].to = v;
    edge[cnt].w = w;
    edge[cnt].nxt = head[u];// 头插法,新边的下一条边为原来的第一条边
    head[u] = cnt;// 新边成为新的第一条边
    cnt++;
}

// 打印有向图
void print(){
    for(int i = 1; i <= n; i++){
        for(int ei = head[i]; ei > 0; ei = edge[ei].nxt){// 遍历点 u 的所有出边
            cout << i << " " << edge[ei].to << " " << edge[ei].w << "\n";
        }
    }
    cout << endl;
}

int main(){
    cin >> n >> m;

    ChainForwardStar<int> graph(n, 2 * m);   // 无向图通常开 2m
    for(int i = 0, u, v, w; i < m; i++){
        graph.addEdge(u, v, w);
        graph.addEdge(v, u, w);
    }
    for(int u = 1; u <= n; u++){
        for (int i = graph.begin(u); i != -1; i = graph.next(i)) {
            auto &e = graph.getEdge(i);

            int v = e.to;
            int w = e.w;
        }
    }
    return 0;
}