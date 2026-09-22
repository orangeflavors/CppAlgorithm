#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e5;

// 静态数组封装
// 有很大的数组,不能建立在函数栈上,只能建立在堆上或者主函数的栈上
// 权值树
class FHQTreap{
public:
    struct Node{
        int ls, rs;// 左子右子
        ll key;// 键值
        uint32_t pri;// 优先级
        int size;// 以当前节点为根的子树的节点数量
        int count;// 词频

        Node(): ls(0), rs(0), key(0), pri(0), size(0), count(0){}
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
        tree[cnt].count = 1;
        return cnt;
    }

    // 更新以 u 为根的子树大小
    void update(int u){
        tree[u].size = tree[tree[u].ls].size + tree[tree[u].rs].size + 1;
    }

    // 权值分裂
    // 返回以 L 为根的子树所有 key <= val, 以 R 为根的子树所有 key > val
    void split(int u, ll val, int &L, int &R){
        if(u == 0){L = 0; R = 0; return;}
        if(tree[u].key <= val){
            L = u;
            split(tree[u].rs, val, tree[u].rs, R);
        }
        else{
            R = u;
            split(tree[u].ls, val, L, tree[u].ls);
        }
        update(u);
    }

    // 合并
    // pri 满足大顶堆,含有隐含条件 L 上所有 key 都小于 R 的
    int merge(int L, int R){
        if(L == 0 || R == 0) return L + R;// L或R为0,到达叶子,返回另一个节点
        if(tree[L].pri > tree[R].pri){// 按大顶堆性质合并
            tree[L].rs = merge(tree[L].rs, R);// 合并并更新L的右子
            update(L);
            return L;
        }
        else{
            tree[R].ls = merge(L, tree[R].ls);// 合并并更新R的左子
            update(R);
            return R;
        }
    }

    // 无重复值插入
    void insert(ll val){
        int L, R;
        split(root, val, L, R);
        int mid = newNode(val);
        root = merge(merge(L, mid), R);
    }

    // 有重复值插入,多次插入同一个val在节点中的count记录
    void insert2(ll val){
        int L, mid, R;
        split(root, val, mid, R);// 以 mid 为根的子树所有 key <= val,以 R 为根的子树所有 key > val
        split(mid, val - 1, L, mid);// 以 L 为根的子树所有 key <= val - 1,以 mid 为根的子树所有 key == val

        if(mid){
            tree[mid].count++;
            update(mid);
        }
        else{
            mid = newNode(val);
        }

        root = merge(merge(L, mid), R);
    }

    // 删除 val
    void erase(ll val){
        int L, mid, R;
        split(root, val, mid, R);// 以 mid 为根的子树所有 key <= val,以 R 为根的子树所有 key > val
        split(mid, val - 1, L, mid);// 以 L 为根的子树所有 key <= val - 1,以 mid 为根的子树所有 key == val

        if(mid){
            if(tree[mid].count > 1){// count > 1 减小计数
                tree[mid].count--;
                update(mid);
            }
            else{// 合并 mid 的左子和右子,删除 mid 节点
                mid = merge(tree[mid].ls, tree[mid].rs);
            }
        }

        root = merge(merge(L, mid), R);
    }

    // 返回 val 的排名,比它小的元素个数 + 1,1-based
    int rank(ll val){
        int L, R;
        split(root, val - 1, L, R);// 以 L 为根的子树所有 key <= val - 1,以 R 为根的子树所有 key > val - 1
        int res = tree[L].size + 1;
        root = merge(L, R);
        return res;
    }

    // 查询第 k 小,1-based
    ll kth(int k){
        int now = root;
        while(now){// 非递归写法
            int Lsize = tree[tree[now].ls].size;
            if(k <= Lsize){// 在左子树
                now = tree[now].ls;
            }
            else if(k <= Lsize + tree[now].count){// 在当前节点
                return tree[now].key;
            }
            else{// 在右子
                k -= Lsize + tree[now].count;
                now = tree[now].rs;
            }
        }
        return -1;// k 不合法
    }

    // 查询 val 的前驱,小于 val 的最大值
    ll predecessor(ll val){
        int L, R;
        split(root, val - 1, L, R);// 以 L 为根的子树所有 key <= val - 1,以 R 为根的子树所有 key > val - 1
        int now = L;
        while(tree[now].rs) now = tree[now].rs;// 非递归写法
        ll res = tree[now].key;
        root = merge(L, R);
        return res;
    }

    // 查询 val 的后继,大于 val 的最小值
    ll successor(ll val){
        int L, R;
        split(root, val, L, R);// 以 L 为根的子树所有 key <= val,以 R 为根的子树所有 key > val
        int now = R;
        while(tree[now].ls) now = tree[now].ls;// 非递归写法
        ll res = tree[now].key;
        root = merge(L, R);
        return res;
    }

    // 返回整棵树的大小
    int size(){
        return tree[root].size;
    }

    // 打印以 u 为根的子树,中序遍历
    void printTree(int u){
        if(u == 0) return;
        printTree(tree[u].ls);
        cout << tree[u].key << " ";
        printTree(tree[u].rs);
    }
};

