#include <bits/stdc++.h>
using namespace std;

class StringHash {
public:
    using ull = unsigned long long;
    int n;                  // 字符串长度
    static constexpr ull base = 13131;

    string s;               // 1-indexed
    vector<ull> pw;         // pw[i] = base^i
    vector<ull> pre;        // 正向前缀哈希
    vector<ull> revPre;     // 反向前缀哈希

    /*
        pre[i]:
        hash(s[1..i])

        采用：
        H(s1...sk) = s1 * B^(k-1) + s2 * B^(k-2) + ... + sk

        递推：
        pre[i] = pre[i-1] * B + val(s[i])
    */

    /*
        revPre[i]:
        将字符串反转以后计算前缀哈希。

        例如：
        s = "abcde"
        reverse(s) = "edcba"

        revPre 中存的就是 "edcba" 的前缀哈希。
    */

    explicit StringHash(const string& str) {
        n = (int)str.size();

        // 改成 1-indexed
        s = "#" + str;

        pw.resize(n + 1);
        pre.resize(n + 1);
        revPre.resize(n + 1);

        // 预处理 base 的幂
        pw[0] = 1;
        for (int i = 1; i <= n; i++) {
            pw[i] = pw[i - 1] * base;
        }

        // 正向哈希
        for (int i = 1; i <= n; i++) {
            pre[i] = pre[i - 1] * base + getValue(s[i]);
        }

        // 反向字符串
        string rs = "#";
        for (int i = n; i >= 1; i--) {
            rs += s[i];
        }

        // 反向哈希
        for (int i = 1; i <= n; i++) {
            revPre[i] = revPre[i - 1] * base + getValue(rs[i]);
        }
    }

    // 字符映射
    static ull getValue(char c) {
        return (ull)(c - 'a' + 1);
    }

    /*
        正向哈希：
        hashForward(l, r)
        = hash(s[l..r])

        O(1)
    */
    ull hashForward(int l, int r) const {
        if (l > r) return 0;

        int len = r - l + 1;

        return pre[r] - pre[l - 1] * pw[len];
    }

    /*
        反向哈希：
        hashBackward(l, r)
        = hash(s[r], s[r-1], ..., s[l])

        O(1)

        原串位置 [l, r]
        在反转串中的对应位置为：

        n-r+1 ~ n-l+1
    */
    ull hashBackward(int l, int r) const {
        if (l > r) return 0;

        int L = n - r + 1;
        int R = n - l + 1;
        int len = r - l + 1;

        return revPre[R] - revPre[L - 1] * pw[len];
    }

    /*
        返回区间 [l, r] 的正向、反向哈希
    */
    pair<ull, ull> getHash(int l, int r) const {
        return {
            hashForward(l, r),
            hashBackward(l, r)
        };
    }

    /*
        判断 s[l..r] 是否为回文串
        O(1)
    */
    bool isPalindrome(int l, int r) const {
        return hashForward(l, r) == hashBackward(l, r);
    }
};


// 树上双向哈希
class LCA{
public:
    using ull = unsigned long long;
    int n;// 节点个数 n
    int power;// st表层数[0, power],最多要跳的步数,log2(n) 下取整
    int root;// 根节点
    vector<vector<int>> tree;// 邻接表存树
    vector<vector<int>> stjump;// st表,st[u][p] 节点 u 的第 2^k 个祖先
    vector<int> deep;// deep[u] 节点 u 的深度
    vector<ull> stup, stdown;
    // stup[u] u 到 root 的字符串哈希，stdown[u] root 到 u 的字符串哈希
    string s;

    // 一维字符串哈希
    ull const static base = 13131;
    vector<ull> power_base;

    // 手写 log2(x) 下取整
    inline int log2(unsigned long long x){
        int power = 0;
        while((1ULL << power) <= (x >> 1)){
            power++;
        }
        return power;
    }

    ull fastPow(ull base, ull exp){
        ull ans = 1;
        while(exp > 0){
            if(exp & 1)
                ans *= base;
            base *= base;
            exp >>= 1;
        }
        return ans;
    }

