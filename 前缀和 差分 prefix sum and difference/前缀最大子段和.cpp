#include<bits/stdc++.h>
using namespace std;

void solve(){
    int n;
    cin >> n;
    vector<int> a(n);
    for(auto &i : a) cin >> i;

    vector<int> pre(n, 0);// 前缀最大子段和,pre[i]0到i位置的最大子段和
    int cur = a[0];// 以a[i]结尾的最大子段和
    pre[0] = a[0];// pre[0]只能选a[0]
    for(int i = 1; i < n; i++){
        cur = max(a[i], cur + a[i]);// 只取a[i]或将a[i]接在以a[i - 1]结尾的最大子段后
        pre[i] = max(pre[i - 1], cur);// 取pre[i - 1]的最大子段和或取新的
    }

    vector<int> post(n, 0);// 后缀最大子段和,post[i]n-1到i位置的最大子段和
    cur = a[n - 1];// 以a[i]开头的最大子段和
    post[n - 1] = a[n - 1];// pre[n - 1]只能选a[n - 1]
    for(int i = n - 2; i >= 0; i--){
        cur = max(a[i], cur + a[i]);// 只取a[i]或将a[i]接在以a[i - 1]开头的最大子段前
        post[i] = max(post[i + 1], cur);// 取post[i + 1]的最大子段和或取新的
    }
}