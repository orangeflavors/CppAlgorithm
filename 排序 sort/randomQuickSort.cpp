#include<bits/stdc++.h>
using namespace std;

// 随机三路快排
// 动态数组
// 区间[first, last)
template<typename T>
T randint(T l, T r);
template <class T>
void quickSort(vector<T>& arr, const int first, const int last){
    if(last - first <= 1)
        return;
    
    const T pivot = arr[randint(first, last - 1)];
    int i = first, j = first, k = last - 1;
    // 荷兰国旗问题,将序列分为三路:
    // 小于 pivot 的元素 | 等于 pivot 的元素 | 大于 pivot 的元素
    // [first, j - 1]元素<pivot
    // [j, i - 1]元素==pivot
    // [i, k]元素待处理
    // [k + 1, last - 1]元素>pivot
    while(i <= k){
        if(arr[i] < pivot){
            swap(arr[i], arr[j]);
            i++;
            j++;
        }
        else if(pivot < arr[i]){
            swap(arr[i], arr[k]);
            k--;
        }
        else{
            i++;
        }
    }
    // 递归
    quickSort(arr, first, j);
    quickSort(arr, k + 1, last);
}


const int N = 1e5;
int n;
int a[N];
// 随机快排朴素版,不推荐
// 静态数组
// 区间[l, r]
int partition1(int l, int r, int x);
void quickSort1(int l, int r){
    if(l >= r)
        return;

    int x = a[randint(l, r)];
    int mid = partition1(l, r, x);// mid上的x已排好
    quickSort1(l, mid - 1);// [l, mid - 1]元素<=x
    quickSort1(mid + 1, r);// [mid + 1, r]元素>x
}

// 将数组区间[l,r]分为
// [l, mid]元素<=x
// [mid + 1, r]元素>x
int partition1(int l, int r, int x){
    int j = l;// [l, j-1]表示<=x的范围
    int xi = 0;// 记录在<=的范围中任意一个x的位置
    for(int i = l; i <= r; i++){
        if(a[i] <= x){
            swap(a[i], a[j]);
            if(a[j] == x)
                xi = j;
            j++;
        }
    }
    swap(a[xi], a[j - 1]);
    return j - 1;
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
        a[i] = arr[i];
    }

    timer.start();
    quickSort(arr, 0, n);
    quickSort1(0, n - 1);
    double time = timer.stop();

    for(int i = 0; i < n; i++){
        cout << a[i] << " ";
    }
    cout << endl;
    cout << (isSorted(arr) ? "Yes" : "No") << endl;
    cout << time << "ms" <<endl;
    printvec(arr);
    return 0;
}