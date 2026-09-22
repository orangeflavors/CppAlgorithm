#include<bits/stdc++.h>

using namespace std;
using ll = long long;
const int N = 1e5 + 5;
const int inf = 0x3f3f3f3f;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
//const int MOD = 998244353;

//DP
//O(n**2)
void solve1(){
    int n; cin >> n;
    vector<int> nums(n), dp(n, 0);
    //dp[i]表示以i位置的数做结尾的最长递增子序列的长度
    int ans = 0;
    for(int i = 0; i < n; i++) cin >> nums[i];

    for(int i = 0; i < n; i++){
        for(int j = 0; j < i; j++){
            if(nums[j] < nums[i])
                dp[i] = max(dp[i], dp[j]);
        }
        dp[i] += 1;
        ans = max(ans, dp[i]);
    }
    cout << ans << endl;
}

//贪心+二分
//O(nlogn)
void solve2(){
    int n; cin >> n;
    vector<int> nums(n), ends(n, 0);
    //ends[i]表示目前所有长度为i+1的递增子序列的最小结尾
    //省去dp,dp[i]表示以i位置的数做结尾的最长递增子序列的长度
    int len = 0;//len表示ends有效区间长度
    for(int i = 0; i < n; i++) cin >> nums[i];
    
    for(int i = 0; i < n; i++){
        auto find = lower_bound(ends.begin(), ends.begin() + len, nums[i]);
        if(find == ends.begin() + len){
            ends[len++] = nums[i];
        }
        else{
            ends[find - ends.begin()] = nums[i];
        }
    }
    /*
    for(int i = 0, find; i < n; i++){
        find = binarySearch(ends, len, nums[i]);
        if(find == -1){
            ends[len++] = nums[i];
        }
        else{
            ends[find] = nums[i];
        }
    }
    */
    cout << len << endl;
}

// 封装求 LIS 函数,返回找到的 LIS 及其在原数组中的下标
pair<vector<int>, vector<int>> LIS(const vector<int> &nums){
    int n = nums.size();
    if(n == 0) return {{}, {}};

    vector<int> ends(n, 0), endIdx(n, -1), prev(n, -1);
    //ends[i]表示目前所有长度为i+1的递增子序列的最小结尾
    //省去dp,dp[i]表示以i位置的数做结尾的最长递增子序列的长度
    int len = 0;//len表示ends有效区间长度
    for(int i = 0; i < n; i++){
        auto it = lower_bound(ends.begin(), ends.begin() + len, nums[i]);
        int pos = it - ends.begin();

        if(it == ends.begin() + len){
            ends[len] = nums[i];
            endIdx[len] = i;
            len++;
        }
        else{
            ends[pos] = nums[i];
            endIdx[pos] = i;
        }

        if(pos > 0){
            prev[i] = endIdx[pos - 1];
        }
    }
    
    // 回溯重建LIS
    vector<int> lis;
    vector<int> lisIdx;
    for(int cur = endIdx[len - 1]; cur != -1; cur = prev[cur]){
        lis.push_back(nums[cur]);
        lisIdx.push_back(cur);
    }
    reverse(lis.begin(), lis.end());
    reverse(lisIdx.begin(), lisIdx.end());
    
    return {lis, lisIdx};
}

//ends严格升序,找到>=nums的最左位置,如果不存在返回-1
int binarySearch(vector<int> ends, int len, int num){
    int l = 0, r = len - 1, mid = (l + r) / 2, ans = -1;
    while(l <= r){
        mid = (l + r) / 2;
        if(num <= ends[mid]){
            ans = mid;
            r = mid - 1;
        }
        else{
            l = mid + 1;
        }
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int T; cin >> T;
    while(T--){
        solve2();
    }
    return 0;
}