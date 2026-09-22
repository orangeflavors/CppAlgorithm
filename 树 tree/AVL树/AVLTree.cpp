#include<bits/stdc++.h>
using namespace std;
const int N = 1e5 + 5;

// 静态数组封装
// 有很大的数组,不能建立在函数栈上,只能建立在堆上或者主函数的栈上
class AVLTree{
    public:
    struct Node{
        int key;// 键值
        int ls, rs;// 左右子
        int height;// 节点高度
        int count;// 词频
        int size;// 子树大小
    };

    Node t[N];// 节点数组
    int cnt;// 节点计数
    int root;// 根节点索引

    // 清空AVL树
    void clear(){
        for(int i = 0; i <= cnt; i++)
            t[i] = {0, 0, 0, 0, 0, 0};
        cnt = 0;
        root = 0;
    }

    AVLTree(){
        cnt = 0;
        root = 0;
        clear();
    }
    
    // 更新节点i的信息
    void update(int i){
        if(i == 0) return;// 空指针保护
        t[i].height = max(t[t[i].ls].height, t[t[i].rs].height) + 1;
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

    // 维护以i为根节点的子树的平衡,返回根节点的空间编号
    int maintain(int i){
        if(i == 0) return 0;// 空指针保护
        int ls = t[i].ls, rs = t[i].rs;
        int lh = t[ls].height, rh = t[rs].height;
        if(lh - rh > 1){
            if(t[t[ls].ls].height >= t[t[ls].rs].height)// LL
                i = rightRotate(i);
            else{
                t[i].ls = leftRotate(ls);// LR
                i = rightRotate(i);
            }
        }
        else if(rh - lh > 1){
            if(t[t[rs].rs].height >= t[t[rs].ls].height)// RR
                i = leftRotate(i);
            else{
                t[i].rs = rightRotate(rs);// RL
                i = leftRotate(i);
            }
        }
        return i;
    }

    // 在以i为根的子树中增加num,返回根节点的空间编号
    int add(int i, int num){
        if(i == 0){// 不存在该节点,创建节点
            cnt++;
            t[cnt].key = num;
            t[cnt].ls = t[cnt].rs = 0;
            t[cnt].count = t[cnt].size = t[cnt].height = 1;
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
        return maintain(i);
    }

    // 向AVL树中增加num
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

    // 查询AVL树中num的排名,即比num小的数的数量+1
    int getRank(int num){
        return getRank(root, num) + 1;
    }

    // 移除以i为根的子树中的最左子节点,返回根节点的空间编号
    int removeMostLeft(int i, int most_left){
        if(i == most_left)// 最左子没有左子,将其右子交给父节点接管
            return t[most_left].rs;
        else{// 向左子递归
            t[i].ls = removeMostLeft(t[i].ls, most_left);
            update(i);
            return maintain(i);
        }
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
                    int most_left = t[i].rs;
                    while(t[most_left].ls != 0)// 找到i的右子树的最左子
                        most_left = t[most_left].ls;
                    t[i].rs = removeMostLeft(t[i].rs, most_left);// 删除右子树中的最左子
                    t[most_left].rs = t[i].rs;// 最左子接管删除了最左子的i的右子树
                    t[most_left].ls = t[i].ls;// 最左子接管i的左子树
                    i = most_left;// 子树根节点i替换为最左子,原节点空间废弃
                }
            }
        }
        update(i);
        return maintain(i);
    }

    // 在AVL树中删除num
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

    // 查询AVL树中第x位的数
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

    // 查询AVL树中num的前驱
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

    // 查询AVL树中num的前驱
    int post(int num){
        return post(root, num);
    }
};

// 动态空间分配封装
class AVLTree2{
private:
    struct Node {
        int key, height, size, count;
        Node *left, *right;
        Node(int val) : key(val), height(1), size(1), count(1), left(nullptr), right(nullptr) {}
    };

    Node* root;
    vector<Node*> nodes; // 用于内存管理

public:
    AVLTree2() : root(nullptr) {}
    
    ~AVLTree2() {
        clear();
    }
    
