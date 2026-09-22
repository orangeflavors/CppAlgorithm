#include<bits/stdc++.h>
using namespace std;

const int N = 1000;// 点的最大数量
vector<pair<int, int>> graph[N];// 邻接表
int n, m;// 点的数量,边的数量

void undirectGraph(){
    for(int i = 0; i <= n; i++) graph[i].clear();
    int u, v, w;
    while(m--){
        cin >> u >> v >> w;
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }
}

void print(){
    for(int i = 1; i <= n; i++){
        for(auto edge: graph[i]){
            cout << i << " " << edge.first << " " << edge.second << "\n";
        }
    }
    cout << endl;
}

int main(){
    cin >> n >> m;
    undirectGraph();
    print();
    return 0;
}
/*测试数据
4 5
1 2 5
1 4 3
2 3 8
2 4 12
3 4 9
*/