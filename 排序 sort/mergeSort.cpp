#include<bits/stdc++.h>
using namespace std;

// 归并排序
// 动态数组
// 区间[l,r]
template <class T>
class MergeSort {
private:
    static void merge(vector<T>& a, vector<T>& b, int L, int mid, int R) {
        int i = L, j = mid + 1, t = 0;
        while (i <= mid && j <= R) {
            if (a[i] > a[j]) b[t++] = a[j++];
            else b[t++] = a[i++];
        }
        while (i <= mid) b[t++] = a[i++];
        while (j <= R) b[t++] = a[j++];
        for (i = 0; i < t; i++) a[L + i] = b[i];
    }

    static void mergeSort(vector<T>& a, vector<T>& b, int L, int R) {
        if (L < R) {
            int mid = L + (R - L) / 2;// 避免溢出
            mergeSort(a, b, L, mid);
            mergeSort(a, b, mid + 1, R);// 递归分解为左右两个长度相同的子序列
            merge(a, b, L, mid, R);// 合并
        }
    }
    
public:
    static void sort(vector<T>& a, int l, int r) {
        vector<T> b(a.size());
        mergeSort(a, b, l, r);
    }
};

const int N = 1e5 + 5;
int a[N], b[N];

// 归并排序
// 静态数组
// 区间[l,r]
void merge(int L,int mid ,int R){
    int i = L, j = mid + 1, t = 0;
    while(i <= mid && j <= R){
        if(a[i] > a[j]) b[t++] = a[j++];
        else b[t++] = a[i++];
    }
    while(i <= mid) b[t++] = a[i++];
    while(j <= R) b[t++] = a[j++];
    for(i = 0; i < t; i++) a[L + i] = b[i];
}

void mergeSort(int L, int R){
    if(L < R){
        int mid = (L + R) / 2;
        mergeSort(L, mid);
        mergeSort(mid + 1, R);//递归分解为左右两个长度相同的子序列
        merge(L, mid, R);//合并
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
        a[i] = arr[i];
    }

    timer.start();
    // mergeSort(0, n - 1);
    MergeSort<int>::sort(arr, 0, n - 1);
    double time = timer.stop();

    // for(int i = 0; i < n; i++){
    //     cout << a[i] << " ";
    // }
    // cout << endl;
    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}