    // 初始化
    explicit LCA(int n, string s): n(n), s(s), root(0), tree(n + 1), deep(n + 1) {
        power = log2(n);
        stjump.assign(n + 1, vector<int>(power + 1, 0));
        power_base.assign(n + 1, 0);
        power_base[0] = 1;
        for(int i = 1; i <= n; i++){
            power_base[i] = power_base[i - 1] * base;
        }
        stup.assign(n + 1, 0);
        stdown.assign(n + 1, 0);
    }

    // 加双向边
    void add(int u, int v){
        tree[u].emplace_back(v);
        tree[v].emplace_back(u);
    }

    // dfs 收集信息
    void dfs(int u, int fa){
        stjump[u][0] = fa;// u 向上跳 1 步是 fa
        stup[u] = getValue(s[u]) * power_base[deep[u]] + stup[fa];
        stdown[u] = getValue(s[u]) + stdown[fa] * base;
        for(int p = 1; p <= power; p++){
            // u 向上跳 2^p 步等价于 u 向上跳 2^(p - 1) 步的点再向上跳 2^(p - 1) 步
            stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
        }

        for(auto v : tree[u]){
            if(v == fa) continue;
            deep[v] = deep[u] + 1;
            dfs(v, u);
        }
    }

    static ull getValue(char c) {
        return (ull)(c - 'a' + 1);
    }

    // 预处理
    // 默认 root = 1
    void work(int rt = 1){
        root = rt;
        fill(deep.begin(), deep.end(), 0);

        deep[root] = 0;
        stup[root] = s[root] - 'a' + 1;
        stdown[root] = s[root] - 'a' + 1;
        dfs(root, 0);
    }

    // 返回节点 u 的第 k 个祖先，即向上跳 k 步
    // O(log n)
    int getKthAncestor(int u, int k) const{
        if(k < 0 || k > deep[u])// 越界
            return 0;
        
        for(int bit = 0; bit <= power; bit++){// 对 k 二进制分解
            if((k >> bit) & 1)
                u = stjump[u][bit];
        }
        return u;
    }

    // 返回节点 u, v 的 lca
    // O(log n)
    int getLCA(int u, int v) const{
        if(deep[u] < deep[v]) swap(u, v);// u 作深度大的节点

        // 1.u 跳至与 v 同深度
        u = getKthAncestor(u, deep[u] - deep[v]);

        if(u == v) return u;// 特判 v 是 u 的祖先
        // 2.u,v 同时往上跳
        for(int p = power; p >= 0; p--){// 二进制提升
            if(stjump[u][p] != stjump[v][p]){
                u = stjump[u][p];
                v = stjump[v][p];
            }
        }
        return stjump[u][0];
    }

    // 返回 u, v 间距离，即边数
    int getDis(int u, int v) const{
        // u 到根距离 + v 到根距离 - 2 * lca到根距离
        return deep[u] + deep[v] - 2 * deep[getLCA(u, v)];
    }

    // 返回从 u 到 v 的路径上走第 k 步到达的点
    int getKthPoint(int u, int v, int k) const{
        int lca = getLCA(u, v);
        int l = deep[u] - deep[lca], r = deep[v] - deep[lca];
        if(k < 0 || k > l + r) return 0;// k 范围 [0, l + r]
        // 在 u 一侧从 u 向上跳，在 v 一侧从 v 向上跳
        return k <= l ? getKthAncestor(u, k) : getKthAncestor(v, l + r - k);
    }

    ull getHash(int u, int v){
        int lca = getLCA(u, v);

        int len1 = deep[u] - deep[lca], len2 = deep[v] - deep[lca];
        ull left = stup[u] - stup[lca];
        ull right = stdown[v] - stdown[lca] * power_base[len2];
        ull ans = left * power_base[len2] + getValue(s[lca]) * power_base[deep[lca] + len2] + right * power_base[deep[lca]];
        return ans;
    }
};