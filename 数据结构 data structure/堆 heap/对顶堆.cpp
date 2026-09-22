#include<bits/stdc++.h>
using namespace std;
using ll = long long;

// 对顶堆 多重集写法
class MedianFinder{
public:
    ll sum_left, sum_right;
    multiset<ll> left, right;

    void clear(){
        left.clear();
        right.clear();
        sum_left = 0;
        sum_right = 0;
    }

    // 调平
    void remake(){
        while(left.size() > right.size()){
            right.insert(*left.rbegin());
            sum_right += *left.rbegin();
            sum_left -= *left.rbegin();
            left.erase(prev(left.end()));
        }
        while(left.size() < right.size()){
            left.insert(*right.begin());
            sum_left += *right.begin();
            sum_right -= *right.begin();
            right.erase(right.begin());
        }
    }

    // 添加元素
    void add(ll num){
        left.insert(num);
        sum_left += num;
        remake();
    }

    // 删除元素
    void del(ll num){
        if(left.find(num) != left.end()){
            left.erase(left.lower_bound(num));
            sum_left -= num;
        }
        else{
            right.erase(right.lower_bound(num));
            sum_right -= num;
        }
        remake();
    }

    // 返回中位数的 2 倍，避免精度误差
    ll findMedian(){
        if(left.empty()) return 0;
        if(left.size() > right.size()){
            return 2 * (*left.rbegin());
        }
        return (*left.rbegin()) + (*right.begin());
    }
};


// 懒删除堆
template<typename T, typename Compare = less<T>>
class LazyHeap {
    priority_queue<T, vector<T>, Compare> pq;
    unordered_map<T, int> remove_cnt; // 每个元素剩余需要删除的次数
    size_t sz = 0; // 堆的实际大小

    // 正式执行删除操作
    void apply_remove() {
        while (!pq.empty() && remove_cnt[pq.top()] > 0) {
            remove_cnt[pq.top()]--;
            pq.pop();
        }
    }

public:
    // 删除堆中所有应该删除的元素后，堆的实际大小
    size_t size() {
        return sz;
    }

    // 删除
    void remove(T x) {
        remove_cnt[x]++; // 懒删除
        sz--;
    }

    // 查看堆顶
    T top() {
        apply_remove();
        return pq.top(); // 真正的堆顶
    }

    // 出堆
    T pop() {
        apply_remove();
        sz--;
        T x = pq.top();
        pq.pop();
        return x;
    }

    // 入堆
    void push(T x) {
        if (remove_cnt[x] > 0) {
            remove_cnt[x]--; // 抵消之前的删除
        } else {
            pq.push(x);
        }
        sz++;
    }
};

// 对顶堆 堆写法
// 使用懒删除堆实现删除
class MedianFinder2{
public:
    LazyHeap<ll> left;// 大顶堆
    LazyHeap<ll, greater<>> right;// 小顶堆

    // 调平
    void remake(){
        while(left.size() > right.size()){
            right.push(left.pop());
        }        
        while(left.size() < right.size()){
            left.push(right.pop());
        }
    }

    // 添加元素
    void add(ll num){
        left.push(num);
        remake();
    }

    // 删除元素
    void del(ll num){
        if(num > left.top()){
            right.remove(num);
        }
        else{
            left.remove(num);
        }
        remake();
    }

    // 返回中位数的 2 倍，避免精度误差
    ll findMedian(){
        if(left.size() > right.size()){
            return 2 * left.top();
        }
        return left.top() + right.top();
    }
};