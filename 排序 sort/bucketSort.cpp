#include<bits/stdc++.h>
using namespace std;

// 桶排序
class BucketSort {
public:
    static void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        
        int n = arr.size();
        int max_val = 1e7;
        
        // 1. 使用计数排序思路的桶排序
        int bucket_count = 10000;  // 固定桶数
        std::vector<std::vector<int>> buckets(bucket_count);
        
        // 2. 第一次扫描：只记录每个桶的大小
        std::vector<int> bucket_sizes(bucket_count, 0);
        for (int num : arr) {
            int bucket_idx = (long long)num * bucket_count / (max_val + 1);
            bucket_sizes[bucket_idx]++;
        }
        
        // 3. 预分配内存
        for (int i = 0; i < bucket_count; i++) {
            buckets[i].reserve(bucket_sizes[i]);
        }
        
        // 4. 第二次扫描：填充数据
        for (int num : arr) {
            int bucket_idx = (long long)num * bucket_count / (max_val + 1);
            buckets[bucket_idx].push_back(num);
        }
        
        // 5. 并行排序（如果支持OpenMP）
        #pragma omp parallel for if(n > 1000000)
        for (int i = 0; i < bucket_count; i++) {
            if (!buckets[i].empty()) {
                std::sort(buckets[i].begin(), buckets[i].end());
            }
        }
        
        // 6. 合并
        int idx = 0;
        for (int i = 0; i < bucket_count; i++) {
            std::copy(buckets[i].begin(), buckets[i].end(), arr.begin() + idx);
            idx += buckets[i].size();
        }
    }
};

// 位图桶排序
// 处理1e7级别的整数
// 动态数组
// 区间arr整体
class BitmapBucketSort {
public:
    static void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        
        const int MAX_VAL = 1e7;
        const int BUCKETS = 1024;  // 1024个桶
        
        // 1. 分桶
        std::vector<std::vector<int>> buckets(BUCKETS);
        
        // 2. 分配（使用位运算加速）
        for (int num : arr) {
            // 桶索引：取高10位（因为1024=2^10）
            int bucket_idx = num >> 10;  // 等价于 num / 1024
            buckets[bucket_idx].push_back(num);
        }
        
        // 3. 排序并合并
        int idx = 0;
        for (int i = 0; i < BUCKETS; i++) {
            if (!buckets[i].empty()) {
                // 对桶内使用基数排序（适合整数）
                if (buckets[i].size() > 10000) {
                    radixSort(buckets[i]);
                } else {
                    std::sort(buckets[i].begin(), buckets[i].end());
                }
                
                for (int val : buckets[i]) {
                    arr[idx++] = val;
                }
            }
        }
    }
    
private:
    // 基数排序实现
    static void radixSort(std::vector<int>& arr) {
        const int BASE = 10;
        int max_val = *std::max_element(arr.begin(), arr.end());
        
        for (int exp = 1; max_val / exp > 0; exp *= BASE) {
            countingSortByDigit(arr, exp, BASE);
        }
    }
    
    static void countingSortByDigit(std::vector<int>& arr, int exp, int base) {
        int n = arr.size();
        std::vector<int> output(n);
        std::vector<int> count(base, 0);
        
        // 统计频率
        for (int i = 0; i < n; i++) {
            int digit = (arr[i] / exp) % base;
            count[digit]++;
        }
        
        // 累加频率
        for (int i = 1; i < base; i++) {
            count[i] += count[i - 1];
        }
        
        // 从后向前排序（稳定排序）
        for (int i = n - 1; i >= 0; i--) {
            int digit = (arr[i] / exp) % base;
            output[count[digit] - 1] = arr[i];
            count[digit]--;
        }
        
        arr = std::move(output);
    }
};

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
    BitmapBucketSort::sort(arr);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}