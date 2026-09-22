#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

// 静态数组封装
// 旋转Treap
// key按二叉搜索树组织,pri按大顶堆组织
// 有很大的数组,不能建立在函数栈上,只能建立在堆上或者主函数的栈上
// 须在类外定义和初始化随机数生成器或者使用inline
/*
std::random_device Treap::rd;
std::mt19937 Treap::gen(Treap::rd());
std::uniform_int_distribution<int> Treap::dis(1, 1000000);
*/
class Treap{
public:
    struct Node{
        int key;// 键值
        int pri;// 优先级
        int ls, rs;// 左右子
        int count;// 词频
        int size;// 子树大小
    };

    Node t[N];
    int cnt;
    int root;

    static random_device rd;
    static mt19937 gen;
    static uniform_int_distribution<int> dis;

    // 清空Treap
    void clear(){
        for(int i = 0; i <= cnt; i++){
            t[i] = {0, 0, 0, 0, 0, 0};
        }
        cnt = 0;
        root = 0;
    }

    Treap(){
        cnt = 0;
        root = 0;
        clear();
    }

    // 更新节点i的信息
    void update(int i){
        if(i == 0) return;// 空指针保护
        t[i].size = t[t[i].ls].size + t[t[i].rs].size + t[i].count;
    }

    // 左旋节点i,返回左旋后根节点的空间编号
    int leftRotate(int i){
        if(i == 0) return 0;// 空指针保护
        int rs = t[i].rs;
        if(rs == 0) return i;// 空指针保护
        t[i].rs = t[rs].ls;
        t[rs].ls = i;
        update(i);
        update(rs);
        return rs;// 返回新的根节点
    }

    // 右旋节点i,返回右旋后根节点的空间编号
    int rightRotate(int i){
        if(i == 0) return 0;// 空指针保护
        int ls = t[i].ls;
        if(ls == 0) return i;// 空指针保护
        t[i].ls = t[ls].rs;
        t[ls].rs = i;
        update(i);
        update(ls);
        return ls;// 返回新的根节点
    }

    // 在以i为根的子树中增加num,返回根节点的空间编号
    int add(int i, int num){
        if(i == 0){// 不存在该节点,创建节点
            cnt++;
            t[cnt].key = num;
            t[cnt].ls = t[cnt].rs = 0;
            t[cnt].count = t[cnt].size  = 1;
            t[cnt].pri = dis(gen);
            return cnt;
        }
        else{
            if(num < t[i].key)// 左滑
                t[i].ls = add(t[i].ls, num);
            else if(num > t[i].key)// 右滑
                t[i].rs = add(t[i].rs, num);
            else// 存在该节点
                t[i].count++;
        }
        update(i);
        if(t[i].ls != 0 && t[t[i].ls].pri > t[i].pri)
            return rightRotate(i);// 按最大堆组织,如果左子优先级大右旋为根
        if(t[i].rs != 0 && t[t[i].rs].pri > t[i].pri)
            return leftRotate(i);// 按最大堆组织,如果右子优先级大左旋为根
        return i;
    }

    // 向Treap中增加num
    void add(int num){
        root = add(root, num);
    }

    // 查询以i为根的子树中,比num小的数的数量
    int getRank(int i, int num){
        if(i == 0)// 查到空节点则比树中所有节点都小,返回0
            return 0;
        if(num <= t[i].key)// 小于等于i节点的值,查询左子树
            return getRank(t[i].ls, num);
        else// 大于i节点的值,左子树所有节点及i均小于num,再查询右子树
            return t[t[i].ls].size + t[i].count + getRank(t[i].rs, num);
    }

    // 查询Treap中num的排名,即比num小的数的数量+1
    int getRank(int num){
        return getRank(root, num) + 1;
    }

    // 在以i为根的子树中删除num,返回根节点的空间编号
    int remove(int i, int num){
        if(num < t[i].key)// 左滑
            t[i].ls = remove(t[i].ls, num);
        else if(num > t[i].key)// 右滑
            t[i].rs = remove(t[i].rs, num);
        else{// 节点i为被删除节点
            if(t[i].count > 1)
                t[i].count--;
            else{
                if(t[i].ls == 0 && t[i].rs == 0)// 无左右子
                    return 0;
                else if(t[i].rs == 0)// 只有左子
                    i = t[i].ls;
                else if(t[i].ls == 0)// 只有右子
                    i = t[i].rs;
                else{// 既有左子又有右子
                    if(t[t[i].ls].pri >= t[t[i].rs].pri){// 如果左子优先级大于右子,右旋以左子为根,再删除右子树中的num
                        i = rightRotate(i);
                        t[i].rs = remove(t[i].rs, num);
                    }
                    else{// 如果右子优先级大于左子,左旋以右子为根,再删除左子树中的num
                        i = leftRotate(i);
                        t[i].ls = remove(t[i].ls, num);
                    }
                }
            }
        }
        update(i);
        return i;
    }

    // 在Treap中删除num
    void remove(int num){
        if(getRank(num) != getRank(num + 1))// 查询树中是否存在num
            root = remove(root, num);
    }

