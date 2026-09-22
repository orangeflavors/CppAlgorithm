#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

// 未封装
int arr[N];// 下标key,值value
int ls[N];// 左子
int rs[N];// 右子
int sta[N];// 单调栈,存储key
int n;

// 建树
// key用二叉搜索树组织,value用小根堆组织
void build(){
    for(int i = 1, top = 0, pos = 0; i <= n; i++){
        pos = top;// pos当前检查的位置,top栈顶
        while(pos > 0 && arr[i] < arr[sta[pos]]){
            pos--;
        }
        if(pos > 0){// 若栈中还有节点,i作为栈顶节点的右子
            rs[sta[pos]] = i;
        }
        if(pos < top){// 若有节点弹出,将最后弹出的节点作为i的左子
            ls[i] = sta[pos + 1];
        }
        sta[++pos] = i;// 入栈
        top = pos;
    }
}