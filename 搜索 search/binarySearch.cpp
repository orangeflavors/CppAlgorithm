#include<bits/stdc++.h>
using namespace std;

// 整数域二分
// num为非递减序列
// 返回第一个不小于给定值的元素位置,即目标值的下界.如果值存在,返回第一个出现的位置,如果值不存在，返回第一个大于该值的位置
int lowerBound(vector<int>& nums, int target){
    int n = nums.size();
    int left = 0, right = n - 1, mid;
    // 左闭右闭[left, right]
    // 循环不变量
    // left - 1始终为<target
    // right + 1始终为>=target
    while(left <= right){ // 区间不为空
        mid = (left + right) / 2;
        // mid = left + (right - left) / 2; 防止整数溢出
        if(nums[mid] < target){
            left = mid + 1; // [mid + 1, right]
        }
        else{
            right = mid - 1; // [left, mid - 1]
        }
    }
    return left;// 或right + 1
    // 循环结束后 left == right+1
    // 此时 nums[left-1] < target 而 nums[left] = nums[right+1] >= target
    // 所以 left 就是第一个 >= target 的元素下标
}

int lowerBound2(vector<int>& nums, int target){
    int n = nums.size();
    int left = 0, right = n, mid;
    // 左闭右开[left, right)
    // 循环不变量
    // left - 1始终为<target
    // right始终为>=target
    while(left < right){ // 区间不为空
        mid = (left + right) / 2;
        // mid = left + (right - left) / 2; 防止整数溢出
        if(nums[mid] < target){
            left = mid + 1; // [mid + 1, right)
        }
        else{
            right = mid; // [left, mid)
        }
    }
    return left;// 或right
    // 循环结束后 left == right
    // 此时 nums[left-1] < target 而 nums[left] = nums[right] >= target
    // 所以 left 就是第一个 >= target 的元素下标
}

int lowerBound3(vector<int>& nums, int target){
    int n = nums.size();
    int left = -1, right = n, mid;
    // 左开右开(left, right)
    // 循环不变量
    // left始终为<target
    // right始终为>=target
    while(left + 1 < right){ // 区间不为空
        mid = (left + right) / 2;
        // mid = left + (right - left) / 2; 防止整数溢出
        if(nums[mid] < target){
            left = mid; // (mid, right)
        }
        else{
            right = mid; // (left, mid)
        }
    }
    return right;// 或left + 1
    // 循环结束后 left+1 == right
    // 此时 nums[left] < target 而 nums[right] >= target
    // 所以 right 就是第一个 >= target 的元素下标
}

/*
有3种取区间方法[], [), ()如上
有4种取目标方法>=, >, <=, <如下
需求	               写法	                   如果不存在
>=x的第一个元素的下标   lowerBound(nums,x)	    结果为 n
>x 的第一个元素的下标	lowerBound(nums,x+1)	结果为 n
<x 的最后一个元素的下标 lowerBound(nums,x)−1	结果为 −1
<=x的最后一个元素的下标	lowerBound(nums,x+1)−1	结果为 −1
*/

// 返回第一个大于给定值的元素位置,即目标值的上界.无论值是否存在,都返回第一个大于该值的位置
int upperBound(vector<int>& nums, int target){
    int n = nums.size();
    int left = 0, right = n - 1, mid;
    while(left <= right){
        mid = (left + right) / 2;
        if(nums[mid] <= target){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }
    return left;
}

// 最大值查找:在一个满足特定条件的范围内,找到最后一个满足条件的值(最大值)
// 与边界查找(如 lower_bound)不同,最大值查找的目标是找到满足条件的最后一个位置,而不是第一个位置
bool isValid(vector<int>& nums, int num, int target){}// 要满足的条件

int findMaxValid(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    int result = -1;  // 记录最后一个满足条件的值
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (isValid(nums, mid, target)) {  // 如果mid满足条件
            result = mid;      // 记录当前满足条件的值
            left = mid + 1;    // 尝试更大的值
        } else {
            right = mid - 1;   // 尝试更小的值
        }
    }
    
    return result;  // 返回最后一个满足条件的值
}