    // 查询在以i为根的子树中第x位的数
    int index(int i, int x){
        if(t[t[i].ls].size >= x)// x在左子树中
            return index(t[i].ls, x);
        else if(t[t[i].ls].size + t[i].count < x)// x在右子树中
            return index(t[i].rs, x - (t[t[i].ls].size + t[i].count));
        else// x在i中
            return t[i].key;
    }

    // 查询Treap中第x位的数
    int index(int x){
        return index(root, x);
    }

    // 查询在以i为根的子树中num的前驱
    int pre(int i, int num){
        if(i == 0)
            return INT_MIN;
        if(num <= t[i].key)// 前驱在左子树中
            return pre(t[i].ls, num);
        else// 前驱为i或者在i的右子树中
            return max(t[i].key, pre(t[i].rs, num));
    }

    // 查询Treap中num的前驱
    int pre(int num){
        return pre(root, num);
    }

    // 查询在以i为根的子树中num的后继
    int post(int i, int num){
        if(i == 0)
            return INT_MAX;
        if(num >= t[i].key)// 后继在右子树中
            return post(t[i].rs, num);
        else// 后继为i或者在i的左子树中
            return min(t[i].key, post(t[i].ls, num));
    }

    // 查询Treap中num的后继
    int post(int num){
        return post(root, num);
    }
};


/*
// 未封装
int cnt = 0;
int head = 0;
int key[N];// 键值
int pri[N];// 优先级
int ls[N], rs[N];// 左右子
int key_cnt[N];// 词频
int size[N];// 以i为根的子树的大小

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(1, 1000000);

// 更新
void up(int i){
    size[i] = size[ls[i]] + size[rs[i]] + key_cnt[i];
}

// 左旋
int leftRotate(int i){
    int r = rs[i];
    rs[i] = ls[r];
    ls[r] = i;
    up(i);
    up(r);
    return r;
}

// 右旋
int rightRotate(int i){
    int l = ls[i];
    ls[i] = rs[l];
    rs[l] = i;
    up(i);
    up(l);
    return l;
}

// 在以head为根的子树中插入节点num
int add(int i, int num){
    if(i == 0){
        key[++cnt] = num;
        key_cnt[cnt] = size[cnt] = 1;
        pri[cnt] = dis(gen);
        return cnt;
    }
    if(num == key[i])
        key_cnt[i]++;
    else if(num < key[i])
        ls[i] = add(ls[i], num);
    else
        rs[i] = add(rs[i], num);
    up(i);
    if(ls[i] != 0 && pri[ls[i]] > pri[i])
        return rightRotate(i);
    if(rs[i] != 0 && pri[rs[i]] > pri[i])
        return leftRotate(i);
    return i;
}

// 插入节点num
void add(int num){
    head = add(head, num);
}

// 以i为根的树,比num小的数的数量
int small(int i, int num){
    if(i == 0)
        return 0;
    else{
        if(key[i] >= num)
            return small(ls[i], num);
        else
            return size[ls[i]] + key_cnt[i] + small(rs[i], num);
    }
}

// 查询num的排名,即比num小的数的数量+1
int getRank(int num){
    return small(head, num) + 1;
}

// 在以i为根的树中删除num,返回根节点编号
int remove(int i, int num){
    if(num < key[i])// 左滑
        ls[i] = remove(ls[i], num);
    else if(num > key[i])// 右滑
        rs[i] = remove(rs[i], num);
    else{
        if(ls[i] == 0 && rs[i] == 0)// 无子节点
            return 0;
        else if(ls[i] == 0)// 只有右子
            i = rs[i];
        else if(rs[i] == 0)// 只有左子
            i = ls[i];
        else{
            if(pri[ls[i]] >= pri[rs[i]]){
                i = rightRotate(i);
                rs[i] = remove(rs[i], num);
            }
            else{
                i = leftRotate(i);
                ls[i] = remove(ls[i], num);
            }
        }
    }
    up(i);
    return i;
}

// 删除num
void remove(int num){
    if(getRank(num) != getRank(num + 1))
        head = remove(head, num);
}

// 查询以i为根的树中的第x个数
int index(int i, int x) {
   if (size[ls[i]] >= x) {
       return index(ls[i], x);
   } else if (size[ls[i]] + key_cnt[i] < x) {
       return index(rs[i], x - size[ls[i]] - key_cnt[i]);
   }
   return key[i];
}

// 查询第x个数
int index(int x) {
   return index(head, x);
}

// 查询以i为根的树中num的前驱
int pre(int i, int num) {
   if (i == 0) {
       return INT_MIN;
   }
   if (key[i] >= num) {
       return pre(ls[i], num);
   } else {
       return max(key[i], pre(rs[i], num));
   }
}

// 查询num的前驱
int pre(int num) {
   return pre(head, num);
}

// 查询以i为根的树中num的后继
int post(int i, int num) {
   if (i == 0) {
       return INT_MAX;
   }
   if (key[i] <= num) {
       return post(rs[i], num);
   } else {
       return min(key[i], post(ls[i], num));
   }
}

// 查询num的后继
int post(int num) {
   return post(head, num);
}
*/