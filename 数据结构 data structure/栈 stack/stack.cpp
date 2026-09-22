#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

template<class T>
class mystack{
private:
    T a[N];
    int t;
public:
    mystack():t(0){}
    void push(T x){a[++t] = x;}
    void pop(){t--;}
    T top(){return a[t];}
    bool empty(){return t == 0;}
    int size(){return t;}
    void clear(){t = 0;}
};

/*
struct mystack{
    int a[N];
    int t;
    mystack(){t = 0;}
    void push(int x){a[++t] = x;}
    int top(){return a[t];}
    void pop(){t--;}
    int empty(){return t == 0 ? 1 : 0;}
    int size(){return t;}
};
*/