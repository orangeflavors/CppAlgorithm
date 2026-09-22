#include<bits/stdc++.h>
using namespace std;
template<typename Array> void printvec(Array vec);

// 堆排序
// 手写二叉堆,不使用额外空间
// 区间arr整体

// 维护以i为根节点的子树满足最大堆性质,n当前堆的大小
void heapify(vector<int>& arr, int n, int i){
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < n && arr[left] > arr[largest]){
        largest = left;
    }
    if(right < n && arr[right] > arr[largest]){
        largest = right;
    }
    
    if(largest != i){
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
void heapSort(vector<int>& arr){
    int n = arr.size();
    if (n <= 1) return;// 边界

    // 建堆,从最后一个非叶子节点n/2-1开始
    for(int i = n / 2 - 1; i >= 0; i--){
        heapify(arr, n, i);
    }

    for(int i = n - 1; i > 0; i--){
        swap(arr[0], arr[i]);// 弹出堆顶,将当前最大值移到末尾
        heapify(arr, i, 0);// 调整剩余部分为最大堆
    }
}

// 堆排序
// 使用优先队列
// 区间[l,r]
void heapSort(vector<int>& arr, int first, int last){
    priority_queue<int, vector<int>, greater<int>> pq;// 小根堆
    for(int i = first; i < last; i++){
        pq.push(arr[i]);
    }
    for(int i = first; i < last; i++){
        arr[i] = pq.top();
        pq.pop();
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
    heapSort(arr);
    // heapSort(arr, 0, n);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    // printvec(arr);
    return 0;
}