#include<bits/stdc++.h>

using namespace std;
const int N = 1e5 + 5;
int a[N], b[N];

//选择排序
//时间复杂度O(n**2)
void SelectionSort(int n){
    for(int i = 0; i < n - 1; i++){
        int m = i;
        for(int j = i + 1; j < n; j++){
            if(a[j] < a[m]) m = j;
        }
        swap(a[i], a[m]);
    }
}

//冒泡排序
//时间复杂度O(n**2)
void BubbleSort(int n){
    for(int i = 0; i < n - 1; i++){
        bool swapped = false;
        for(int j = 0; j < n - i - 1; j++){
            if(a[j] > a[j + 1]){
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if(!swapped) break;//优化：这一轮冒泡没有发生交换说明已经有序，结束
    }
}

//归并排序
//时间复杂度O(nlog2n)
//空间复杂度O(n)
void Merge(int L,int mid ,int R){
    int i = L, j = mid + 1, t = 0;
    while(i <= mid && j <= R){
        if(a[i] > a[j]) b[t++] = a[j++];
        else b[t++] = a[i++];
    }
    while(i <= mid) b[t++] = a[i++];
    while(j <= R) b[t++] = a[j++];
    for(i = 0; i < t; i++) a[L + i] = b[i];
}

void MergeSort(int L, int R){
    if(L < R){
        int mid = (L + R) / 2;
        MergeSort(L, mid);
        MergeSort(mid + 1, R);//递归分解为左右两个长度相同的子序列
        Merge(L, mid, R);//合并
    }
}

int main(){
    int n; cin >> n;
    for(int i = 0; i < n; i++) cin >> a[i];//输入5 4 3 2 1
    // SelectionSort(n);
    // BubbleSort(n);
    MergeSort(0, n - 1);
    for(int i = 0; i < n; i++) cout << a[i] << " ";//输出1 2 3 4 5
    cout << endl;
    return 0;
}