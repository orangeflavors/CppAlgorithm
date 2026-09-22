#include<bits/stdc++.h>
using namespace std;

struct Node{
    int val, id;// 原值,原下标
};

void solve(){
    vector<int> a;
    // 哈希表
    // 1. 排序去重
    vector<int> sorted = a;
    sort(sorted.begin(), sorted.end());
    sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
    // 2. 建立哈希映射
    unordered_map<int, int> mp;
    for(int i = 0; i < sorted.size(); i++){
        mp[sorted[i]] = i + 1;  // 离散化为 1, 2, 3...
    }

    // 结构体保留原下标
    int n = a.size();
    vector<Node> nodes(n);
    for (int i = 0; i < n; i++) {
        nodes[i] = {a[i], i};
    }
    // 1.按值排序
    sort(nodes.begin(), nodes.end(), [](const Node &a, const Node &b){return a.val < b.val;});
    // 2.离散化,相同值赋予相同编号
    vector<int> res(n);
    int rank = 1;
    for(int i = 0; i < n; i++){
        if (i > 0 && nodes[i].val != nodes[i-1].val) rank++;
        res[nodes[i].id] = rank;
    }
}