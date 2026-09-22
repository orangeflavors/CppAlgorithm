#include<bits/stdc++.h>

enum Color{
    BLACK,
    RED,
};

template<class T>
struct RBTreeNode{
    T _data;
    RBTreeNode* _left;
    RBTreeNode* _right;
    RBTreeNode* _father;
    Color _col;

    RBTreeNode(const T& value)
        :_data(value), _left(nullptr), _right(nullptr), _father(nullptr), _col(RED) {}
};

// 红黑树动态节点封装
// 左根右,根叶黑,不红红,黑路同
template<class T>
class RBTree{
    typedef RBTreeNode<T> Node;
private:
    Node* _root;
    int size;
public:
    RBTree():_root(nullptr), size(0) {}
    ~RBTree(){}

    bool insert(const T& value){
        Node* newNode = new Node(value);

        if(_root == nullptr){
            _root = newNode;
            _root->_col = BLACK;// 根节点为黑色
            size++;
            return ture;
        }

        Node* cur = _root;
        Node* father = nullptr;

        while(cur){
            father = cur;
            if(value < cur->_data){// 左滑
                cur = cur->_left;
            }
            else if(value > cur->_data){// 右滑
                cur = cur->_right;
            }
            else{// 键已存在
                delete newNode;
                return false;
            }
        }

        newNode->_father = father;
        if(value < cur->_data){
            father->_left = newNode;
        }
        else{
            father->_right = newNode;
        }
        size++;
        fixInsert(newNode);
    }

    void fixInsert(Node* cur){
        while(cur != _root && cur->_father->_col == RED){// 违反不红红
            Node* grandfa = cur->_father->_father;
            if(cur->_father == grandfa->_left){// L
                Node* uncle = grandfa->_right;
                if(uncle->_col == RED){// 叔节点红色 染色+爷作新
                    cur->_father->_col = BLACK;// 父节点红染黑
                    uncle->_col = BLACK;// 叔节点红染黑
                    cur->_father->_father->_col = RED;// 爷节点黑染红
                    cur = cur->_father->_father;// 爷节点作新节点进行while
                }
                else{// 叔节点黑色 旋转+染色
                    if(cur == cur->_father->_right){// LR型
                        cur = cur->_father;
                        leftRotate(cur);
                    }
                    
                }
            }
        }
    }

    // 左旋
    void leftRotate(Node* cur){
        Node* fa = cur->_father;
        Node* sR = cur->_right;
        Node* sRL = sR->_left;

        cur->_right = sRL;// 处理sRL
        if(sRL){
            sRL->_father = cur;
        }

        sR->_left = cur;// 提升sR
        cur->_father = sR;

        sR->_father = fa;
        if(fa == nullptr){// 如果fa为空则sR被提升为根
            _root = sR;
        }
        else{
            if(fa->_left == cur){
                fa->_left = sR;
            }
            else{
                fa->_right = sR;
            }
        }
    }

    // 右旋
    void rightRotate(Node* cur){
        Node* fa = cur->_father;
        Node* sL = cur->_left;
        Node* sLR = sL->_right;

        cur->_left = sLR;// 处理sLR
        if(sLR){
            sLR->_father = cur;
        }

        sL->_right = cur;// 提升sL
        cur->_father = sL;

        sL->_father = fa;
        if(fa == nullptr){// 如果fa为空则sL被提升为根
            _root = sL
        }
        else{
            if(fa->_left = cur){
                fa->_left = sL;
            }
            else{
                fa->_right = sL;
            }
        }
    }
};
