#include<bits/stdc++.h>

using namespace std;

//平面上 n 个点,求任意两点的曼哈顿距离的最大值
//时间复杂度O(n)
int maxManhattanDistance(const vector<pair<int, int>>& points) {
    int max1 = INT_MIN/2, min1 = INT_MAX/2;
    int max2 = INT_MIN/2, min2 = INT_MAX/2;
    
    for (const auto& point : points) {
        int x = point.first;
        int y = point.second;
        
        // 更新 max1 和 min1
        int f1 = x + y;
        max1 = max(max1, f1);
        min1 = min(min1, f1);
        
        // 更新 max2 和 min2
        int f2 = x - y;
        max2 = max(max2, f2);
        min2 = min(min2, f2);
    }
 
    int d1 = max1 - min1;
    int d2 = max2 - min2;
 
    return max(d1, d2);
}
 
int main() {
    // 示例输入
    vector<pair<int, int>> points = {{1, 2}, {3, 4}, {6, 1}, {-1, -3}};
 
    int maxDistance = maxManhattanDistance(points);
    cout << "Maximum Manhattan Distance: " << maxDistance << endl;
 
    return 0;
}