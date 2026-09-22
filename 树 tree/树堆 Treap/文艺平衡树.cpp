#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e5;
const ll INF = 4e18;

// 文艺平衡树
// 静态数组封装
// 有很大的数组,不能建立在函数栈上,只能建立在堆上或者主函数的栈上
class FHQTreap{
public:
    struct Node{
        int ls, rs;// 左子右子
        ll key;// 键值
        uint32_t pri;// 优先级
        int size;// 以当前节点为根的子树的节点数量
        ll sum;// 子树区间和
        ll mx;// 子树区间最大值
        int rev_lazy;// 翻转懒标记
        ll add_lazy;// 区间加懒标记

        Node(): ls(0), rs(0), key(0), pri(0), size(0), sum(0), mx(-INF), rev_lazy(0), add_lazy(0){}
    };

    Node tree[N];// 树,tree[0]作为空节点
    int root;// 根节点
    int cnt;// tree[cnt] 最新节点存储的位置
    mt19937 rng;

    FHQTreap(): root(0), cnt(0), rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count()){}

    // 新建节点
    int newNode(ll val){
        cnt++;
        tree[cnt].ls = 0;
        tree[cnt].rs = 0;
        tree[cnt].key = val;
        tree[cnt].pri = rng();
        tree[cnt].size = 1;
        tree[cnt].sum = val;
        tree[cnt].mx = val;
        return cnt;
    }

    // 更新节点 u 的信息
    void update(int u){
        if(!u) return;
        tree[u].size = tree[tree[u].ls].size + tree[tree[u].rs].size + 1;
        tree[u].sum = tree[tree[u].ls].sum + tree[u].key + tree[tree[u].rs].sum;
        tree[u].mx = max({tree[u].key, tree[tree[u].ls].mx, tree[tree[u].rs].mx});
    }

    // 下传 lazy 标记
    void pushdown(int u){
        if(tree[u].add_lazy != 0){
            ll d = tree[u].add_lazy;
            tree[u].key += d;
            tree[u].sum += d * tree[u].size;
            tree[u].mx += d;
            tree[tree[u].ls].add_lazy += d;// 向左右子树下传 add_lazy 标记
            tree[tree[u].rs].add_lazy += d;
            tree[u].add_lazy = 0;
        }

        if(tree[u].rev_lazy){
            swap(tree[u].ls, tree[u].rs);// 翻转左右子树
            tree[tree[u].ls].rev_lazy ^= 1;// 向左右子树下传 rev_lazy 标记
            tree[tree[u].rs].rev_lazy ^= 1;
            tree[u].rev_lazy = 0;// 清除标记
        }
    }

    // 排名分裂
    // 返回以 L 为根的子树包含前 x 个节点, 以 R 为根的子树包含其他节点
    void split(int u, int x, int &L, int &R){
        if(u == 0){L = 0; R = 0; return;}
        pushdown(u);// 下传 lazy 标记
        if(tree[tree[u].ls].size + 1 <= x){// 第 x 个数在右子树上
            L = u;
            split(tree[u].rs, x - (tree[tree[u].ls].size + 1), tree[u].rs, R);
        }
        else{// 第 x 个数在左子树上
            R = u;
            split(tree[u].ls, x, L, tree[u].ls);
        }
        update(u);
    }

    // 合并
    // pri 满足大顶堆,含有隐含条件 L 上所有节点在前, R 上所有节点在后
    int merge(int L, int R){
        if(L == 0 || R == 0) return L + R;// L或R为0,到达叶子,返回另一个节点
        if(tree[L].pri > tree[R].pri){// 按大顶堆性质合并
            pushdown(L);// 下传 lazy 标记
            tree[L].rs = merge(tree[L].rs, R);// 合并并更新L的右子
            update(L);
            return L;
        }
        else{
            pushdown(R);// 下传 lazy 标记
            tree[R].ls = merge(L, tree[R].ls);// 合并并更新R的左子
            update(R);
            return R;
        }
    }

    // 在位置 pos 插入元素,0-based,在第 pos 个元素后面插入新节点,即前有第 1~pos 个节点,插入成为第 pos+1 个节点
    // pos=0 表示插在最前面, pos>=cnt 表示插在最后面
    void insert(int pos, ll val){
        int L, R;
        split(root, pos, L, R);// 以 L 为根的子树包含前 pos 个节点, 以 R 为根的子树包含其他节点
        int mid = newNode(val);
        root = merge(merge(L, mid), R);
    }

    // 删除位置 pos 的元素,0-based,即删除第 pos+1 个元素
    void erase(int pos){
        int L, mid, R;
        split(root, pos, L, mid);// 以 L 为根的子树包含前 pos 个节点, 以 mid 为根的子树包含其他节点
        split(mid, 1, mid, R);// 以 mid 为根的子树只包含第 pos+1 个元素,以 R 为根的子树包含右侧其他节点
        root = merge(L, R);
    }

    // 翻转区间[l, r],0-based
    void reverse(int l, int r){
        if(l >= r) return;
        int L, mid, R;
        split(root, l, L, mid);// 以 L 为根的子树包含前 l 个节点, 以 mid 为根的子树包含其他节点
        split(mid, r - l + 1, mid, R);// 以 mid 为根的子树包含区间 [l,r] 共 r-l+1 个元素,以 R 为根的子树包含右侧其他节点
        tree[mid].rev_lazy ^= 1;// 以 mid 为根的子树记录翻转
        root = merge(merge(L, mid), R);
    }

    // 将区间[l, r]中的元素加上 d
    void rangeAdd(int l, int r, ll d){
        if(l > r) return;
        int L, mid, R;
        split(root, l, L, mid);// 以 L 为根的子树包含前 l 个节点, 以 mid 为根的子树包含其他节点
        split(mid, r - l + 1, mid, R);// 以 mid 为根的子树包含区间 [l,r] 共 r-l+1 个元素,以 R 为根的子树包含右侧其他节点
        tree[mid].add_lazy += d;
        root = merge(merge(L, mid), R);
    }

    // 查询序列第 k 个元素,1-based
    ll kth(int k){
        int now = root;
        while(now){// 非递归写法
            pushdown(now);// 下传 lazy 标记
            int Lsize = tree[tree[now].ls].size;
            if(k <= Lsize){// 在左子树
                now = tree[now].ls;
            }
            else if(k <= Lsize + 1){// 在当前节点
                return tree[now].key;
            }
            else{// 在右子
                k -= Lsize + 1;
                now = tree[now].rs;
            }
        }
        return -1;// k 不合法
    }

    // 查询区间 [l, r] 的和,0-based
    ll rangeSum(int l, int r){
        if(l > r) return 0;
        int L, mid, R;
        split(root, l, L, mid);// 以 L 为根的子树包含前 l 个节点, 以 mid 为根的子树包含其他节点
        split(mid, r - l + 1, mid, R);// 以 mid 为根的子树包含区间 [l,r] 共 r-l+1 个元素,以 R 为根的子树包含右侧其他节点
        ll res = tree[mid].sum;
        root = merge(merge(L, mid), R);
        return res;
    }

    // 查询区间 [l, r] 的最大值,0-based
    ll rangeMax(int l, int r){
        if(l > r) return -INF;
        int L, mid, R;
        split(root, l, L, mid);// 以 L 为根的子树包含前 l 个节点, 以 mid 为根的子树包含其他节点
        split(mid, r - l + 1, mid, R);// 以 mid 为根的子树包含区间 [l,r] 共 r-l+1 个元素,以 R 为根的子树包含右侧其他节点
        ll res = tree[mid].mx;
        root = merge(merge(L, mid), R);
        return res;
    }

    void build(const vector<ll> &a){
        root = 0;
        for(ll x : a){
            int newnode = newNode(x);
            root = merge(root, newnode);
        }
    }

    // 返回整棵树的大小
    int size(){
        return tree[root].size;
    }

    // 打印以 u 为根的子树,中序遍历
    void printTree(int u){
        if(u == 0) return;
        pushdown(u);// 下传 lazy 标记
        printTree(tree[u].ls);
        cout << tree[u].key << " ";
        printTree(tree[u].rs);
    }
};

