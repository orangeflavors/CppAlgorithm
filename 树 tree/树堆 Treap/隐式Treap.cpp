#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int N = 1e5;

// 隐式Treap
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


        Node(): ls(0), rs(0), key(0), pri(0), size(0), sum(0){}
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
        return cnt;
    }

    // 更新以 u 为根的子树大小
    void update(int u){
        tree[u].size = tree[tree[u].ls].size + tree[tree[u].rs].size + 1;
    }

    // 排名分裂
    // 返回以 L 为根的子树包含前 x 个节点, 以 R 为根的子树包含其他节点
    void split(int u, int x, int &L, int &R){
        if(u == 0){L = 0; R = 0; return;}
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

    // 在位置 pos 插入元素,0-based,在第 pos 个位置后插入新节点,即前有第 1~pos 个节点,插入成为第 pos+1 个节点
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

    // 查询序列第 k 个元素,1-based
    ll kth(int k){
        int now = root;
        while(now){// 非递归写法
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

