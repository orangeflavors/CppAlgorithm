#include<bits/stdc++.h>
using namespace std;

int n;

// 冒泡排序
// 区间[first,last)
void bubbleSort(vector<int>& arr, int first, int last){
    for(int i = first; i < last - 1; i++){
        bool swapped = false;
        for(int j = first; j < last - i - 1; j++){
            if(arr[j] > arr[j + 1]){
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if(!swapped) break;//优化：这一轮冒泡没有发生交换说明已经有序，结束
    }
}

// 以下为辅助程序
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
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        arr[i] = randint(1, 100000);
    }

    timer.start();
    bubbleSort(arr, 0, n);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" << endl;
    printvec(arr);
    return 0;
}