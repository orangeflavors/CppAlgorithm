#include<bits/stdc++.h>
using namespace std;

/*
最小生成树 Kruskal
需要并查集
1-indexed
*/
template<class T, class DSU>
class MST{
public:
    struct Edge{
        int u, v;
        T w;
    };

    struct Result{
        bool connected;// connected = false 表示原图不连通，返回的是最小生成森林
        T w;// 最小生成树/森林的权值
        vector<Edge> vis;// 最小生成树/森林的边
    };

    int n;// 节点数量
    vector<Edge> edge;// 边集数组
    DSU dsu;// 并查集

    explicit MST(int n): n(n), dsu(n) {}

    // 加无向边
    void add(int u, int v, T w){
        edge.emplace_back(u, v, w);
    }

    // 返回最小生成树/森林
    Result solve() {
        vector<Edge> sorted = edge;
        sort(sorted.begin(), sorted.end(), [](const Edge &x, const Edge &y){return x.w < y.w;});

        dsu.clear();
        T sum = 0;
        vector<Edge> vis;
        vis.reserve(n - 1);
        for(const Edge &e : sorted){
            if(!dsu.merge(e.u, e.v)) continue;
            sum += e.w;
            vis.emplace_back(e);
        }

        return {vis.size() == n - 1, sum, move(vis)};
    }
};

// 最小生成树 Kruskal
// 未封装
using ll = long long;
const int N = 1e5 + 5, M = 2e5 + 5;// 最大顶点数,最大边数

struct Edge{
    int u, v, w;
}edge[M];// 边集数组

bool cmp(Edge a, Edge b){// 对边升序排序
    return a.w < b.w;
}

int ds[N];// 并查集

void init_Set(){//初始化,每个元素以自身为集合
    for(int i = 0; i < N; i++) ds[i] = i;
}

int Find(int x){//查询元素所在集合
    if(x != ds[x]) ds[x] = Find(ds[x]);//路径压缩
    return ds[x];
}

void Union(int x, int y){//合并两个元素的集合
    x = Find(x);
    y = Find(y);
    if(ds[x] != ds[y]) ds[x] = ds[y];
}

int n, m; // 点数,边数
// 时间复杂度O(mlogm)
void Kruskal(){
    sort(edge + 1, edge + m + 1, cmp);// 对边升序排序
    init_Set();// 并查集初始化

    ll ans = 0, cnt = 0;// MST的边权之和,加入MST的边数
    vector<Edge> MST_edges;// 存储最小生成树的边
    for(int i = 1; i <= m ;i++){// 对所有边贪心,最短边一定在MST上
        if(cnt == n - 1) break;// 优化
        auto [u, v, w] = edge[i];
        if(Find(u) != Find(v)){// 判环,不属于同一个集则不会成环
            ans += w;
            Union(u, v);// 合并
            cnt++;// 记录边数
            MST_edges.emplace_back(edge[i]);
        }
    }
    if(cnt == n - 1){
        cout << ans << "\n";
        for(auto [u, v, w] : MST_edges){
            cout << u << " " << v << " " << w << "\n";
        }
    }
    else{
        cout << "not a Connected Component\n";
    }
}