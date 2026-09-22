#include<bits/stdc++.h>

using namespace std;
using ll = long long;

void solve(){
    int n;
    vector<ll> a(n + 1);

    stack<ll> st;
    vector<ll> L(n + 1), R(n + 1);
    // L[i] 左边第一个严格大于a[i]的位置,R[i] 右边第一个大于等于a[i]的位置
    for(int i = 1; i <= n; i++){
        while(!st.empty() && a[st.top()] <= a[i]) st.pop();// 严格递减栈,栈顶为最小值
        L[i] = st.empty() ? 0 : st.top();// 元素按顺序入栈,弹出小于等于的,剩下的栈顶就是最近的严格大于a[i]的
        st.push(i);
    }
    while(!st.empty()) st.pop();

    for(int i = n; i >= 1; i--){
        while(!st.empty() && a[st.top()] < a[i]) st.pop();// 非严格递减,栈顶为最小值,可以相等
        R[i] = st.empty() ? n + 1 : st.top();
        st.push(i);
    }
/*
    用于解决区间最大值贡献问题
    给定数组 a[1..n],求所有子数组的最大值之和
    等价转化为：
    每个元素 a[i] 作为"区间最大值"时,对答案的贡献是多少
    利用单调栈找到 i 能扩展到的最大左右边界
    定义L[i]左边第一个严格大于a[i]的位置,R[i]右边第一个大于等于a[i]的位置
    合法区间[l, r]满足
        L[i] < l <= i <= r < R[i]
    总区间数 = (i - L[i]) * (R[i] - i)
    贡献 = a[i] * (i - L[i]) * (R[i] - i)
    左严格,右非严格保证含有多个相等最大元素的区间,都归属于最后一个的贡献,避免重复计算
    解释:
    设数组中有x1,x2...xm相等
    R[i]定义大于等于保证了对于xi,x1...xi-1在扩展右区间时无法越过xi
    也就保证只有xi计算了所有以xi为最后一个x的区间

    该单调栈的构建过程等同于构建一棵大顶堆,中序顺序的笛卡尔树
    如果左非严格,右严格则归属于第一个
*/
/*
    解决区间最小值贡献问题同理
    定义L[i]左边第一个严格小于a[i]的位置,R[i]右边第一个小于等于a[i]的位置
*/
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for(int i = 0; i < n; i++){
        cin >> a[i];
    }

    vector<int> L(n + 1), R(n + 1);
    stack<int> st;
    for(int i = 0; i < n; i++){
        while(!st.empty() && a[st.top()] >= a[i]){
            int j = st.top();
            st.pop();
            R[j] = i;
            L[j] = (st.empty() ? -1 : st.top());
        }
        st.push(i);
    }

    while(!st.empty()){
        int j = st.top();
        st.pop();
        R[j] = -1;
        L[j] = (st.empty() ? -1 : st.top());
    }

    for(int i = n - 1; i >= 0; i--){
        while(R[i] >= 0 && a[R[i]] == a[i]){
            R[i] = R[R[i]];
        }
    }

    for(int i = 0; i < n; i++){
        cout << L[i] << " " << R[i] << endl;
    }

    return 0;
}