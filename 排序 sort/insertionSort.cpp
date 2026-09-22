#include<bits/stdc++.h>
using namespace std;

int n;

// 插入排序
// 区间[first,last)
void insertionSort(vector<int>& arr, int first, int last){
    for(int i = first + 1; i < last; i++){
        int key = arr[i];// 记录arr[i],准备将其插到前面合适的位置
        int j = i - 1;
        while(j >= first && arr[j] > key){// 若key比arr[j]小
            arr[j + 1] = arr[j];// 把arr[j]往后挪,给key腾位置
            j--;
        }
        arr[j + 1] = key;// 插入key
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
    }

    timer.start();
    insertionSort(arr, 0, n);
    double time = timer.stop();

    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    // printvec(arr);
    return 0;
}