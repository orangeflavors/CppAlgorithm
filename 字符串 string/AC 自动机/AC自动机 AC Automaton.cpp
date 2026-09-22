#include<bits/stdc++.h>
#include<cassert>
using namespace std;

/*
AC自动机
多模匹配
接口
    init() 初始化
    insert(s) 插入模式串 s 返回该模式串终点对应的节点编号
    work() 预处理
    getCnt(s) 统计文章中每个状态对应字符串的出现次数
    matchAny(s) 判断文本中是否出现任意模式串
复杂度
设 L = 所有模式串总长度，S = 文章长度，V = AC 自动机节点数，M = 字符集大小
总体复杂度O(L + VM + S + V)
*/
template<int M = 26>
class ACAM{
public:
    // M 字符集大小，默认为 26 个小写字母
    int n;// 最大节点编号，初始 n = 1，节点数量为 n + 1
    int root;// 0 超级根，1 真正的 Trie 根
    vector<array<int, M>> tree;// trie 结构,tree[i][c] 节点 i 的字符 c 指向哪个节点,0 表示空节点
    vector<int> fail;// fail 指针，表示当前节点对应字符串的最长真后缀对应的状态
    vector<int> order;// BFS序
    vector<int> len;// len[u] 节点 u 对应的字符串长度，同时也是 Trie 的深度
    vector<int> cnt;// cnt[u] 节点 u 的词频，汇总后表示节点 u 对应字符串的实际出现次数
    vector<int> end;// end[u] = 1 节点 u 是模式串的终点
    vector<int> alert;// alert[u] = 1 节点 u 是模式串终点或 fail 链上存在模式串终点
    bool ok;// 是否已预处理过

    // 字符集大小默认为 26
    ACAM(): root(1){
        init();
    }

    // 初始化
    void init(){
        n = 1;
        tree.assign(2, array<int, M>{});// 初始创建两个节点 0 = 超级根 1 = 真正根
        tree[0].fill(1);// 超级根 0 的所有字符都指向真正根 1，使得根节点相关的 fail 转移能够统一处理
        fail.assign(2, 0);// 0,1 的 fail 指针均为 0
        len.assign(2, 0);
        len[0] = -1;// 真正根的儿子长度可以统一写成 len[child] = len[parent] + 1
        cnt.assign(2, 0);
        end.assign(2, 0);
        alert.assign(2, 0);
        order.clear();
        ok = false;
    }

    // 将字符集映射为 [0, M) 的编号
    int getPath(char c) const {
        int x = c - 'A';
        assert(0 <= x && x < M);
        return x;
    }

    // 新建节点，返回节点编号
    int node(){
        n++;
        int u = n;
        tree.emplace_back();
        fail.emplace_back(0);
        len.emplace_back(0);
        cnt.emplace_back(0);
        end.emplace_back(0);
        alert.emplace_back(0);
        return u;
    }

    // 插入模式串 s 返回该模式串终点对应的节点编号
    // int id = ac.insert("abc");那么 id 就是字符串 "abc" 的终点
    // 后续可以通过 ans[id] 获取这个模式串的匹配次数
    // 插入所有字符串复杂度 O(L)
    int insert(const string &s){
        assert(!ok);// 预处理后不允许插入模式串
        assert(!s.empty());// 不允许空串

        int u = root;
        for(auto c : s){
            int path = getPath(c);
            if(tree[u][path] == 0){// 新建节点
                int v = node();
                tree[u][path] = v;
                len[v] = len[u] + 1;
            }
            u = tree[u][path];
        }

        end[u] = 1;
        alert[u] = 1;
        return u;// 返回终点节点
    }

    // 从已有 Trie 节点 u 添加一个字符 c
    // 返回添加后的节点
    int extend(int u, char c){
        assert(!ok);

        int path = getPath(c);
        if(tree[u][path] == 0){// 新建节点
            int v = node();
            tree[u][path] = v;
            len[v] = len[u] + 1;
        }

        return tree[u][path];
    }

    // 预处理
    // 建立 fail 指针，补全自动机转移，记录 BFS 序
    // 复杂度 O(VM)
    void work(){
        assert(!ok);
        queue<int> q;
        q.push(root);
        order.clear();
        order.reserve(n);
        order.emplace_back(root);
        while(!q.empty()){
            int u = q.front();
            q.pop();
            for(int i = 0; i < M; i++){
                int v = tree[u][i];
                if(v == 0){// 不存在边
                    // 优化 1 直接设置失配后去向，避免 fail 指针绕圈
                    tree[u][i] = tree[fail[u]][i];
                }
                else{// 存在边
                    fail[v] = tree[fail[u]][i];
                    // 优化 3 命中状态提前传播，避免 fail 指针绕圈
                    alert[v] |= alert[fail[v]];
                    q.push(v);
                    order.emplace_back(v);
                }
            }
        }
        ok = true;
    }

    // 统计文章中每个状态对应字符串的出现次数
    // 复杂度 O(S + V) 
    vector<int> getCnt(const string &s){
        assert(ok);
        // 优化 2 只收集词频，不立即沿 fail 链传播，文章遍历结束之后，再统一处理
        cnt.assign(n + 1, 0);
        int u = root;
        for(auto c : s){
            u = tree[u][getPath(c)];
            cnt[u]++;// 只统计当前状态，暂时不传递次数
        }

        // 按 BFS 序从深到浅，子节点贡献给父节点
        for(int i = order.size() - 1; i > 0; i--){
            int v = order[i];
            cnt[fail[v]] += cnt[v];
        }
        return cnt;
    }

    // 判断文本中是否出现任意模式串
    // 复杂度 O(S)
    bool matchAny(const string &s){
        assert(ok);
        int u = root;
        for(auto c : s){
            u = tree[u][getPath(c)];
            if(alert[u]){
                return true;
            }
        }
        return false;
    }
};

// 使用示例
int main(){
    int n;
    cin >> n;// 模式串的个数
    ACAM<> acam;
    vector<int> id(n);// 记录模式串的结尾的节点编号
    string s;
    for(int i = 0; i < n; i++){
        cin >> s;
        id[i] = acam.insert(s);
    }
    acam.work();
    cin >> s;
    vector<int> cnt = acam.getCnt(s);
    for(int i = 0; i < n; i++){
        cout << cnt[id[i]] << endl;
    }
}