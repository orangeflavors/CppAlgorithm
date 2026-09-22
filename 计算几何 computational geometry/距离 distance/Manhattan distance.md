#### 高效求解n个点之间的最大曼哈顿距离

平面上有n个点，如何求出任意两点的曼哈顿距离的最大值?
曼哈顿距离的公式为：
$$
d((x_1, y_1), (x_2, y_2)) = |x_1 - x_2| + |y_1 - y_2|
$$
为了最大化曼哈顿距离，可以考虑绝对值展开的所有情况，我们可以考虑以下四个表达式：
对于点 $(x_i, y_i)$ 和 $(x_j, y_j)$，曼哈顿距离$|x_i - x_j| + |y_i - y_j|$可以转化为以下四种形式之一：
1. $(x_i + y_i) - (x_j + y_j)$
2. $(x_i - y_i) - (x_j - y_j)$
3. $-(x_i + y_i) + (x_j + y_j)$
4. $-(x_i - y_i) + (x_j - y_j)$

那么我们只需要跟踪这两个表达式的最大值和最小值：
1. $f_1(x, y) = x + y$
2. $f_2(x, y) = x - y$

对于每个表达式，我们可以找到其最大值和最小值，然后计算这些最大值和最小值之间的差值。
1. 初始化四组变量，用来记录上述四个表达式的最大值和最小值：
+ $max1=min1=x_1+y_1$
+ $max2=min2=x_1-y_1$
1. 遍历所有点 $(x_i,y_i)$，更新这四个值
2. 计算两组差值的最大值：
+ $d1 = max1 - min1$
+ $d2 = max2 - min2$
两个值实际上代表了所有四种绝对值展开的情况，几何上即线段从左下到右上和从左上到右下的两种情况。
3. 曼哈顿距离的最大值就是最大值： $maxdistance = max(d1, d2)$
这个算法的时间复杂度为 $O(n)$，因为只需遍历所有点一次即可完成所有更新和计算。

```cpp
int maxManhattanDistance(vector<pair<int, int>>& points){
    int max1 = INT_MIN, max2 = INT_MIN, min1 = INT_MAX, min2 = INT_MAX;

    for(auto [x, y] : points){
        max1 = max(max1, x + y);
        min1 = min(min1, x + y);
        max2 = max(max2, x - y);
        min2 = min(min2, x - y);
    }

    return max(max1 - min1, max2 - min2);
}
```

[codeforces 2122 C](https：//codeforces.com/contest/2122/problem/C)
最大化曼哈顿距离和
