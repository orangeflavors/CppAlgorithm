#include<bits/stdc++.h>
using namespace std;
template<typename Array> void printvec(Array vec);

// 快速排序
// 动态数组
// 区间[first, last)
template<class T>
void quickSort(vector<T>& arr, int first, int last){
    if(first >= last - 1) return;// 边界

    int i = first, j = last - 1;
    T key = arr[(i + j) / 2];
    // int mid = first + (last - first) / 2;
    // // 三数取中法选择基准值
    // if (arr[first] > arr[mid]) swap(arr[first], arr[mid]);
    // if (arr[first] > arr[last-1]) swap(arr[first], arr[last-1]);
    // if (arr[mid] > arr[last-1]) swap(arr[mid], arr[last-1]);
    // T key = arr[mid];
    while(i <= j){
        while(arr[i] < key) i++;
        while(arr[j] > key) j--;
        if(i <= j){
            swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    // cout << "左区间[" << first << "," << j + 1 << ") ";
    // cout << "右区间[" << i << "," << last << ")\n";
    // printvec(arr);
    if(j > first) quickSort(arr, first, j + 1);
    if(i < last - 1) quickSort(arr, i, last);
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
    quickSort(arr, 0, n);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}