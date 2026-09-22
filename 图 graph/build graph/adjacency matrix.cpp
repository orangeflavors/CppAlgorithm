#include<bits/stdc++.h>
using namespace std;

const int N = 500;//点的最大数量
int graph[N][N];//邻接矩阵
int n, m;//点的数量,边的数量

void undirectGraph(){
    for(int i = 1; i <= n; i++)
        for(int j = 1; j <= n; j++)
            graph[i][j] = 0;
    int u, v, w;
    while(m--){
        cin >> u >> v >> w;
        graph[u][v] = w;
        graph[v][u] = w;
    }
}

int main(){
    cin >> n >> m;
    undirectGraph();
    return 0;
}