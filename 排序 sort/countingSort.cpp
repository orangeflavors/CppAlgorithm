#include<bits/stdc++.h>
using namespace std;

// 计数排序
// 动态数组
// 区间arr整体
template<class T>
void countingSort(vector<T>& arr){
    if(arr.empty()) return;

    int n = arr.size();
    T max_val = *std::max_element(arr.begin(), arr.end());
    T min_val = *std::min_element(arr.begin(), arr.end());
    
    int range = max_val - min_val + 1;
    vector<int> cnt(range, 0);
    vector<T> output(n);

    for(int i = 0; i < n; i++){
        cnt[arr[i] - min_val]++;// 计数,将值映射到[0, range-1]的索引
    }

    for(int i = 1; i < range; i++){
        cnt[i] += cnt[i - 1];// 将计数数组转换为前缀和
    }
    // cnt[i]现在表示小于等于i + min_val的元素个数

    for(int i = n - 1; i >= 0; i--){// 从后往前遍历原数组,保证排序的稳定性
        output[cnt[arr[i] - min_val] - 1] = arr[i];// cnt[arr[i] - min_val] - 1找到该元素在输出数组中的正确位置
        cnt[arr[i] - min_val]--;// 放置一个数就将其减去
    }

    for(int i = 0; i < n; i++){
        arr[i] = output[i];// 复制回原数组
    }
}

template<typename Array> void printvec(Array vec) { for (auto it = vec.begin(); it != vec.end(); ++it) { cout << *it << " ";} cout << endl;}
template<typename T>
bool isSorted(const vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}
template<typename T>
T randint(T l, T r) {
    static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<T> dis(l, r);
    return dis(gen);
}
class Timer {
private:
    chrono::time_point<chrono::high_resolution_clock> startTime;

public:
    void start() {
        startTime = chrono::high_resolution_clock::now();
    }

    double stop() {
        auto endTime = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = endTime - startTime;
       return chrono::duration<double, milli>(elapsed).count();
    }
};

int main(){
    Timer timer;
    int n;
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        arr[i] = randint(1, 100000);
    }

    timer.start();
    countingSort(arr);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}