    void clear() {
        for (Node* node : nodes) {
            delete node;
        }
        nodes.clear();
        root = nullptr;
    }
    
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }
    
    int getSize(Node* node) {
        return node ? node->size : 0;
    }
    
    int getBalance(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    void update(Node* node) {
        if (!node) return;
        node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
        node->size = getSize(node->left) + getSize(node->right) + node->count;
    }
    
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        
        x->right = y;
        y->left = T2;
        
        update(y);
        update(x);
        
        return x;
    }
    
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        
        y->left = x;
        x->right = T2;
        
        update(x);
        update(y);
        
        return y;
    }
    
    Node* balance(Node* node) {
        if (!node) return node;
        
        update(node);
        int balanceFactor = getBalance(node);
        
        // Left Left Case
        if (balanceFactor > 1 && getBalance(node->left) >= 0)
            return rightRotate(node);
        
        // Left Right Case
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        
        // Right Right Case
        if (balanceFactor < -1 && getBalance(node->right) <= 0)
            return leftRotate(node);
        
        // Right Left Case
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
        
        return node;
    }
    
    void add(int num) {
        root = add(root, num);
    }
    
    Node* add(Node* node, int num) {
        if (!node) {
            Node* newNode = new Node(num);
            nodes.push_back(newNode);
            return newNode;
        }
        
        if (num < node->key)
            node->left = add(node->left, num);
        else if (num > node->key)
            node->right = add(node->right, num);
        else
            node->count++;
        
        return balance(node);
    }
    
    void remove(int num) {
        root = remove(root, num);
    }
    
    Node* remove(Node* node, int num) {
        if (!node) return nullptr;
        
        if (num < node->key)
            node->left = remove(node->left, num);
        else if (num > node->key)
            node->right = remove(node->right, num);
        else {
            if (node->count > 1) {
                node->count--;
            } else {
                if (!node->left || !node->right) {
                    Node* temp = node->left ? node->left : node->right;
                    if (!temp) {
                        temp = node;
                        node = nullptr;
                    } else {
                        *node = *temp;
                    }
                } else {
                    Node* temp = findMin(node->right);
                    node->key = temp->key;
                    node->count = temp->count;
                    temp->count = 1; // 确保只删除一个
                    node->right = remove(node->right, temp->key);
                }
            }
        }
        
        if (!node) return nullptr;
        
        return balance(node);
    }
    
    Node* findMin(Node* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }
    
    int getRank(int num) {
        return getRank(root, num) + 1;
    }
    
    int getRank(Node* node, int num) {
        if (!node) return 0;
        
        if (num < node->key)
            return getRank(node->left, num);
        else if (num > node->key)
            return getSize(node->left) + node->count + getRank(node->right, num);
        else
            return getSize(node->left);
    }
    
    int index(int x) {
        if (!root || x < 1 || x > root->size) return -1;
        return index(root, x);
    }
    
    int index(Node* node, int x) {
        if (!node) return -1;
        
        int leftSize = getSize(node->left);
        
        if (x <= leftSize)
            return index(node->left, x);
        else if (x <= leftSize + node->count)
            return node->key;
        else
            return index(node->right, x - leftSize - node->count);
    }
    
    int pre(int num) {
        return pre(root, num);
    }
    
    int pre(Node* node, int num) {
        if (!node) return INT_MIN;
        
        if (node->key >= num)
            return pre(node->left, num);
        else
            return max(node->key, pre(node->right, num));
    }
    
    int post(int num) {
        return post(root, num);
    }
    
    int post(Node* node, int num) {
        if (!node) return INT_MAX;
        
        if (node->key <= num)
            return post(node->right, num);
        else
            return min(node->key, post(node->left, num));
    }
};

