#include<bits/stdc++.h>
using namespace std;
template<typename Array> void printvec(Array vec);

// 基数排序
// 最低位优先法(Least Significant Digit first, LSD)
// 动态数组
// 区间arr整体
template<class T>
void radixSort(vector<T>& arr){
    if(arr.empty()) return;

    T max_val = *std::max_element(arr.begin(), arr.end());

    int n = arr.size();
    int cnt[10];
    vector<T> output(n);
    for(long long exp = 1; max_val / exp > 0; exp *= 10){
        memset(cnt, 0, sizeof(cnt));

        for(int i = 0; i < n; i++){
            cnt[(arr[i] / exp) % 10]++;// 统计exp位的每个数字的出现次数
        }
        
        for(int i = 1; i < 10; i++){
            cnt[i] += cnt[i - 1];// 将计数数组转换为前缀和
        }
        // cnt[i]现在表示exp位的数字<=i的元素个数
        
        for(int i = n - 1; i >= 0; i--){// 从后往前遍历原数组,保证排序的稳定性
            output[cnt[(arr[i] / exp) % 10] - 1] = arr[i];// cnt[(arr[i] / exp) % 10] - 1找到该元素在输出数组中的正确位置
            cnt[(arr[i] / exp) % 10]--;// 放置一个数就将其减去
        }
        
        for(int i = 0; i < n; i++){
            arr[i] = output[i];// 复制回原数组
        }
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
        // cin >> arr[i];
    }

    timer.start();
    radixSort(arr);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}