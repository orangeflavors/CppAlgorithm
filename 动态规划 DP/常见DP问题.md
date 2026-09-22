#### 多集合可达子集和的交集最大化
1.求每个集合的所有子集和
2.取所有子集和的交集
3.在交集中找出最大值
luoguP1504
```cpp
int N = 1e4 + 5;
void solve(){
    int n;
    cin >> n;
    vector<vector<int>> a(n);
    bitset<N> ans;
    ans.set();
    for(int i = 0; i < n; i++){
        int x;
        bitset<N> temp;
        temp.set(0);
        while(cin >> x && x != -1){
            a[i].emplace_back(x);
            temp |= (temp << x);
        }
        ans &= temp;
    }
    for(int i = N - 1; i >= 0; i--){
        if(ans.test(i)){
            cout << i << endl;
            return;
        }
    }
}
```

#### DP 与概率结合，求最佳期望
[2025ICPC南京区域赛 I](https://qoj.ac/contest/2581/problem/14809)