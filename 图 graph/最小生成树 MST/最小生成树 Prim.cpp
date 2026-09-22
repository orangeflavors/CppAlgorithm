/*
// 只计算MST的大小
#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using PII = pair<int, int>;

const int N = 5005, M = 2e5 + 5;// 最大顶点数,最大边数

int n, m;// 点数,边数
vector<PII> graph[N];// 邻接表
bool done[N];// done[i] = true点i在MST中

auto cmp = [](PII a, PII b){
    return a.second > b.second;// 小顶堆
};

// 时间复杂度O(mlogn)
void Prim(){
    for(int i = 0; i < N; i++) done[i] = false;// 初始化done
    int s = 1;// 从任意点开始

    ll ans = 0, cnt = 0;// ans: MST的边权之和,cnt: 加入MST的点的数量
    priority_queue<PII, vector<PII>, decltype(cmp)> pq(cmp);// 小顶堆
    pq.push({s, 0});
    while(!pq.empty()){// 对点的最近邻居进行贪心,最近的邻居一定在MST上
        auto [u, w] = pq.top();
        pq.pop();
        if(done[u]) continue;// 判环,若点u在MST中则会成环
        done[u] = true;
        ans += w;
        cnt++;// 记录MST中点的数量
        for(auto[v, dis] : graph[u]){// 将邻居入队
            if(done[v]) continue;
            pq.push({v, dis});
        }
    }
    if(cnt == n){
        cout << ans << "\n";
    }
    else{
        cout << "not a Connected Component\n";
    }
}
*/


// 计算MST的大小并输出MST的边
#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using PII = pair<int, int>;

const int N = 5005, M = 2e5 + 5;// 最大顶点数,最大边数

int n, m;// 点数,边数
vector<PII> graph[N];// 邻接表
bool done[N];// done[i] = true点i在MST中

// 存储边的信息
struct MSTEdge {
    int u, v, w;
};

auto cmp = [](pair<int, PII> a, pair<int, PII> b){
    return a.first > b.first;// 小顶堆，按权重排序
};

// 时间复杂度O(mlogn)
void Prim(){
    for(int i = 0; i < N; i++) done[i] = false;// 初始化done
    int s = 1;// 从任意点开始

    ll ans = 0, cnt = 0;// ans: MST的边权之和,cnt: 加入MST的点的数量
    vector<MSTEdge> mst_edges; // 存储最小生成树的边
    
    // 优先队列存储 (权重, (当前节点, 父节点))
    priority_queue<pair<int, PII>, vector<pair<int, PII>>, decltype(cmp)> pq(cmp);
    pq.push({0, {s, -1}}); // 起始节点没有父节点，用-1表示
    
    while(!pq.empty()){// 对点的最近邻居进行贪心,最近的邻居一定在MST上
        auto [w, nodes] = pq.top();
        auto [u, parent] = nodes;
        pq.pop();
        
        if(done[u]) continue;// 判环,若点u在MST中则会成环
        
        done[u] = true;
        ans += w;
        cnt++;// 记录MST中点的数量
        
        // 如果不是起始节点，记录这条边
        if(parent != -1) {
            mst_edges.push_back({parent, u, w});
        }
        
        for(auto [v, dis] : graph[u]){// 将邻居入队
            if(done[v]) continue;
            pq.push({dis, {v, u}}); // 记录父节点信息
        }
    }
    
    if(cnt == n){
        cout << ans << "\n";
        for(auto [u, v, w] : mst_edges){
            cout << u << " " << v << " " << w << "\n";
        }
    }
    else{
        cout << "not a Connected Component\n";
    }
}