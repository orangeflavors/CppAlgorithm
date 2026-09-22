#include<bits/stdc++.h>

using namespace std;

/*
全功能并查集
1-indexed
*/
class DSU {
public:
    int n;// 节点数量
    vector<int> parent;// parent[i] : i 的父节点（并查集核心）
    vector<int> compSize;// compSize[i] : 以 i 为根的连通块点数
    vector<int> edgeCount;// edgeCount[i] : 以 i 为根的连通块边数
    vector<int> hasSelfLoop;// hasSelfLoop[i] : 该连通块是否存在自环

    // 1-indexed
    DSU(int n_): n(n_) {
        init(n);
    }

    DSU(){}

    // 初始化
    void init(int n_){
        n = n_;
        parent.assign(n + 1, 0);
        iota(parent.begin(), parent.end(), 0);// 初始化每个点的父亲是自己

        compSize.assign(n + 1, 1);// 初始每个连通块只有一个点
        edgeCount.assign(n + 1, 0);// 初始没有边
        hasSelfLoop.assign(n + 1, 0);// 初始没有自环
    }

    // 清空并查集
    void clear(){
        iota(parent.begin(), parent.end(), 0);
        fill(compSize.begin(), compSize.end(), 1);
        fill(edgeCount.begin(), edgeCount.end(), 0);
        fill(hasSelfLoop.begin(), hasSelfLoop.end(), 0);
    }

    // 查找 x 所在集合的根（路径压缩 + 路径折半）
    int findRoot(int x) {
        while (x != parent[x]) {
            parent[x] = parent[parent[x]]; // 路径压缩（折半优化）
            x = parent[x];
        }
        return x;
    }

    // 合并 x 和 y 所在集合
    // 返回值：
    //   true  -> 成功合并（原本不连通）
    //   false -> 原本已经连通
    bool merge(int x, int y) {
        int rx = findRoot(x);
        int ry = findRoot(y);

        // 每加入一条边，该连通块边数 +1（先记到 rx 上）
        edgeCount[rx]++;

        // 如果已经在同一集合
        if (rx == ry) {
            // x == y 表示自环
            if (x == y) hasSelfLoop[rx] = 1;
            return false;
        }

        // 按编号合并，编号较大的根作为父亲（也可以换成按 size/秩优化）
        if (rx < ry) swap(rx, ry);

        // ry 挂到 rx 上
        parent[ry] = rx;

        // 合并信息
        compSize[rx] += compSize[ry];         // 点数相加
        edgeCount[rx] += edgeCount[ry];       // 边数相加
        hasSelfLoop[rx] |= hasSelfLoop[ry];   // 自环信息合并

        return true;
    }

    // 判断 x 和 y 是否连通
    bool isSame(int x, int y) {
        return findRoot(x) == findRoot(y);
    }

    // 判断 x 所在连通块是否存在自环
    bool hasLoop(int x) {
        return hasSelfLoop[findRoot(x)];
    }

    // 获取 x 所在连通块的点数
    int getSize(int x) {
        return compSize[findRoot(x)];
    }

    // 获取 x 所在连通块的边数
    int getEdgeCount(int x) {
        return edgeCount[findRoot(x)];
    }
};