/*
// 未封装
int cnt = 0;
int head = 0;
struct Node{
    int key;// 节点的值
    int height;
    int left, right;// 左子右子
    int count;// 词频
    int size;
} tree[N];

// 更新节点信息
void up(int i){
    tree[i].size = tree[tree[i].left].size + tree[tree[i].right].size + tree[i].count;
    tree[i].height = max(tree[tree[i].left].height, tree[tree[i].right].height) + 1;
}

// 左旋
int leftRotate(int i){
    int rs = tree[i].right;
    tree[i].right = tree[rs].left;
    tree[rs].left = i;
    up(i);
    up(rs);
    return rs;
}

// 右旋
int rightRotate(int i){
    int ls = tree[i].left;
    tree[i].left = tree[ls].right;
    tree[ls].right = i;
    up(i);
    up(ls);
    return ls;
}

int maintain(int i){
    int ls = tree[i].left, rs = tree[i].right;
    int lh = tree[ls].height, rh = tree[rs].height;
    if(lh - rh > 1){
        int lls = tree[ls].left, lrs = tree[ls].right;
        if(tree[lls].height >= tree[lrs].height)// LL
            i = rightRotate(i);
        else{// LR
            tree[i].left = leftRotate(ls);
            i = rightRotate(i);
        }
    }
    else if(rh - lh > 1){
        int rls = tree[rs].left, rrs = tree[rs].right;
        if(tree[rrs].height >= tree[rls].height)// RR
            i = leftRotate(i);
        else{// RL
            tree[i].right = rightRotate(rs);
            i = leftRotate(i);
        }
    }
    return i;
}

// 在以head为根的子树中插入节点num
int add(int i, int num){
    if(i == 0){
        tree[++cnt].key = num;
        tree[cnt].count = tree[cnt].size = tree[cnt].height = 1;
        return cnt;
    }
    if(num == tree[i].key)
        tree[i].count++;
    else if(num < tree[i].key)
        tree[i].left = add(tree[i].left, num);
    else
        tree[i].right = add(tree[i].right, num);
    up(i);
    return maintain(i);
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
        if(tree[i].key >= num)
            return small(tree[i].left, num);
        else
            return tree[tree[i].left].size + tree[i].count + small(tree[i].right, num);
    }
}

// 查询num的排名,即比num小的数的数量+1
int getRank(int num){
    return small(head, num) + 1;
}

// 在以i为根的子树中删除mostLeft,返回根节点
int removeMostLeft(int i, int most_left){
    if(i == most_left)
        return tree[i].right;
    else{
        tree[i].left = removeMostLeft(tree[i].left, most_left);
        up(i);
        return maintain(i);
    }
}

// 在以i为根的树中删除num,返回根节点编号
int remove(int i, int num){
    if(num < tree[i].key)
        tree[i].left = remove(tree[i].left, num);
    else if(num > tree[i].key)
        tree[i].right = remove(tree[i].right, num);
    else{
        if(tree[i].count > 1)
            tree[i].count--;
        else{
            if(tree[i].left == 0 && tree[i].right == 0)
                return 0;
            else if(tree[i].left == 0)
                i = tree[i].right;
            else if(tree[i].right == 0)
                i = tree[i].left;
            else{
                int most_left = tree[i].right;
                while(tree[most_left].left != 0)
                    most_left = tree[most_left].left;
                tree[i].right = removeMostLeft(tree[i].right, most_left);
                tree[most_left].right = tree[i].right;
                tree[most_left].left = tree[i].left;
                i = most_left;// 旧i空间废弃
            }
        }
    }
    up(i);
    return maintain(i);
}

// 删除num
void remove(int num){
    if(getRank(num) != getRank(num + 1))
        head = remove(head, num);
}

// 查询以i为根的树中的第x个数
int index(int i, int x){
    if(tree[tree[i].left].size >= x)
        return index(tree[i].left, x);
    else if(tree[tree[i].left].size + tree[i].count < x)
        return index(tree[i].right, x - (tree[tree[i].left].size + tree[i].count));
    return tree[i].key;
}

// 查询第x个数
int index(int x){
    return index(head, x);
}

// 查询以i为根的树中num的前驱
int pre(int i, int num){
    if(i == 0)
        return INT_MIN;
    if(num <= tree[i].key)
        return pre(tree[i].left, num);
    else
        return max(tree[i].key, pre(tree[i].right, num));
}

// 查询num的前驱
int pre(int num){
    return pre(head, num);
}

// 查询以i为根的树中num的后继
int post(int i, int num){
    if(i == 0)
        return INT_MAX;
    if(num >= tree[i].key)
        return post(tree[i].right, num);
    else
        return min(tree[i].key, post(tree[i].left, num));
}

// 查询num的后继
int post(int num){
    return post(head, num);
}
*/