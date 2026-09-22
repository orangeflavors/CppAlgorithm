#include<bits/stdc++.h>
using namespace std;
template<typename Array> void printvec(Array vec);

int n;

// 插入排序
// 区间[first,last)
// gap增量
void insertionSort(vector<int>& arr, int first, int last, int gap){
    for(int i = first + gap; i < last; i++){// [first, first + gap - 1]是gap组数的0号位,已排好,从first + gap开始
        int key = arr[i];// 记录arr[i],准备将其插到前面合适的位置
        int j = i - gap;// i所在的组的前一个数,已排好
        while(j >= first && arr[j] > key){// 若key比arr[j]小
            arr[j + gap] = arr[j];// 把arr[j]往后挪,给key腾位置
            j -= gap;
        }
        arr[j + gap] = key;// 插入key
    }
}

// 希尔排序
// 区间[first,last)
void shellSort(vector<int>& arr, int first, int last){
    for(int gap = (last - first) / 2; gap > 0; gap /= 2){// 希尔原始序列,增量序列为(N/2, N/4, ..., 1)
        insertionSort(arr, first, last, gap);
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
    cin >> n;
    vector<int> arr(n);
    for(int i = 0; i < n; i++){
        arr[i] = randint(1, 100000);
        // cin >> arr[i];
    }
    printvec(arr);

    timer.start();
    shellSort(arr, 0, n);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}