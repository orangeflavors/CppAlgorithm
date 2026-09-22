# `std::sort()` 在算法竞赛中的用法与注意事项

## 目录

1. [竞赛中的定位](#竞赛中的定位)
2. [快速上手模板](#快速上手模板)
3. [自定义排序：Lambda 是你的朋友](#自定义排序lambda-是你的朋友)
4. [高频模式](#高频模式)
5. [与常见算法组合](#与常见算法组合)
6. [必须注意的陷阱](#必须注意的陷阱)
7. [性能优化技巧](#性能优化技巧)
8. [C++17 并行排序：竞赛中慎用](#c17-并行排序竞赛中慎用)
9. [C++20 Ranges：什么时候用](#c20-ranges什么时候用)
10. [自测清单](#自测清单)

---

## 竞赛中的定位

在算法竞赛中，`std::sort()` 是**使用频率最高的标准库函数之一**。几乎所有需要"排序后处理"的题目——贪心、二分、双指针、离散化、扫描线——都离不开它。

| 特性 | 竞赛意义 |
|------|----------|
| O(N·log N) 最坏保证 | 不会像 C 的 `qsort()` 那样被卡 O(N²) |
| 元素开销小时常数极小 | 1e6 的 `int` 排序 < 0.3s |
| Lambda 内联 | 比较函数零额外调用开销 |
| C++11 起移动语义 | 大对象（如 `string`）交换几乎零开销 |

**经验法则**：只要元素数 ≤ 1e6，`std::sort()` 在 1s 时限内基本安全。

---

## 快速上手模板

### 最简模板——开箱即用

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> a(n);
    // 读入……
    sort(a.begin(), a.end());
    // a 已升序
}
```

### 降序排序

```cpp
// 方式 1：greater
sort(a.begin(), a.end(), greater<int>());

// 方式 2：lambda（推荐，竞赛中更直观）
sort(a.begin(), a.end(), [](int x, int y) { return x > y; });
```

### 对 pair 排序（竞赛超高频）

```cpp
vector<pair<int, int>> vp(n);

// 默认：按 first 升序，first 相同按 second 升序
sort(vp.begin(), vp.end());

// 按 second 升序
sort(vp.begin(), vp.end(),
     [](auto& a, auto& b) { return a.second < b.second; });

// 按 first 降序，first 相同按 second 升序
sort(vp.begin(), vp.end(), [](auto& a, auto& b) {
    if (a.first != b.first) return a.first > b.first;
    return a.second < b.second;
});
```

### 对 tuple 排序

```cpp
vector<tuple<int, int, int>> vt(n);

// 默认：按第 0 维 → 第 1 维 → 第 2 维 字典序升序
sort(vt.begin(), vt.end());
```

---

## 自定义排序：Lambda 是你的朋友

竞赛中**永远用 lambda**，不写函数对象，不写函数指针：

```cpp
// ✅ 竞赛推荐写法
sort(a.begin(), a.end(), [](int x, int y) { return x > y; });

// ❌ 别这样——多写代码，多犯错误
bool cmp(int x, int y) { return x > y; }
sort(a.begin(), a.end(), cmp);
```

lambda 的优势：
- **内联**：编译器可以完全内联比较逻辑，零调用开销
- **捕获变量**：可以直接使用外部变量，无需全局数组传参
- **紧凑**：写在调用的地方，代码更短

### 捕获外部信息

```cpp
// 按元素在另一个数组中的值排序
vector<int> a(n), priority(n);
sort(a.begin(), a.end(), [&](int x, int y) {
    return priority[x] < priority[y];
});

// 按到目标点 (tx, ty) 的曼哈顿距离排序
int tx, ty;
sort(points.begin(), points.end(), [&](auto& p1, auto& p2) {
    return abs(p1.first - tx) + abs(p1.second - ty) <
           abs(p2.first - tx) + abs(p2.second - ty);
});
```

### 自定义结构体排序

```cpp
struct Edge {
    int u, v, w;
};

vector<Edge> edges(m);

// 按边权升序（Kruskal MST 必备）
sort(edges.begin(), edges.end(),
     [](const Edge& a, const Edge& b) { return a.w < b.w; });
```

---

## 高频模式

### 模式 1：排序 + 去重（离散化前置）

```cpp
// 离散化三行代码
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());

// 查找离散化后的值
int id = lower_bound(a.begin(), a.end(), x) - a.begin();
```

### 模式 2：对下标排序（不改变原数组）

极其常用——保持原数组不动，只排索引：

```cpp
vector<int> a(n);
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);  // 0, 1, 2, ..., n-1

sort(idx.begin(), idx.end(), [&](int i, int j) {
    return a[i] < a[j];  // 按 a 的值排下标
});

// 现在 a[idx[0]] 是最小值，a[idx[1]] 是次小值……
// 想要"排序后的数组"：for (int i : idx) cout << a[i] << ' ';
```

### 模式 3：排序 + 贪心

```cpp
// 经典区间调度：按右端点排序
struct Interval { int l, r; };
vector<Interval> segs(n);

sort(segs.begin(), segs.end(),
     [](auto& a, auto& b) { return a.r < b.r; });

int ans = 0, last = -1e9;
for (auto& s : segs) {
    if (s.l >= last) {
        ans++;
        last = s.r;
    }
}
```

### 模式 4：排序判断字母异位词 / 排列

```cpp
string s, t;
sort(s.begin(), s.end());
sort(t.begin(), t.end());
bool anagram = (s == t);
```

### 模式 5：按多个关键字排序

```cpp
// 成绩单：总分降序，语文降序，学号升序
struct Stu { int id, ch, ma, en, tot; };

sort(a.begin(), a.end(), [](auto& a, auto& b) {
    if (a.tot != b.tot) return a.tot > b.tot;
    if (a.ch  != b.ch)  return a.ch  > b.ch;
    return a.id < b.id;
});
```

### 模式 6：按计算值排序（避免重复计算）

```cpp
// ❌ 差：比较函数中重复计算
sort(a.begin(), a.end(), [](int x, int y) {
    return expensive_func(x) < expensive_func(y);  // 每个元素被调用多次！
});

// ✅ 好：预计算，对下标/对 pair 排序
vector<pair<int, int>> v;  // {计算结果, 原值}
for (int x : a) v.emplace_back(expensive_func(x), x);
sort(v.begin(), v.end());  // 每个元素只计算一次
```

### 模式 7：部分排序——只需要 Top-K

```cpp
// 只需要最小的 K 个？用 partial_sort 而非 sort
partial_sort(a.begin(), a.begin() + k, a.end());
// 复杂度 O(N·log K)，比 O(N·log N) 更优

// 只需要中位数 / 第 K 小？用 nth_element
nth_element(a.begin(), a.begin() + k, a.end());
// 复杂度 O(N)，第 k 个元素就位，但不保证两边有序
```

---

## 与常见算法组合

### sort + lower_bound / upper_bound（二分查找）

```cpp
sort(a.begin(), a.end());

// 第一个 ≥ x 的位置
int lo = lower_bound(a.begin(), a.end(), x) - a.begin();

// 第一个 > x 的位置
int hi = upper_bound(a.begin(), a.end(), x) - a.begin();

// 等于 x 的元素个数
int cnt = hi - lo;

// 是否存在 x
bool exists = binary_search(a.begin(), a.end(), x);
```

### sort + 双指针

```cpp
// 两数之和：找 a[i] + a[j] == target
sort(a.begin(), a.end());
int l = 0, r = n - 1;
while (l < r) {
    int sum = a[l] + a[r];
    if (sum == target) { /* 找到了 */ break; }
    else if (sum < target) l++;
    else r--;
}
```

### sort + 前缀和

```cpp
sort(a.begin(), a.end());
vector<long long> pre(n + 1);
for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + a[i];
// O(1) 查询任意区间和
```

### sort 作为 set 的轻量替代

```cpp
// 有时候不需要 set 的动态插入/删除，只需要一个排序的去重数组：
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());
// 比 set 快得多（连续内存，无指针跳转）
```

---

## 必须注意的陷阱

### 陷阱 1：严格弱序——竞赛中最常见的排序 bug

**必须返回 `a < b` 的语义，而非 `a <= b`。**

```cpp
// ❌ 致命错误：a <= b 会在相等时返回 true
//    违反非自反性 → 未定义行为 → 可能 RE/TLE/WA
sort(a.begin(), a.end(), [](int x, int y) {
    return x <= y;  // 错！！！
});

// ✅ 正确
sort(a.begin(), a.end(), [](int x, int y) {
    return x < y;
});
```

**典型死法**：用 `<=` 排序后程序直接崩溃（访问越界）或死循环。

### 陷阱 2：排序后索引映射丢失

```cpp
// 常见错误场景：需要输出原始顺序
// ❌ 直接排序破坏了原始位置信息
sort(a.begin(), a.end());
// 现在不知道每个元素原来是第几个了

// ✅ 保留原始索引
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(), [&](int i, int j) {
    return a[i] < a[j];
});
// idx[0] 是最小元素在原数组中的位置
```

### 陷阱 3：比较函数不满足传递性

```cpp
// ❌ 危险：abs 排序的这个写法看似正确……
sort(a.begin(), a.end(), [](int x, int y) {
    return abs(x) < abs(y);
});
// 这个本身是 OK 的……

// ❌ 但这个不 OK：混合两个独立条件
sort(a.begin(), a.end(), [](int x, int y) {
    return x % 2 < y % 2 || x < y;
    // 可能产生: comp(a,b)=true, comp(b,c)=true, comp(a,c)=false
});

// ✅ 正确：明确优先级
sort(a.begin(), a.end(), [](int x, int y) {
    if (x % 2 != y % 2) return x % 2 < y % 2;  // 先按奇偶
    return x < y;  // 再按值
});
```

### 陷阱 4：浮点数排序——小心 NaN 和精度

```cpp
vector<double> a(n);

// ✅ 一般情况 OK
sort(a.begin(), a.end());

// ❌ 如果数据可能含 NaN —— 灾难
// NaN 的比较永远返回 false，破坏严格弱序
// 竞赛数据通常不含 NaN，但要注意输出端的浮点误差

// 浮点比较器：用 epsilon
sort(a.begin(), a.end(), [](double x, double y) {
    return x < y - 1e-9;  // 注意：这也不完美
});
```

竞赛中浮点排序的一个经验法则：**如果可以，转化为整数排序**（乘以 10^k 转为整型）。

```cpp
// 比如按 0.01 精度排序 → 乘 100 用整数排
vector<int> a_int(n);
for (int i = 0; i < n; i++) a_int[i] = round(a_double[i] * 100);
sort(a_int.begin(), a_int.end());
```

### 陷阱 5：对空范围排序

```cpp
vector<int> a;
sort(a.begin(), a.end());  // ✅ 安全，空范围是 no-op
// 但注意：begin() == end() 时必须安全
```

### 陷阱 6：`greater` 的类型必须匹配

```cpp
vector<int> a(n);
sort(a.begin(), a.end(), greater<int>());    // ✅

vector<long long> b(n);
sort(b.begin(), b.end(), greater<long long>());  // ✅
sort(b.begin(), b.end(), greater<int>());        // ❌ 类型不匹配！
```

推荐直接用 lambda 避免此类问题：

```cpp
sort(a.begin(), a.end(), [](auto& x, auto& y) { return x > y; });
```

### 陷阱 7：排序后依赖稳定性

```cpp
// 如果先按 A 排序，再按 B 排序——第二次排序会破坏第一次的顺序！
// 需要多级排序时，在一次 sort 中完成所有比较

// ❌ 错误做法
sort(a.begin(), a.end(), [](auto& x, auto& y) { return x.A < y.A; });
sort(a.begin(), a.end(), [](auto& x, auto& y) { return x.B < y.B; });
// 第一次按 A 的排序结果被第二次覆盖了

// ✅ 正确做法：一次完成
sort(a.begin(), a.end(), [](auto& x, auto& y) {
    if (x.B != y.B) return x.B < y.B;  // 主关键字
    return x.A < y.A;                   // 次关键字
});
```

---

## 性能优化技巧

### 技巧 1：对 `pair<int,int>` 而不是 `vector<int>` 排序

当需要把两个数组绑定排序时：

```cpp
// ❌ 较差：大量间接访问
vector<int> a(n), b(n);
vector<int> idx(n);
iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(), [&](int i, int j) { return a[i] < a[j]; });
// 每次比较都要通过 idx 间接访问 a

// ✅ 更好：手动打包
vector<pair<int, int>> ab(n);
for (int i = 0; i < n; i++) ab[i] = {a[i], b[i]};
sort(ab.begin(), ab.end());  // 缓存友好，比较在连续内存上
```

### 技巧 2：预分配 + 避免在比较中分配内存

```cpp
// ❌ 比较函数中分配 string 会极慢
sort(a.begin(), a.end(), [](const string& x, const string& y) {
    string xl = to_lower(x), yl = to_lower(y);  // 每次比较都分配内存！
    return xl < yl;
});

// ✅ 预计算
vector<pair<string, string>> v;  // {小写版本, 原串}
for (auto& s : a) {
    string lower = s;
    for (auto& c : lower) c = tolower(c);
    v.emplace_back(lower, s);
}
sort(v.begin(), v.end());
```

### 技巧 3：小数组不要用 `sort`

插入排序在 N ≤ 16 左右时比 `sort` 快，但竞赛中几乎不值得手写：

```cpp
// 竞赛中，小于 100 个元素直接用 sort，不用纠结
// 更大的场景才考虑 partial_sort / nth_element
```

### 技巧 4：不要对链表用 `sort` 的前置拷贝

如果题目本身就是 `list`，用 `list::sort()` 而非拷到 `vector` 再拷回来。不过竞赛中极少用 `list`，直接用 `vector` 就好。

### 技巧 5：用 `emplace_back` 而非 `push_back`

```cpp
// ✅ 小优化：少一次拷贝/移动
v.emplace_back(x, y);        // 直接构造 pair
// vs
v.push_back({x, y});         // 先构造临时 pair，再移动
```

### 技巧 6：I/O 优化比排序优化更重要

```cpp
// 这行代码带来的性能提升往往比纠结排序写法更大：
ios::sync_with_stdio(false);
cin.tie(nullptr);
```

---

## C++17 并行排序：竞赛中慎用

```cpp
#include <execution>
sort(execution::par, a.begin(), a.end());  // 并行排序
```

**竞赛中不推荐**。原因：

1. **多数 OJ 不支持**：评测机可能没有 TBB 库，`<execution>` 头文件可能不存在或链接失败
2. **小数据反而更慢**：线程创建开销在 1e5 以下得不偿失
3. **不可预测**：不同环境的线程数不同，本地和评测机表现不一致
4. **内存占用增加**：并行排序需要额外临时内存

**替代方案**：如果需要极致性能，考虑基数排序（整数）而非并行。

---

## C++20 Ranges：什么时候用

```cpp
// C++20 写法
ranges::sort(a, {}, &Person::score);  // 按 score 升序
ranges::sort(a, ranges::greater{}, &Person::score);  // 降序
```

**竞赛建议**：

- AtCoder 已全面支持 C++20，**大胆使用** `ranges::sort` + 投影
- Codeforces 也已支持 C++20
- 国内 OJ（洛谷、牛客等）多数支持 C++17，C++20 支持参差不齐
- **保守策略**：用 C++17 的 `std::sort` + lambda 最通用
- **冲分策略**：如果确定 OJ 支持 C++20，投影写法写起来更快且不易出错

### 投影对比

```cpp
// 传统写法
sort(a.begin(), a.end(), [](auto& x, auto& y) {
    return x.score > y.score;
});

// C++20 ranges：更短，更不容易写错比较方向
ranges::sort(a, ranges::greater{}, &Stu::score);
```

---

## 自测清单

写完排序逻辑后，过一遍这个清单：

- [ ] 比较函数用 `<`，**绝不用** `<=`
- [ ] 多级比较写在一个 lambda 里，不拆成多次 `sort`
- [ ] 没有在比较函数里修改数组/容器
- [ ] 比较函数的参数是 `const&` 而非值传递（大对象时重要）
- [ ] 元素数量为零时也能正常工作
- [ ] 如果需要原始顺序，已经把索引记下来了
- [ ] 浮点数没有 NaN
- [ ] 需要 O(N·log K) 而非 O(N·log N) 时，考虑 `partial_sort` / `nth_element`
- [ ] 用 lambda 时，捕获的是 `[&]` 且外部变量生命周期覆盖排序全过程
- [ ] I/O 解绑已写：`ios::sync_with_stdio(false); cin.tie(nullptr);`

---

## 速查卡

```cpp
// ═══════════════════════════════════════════
// 竞赛 std::sort 速查
// ═══════════════════════════════════════════

// 升序
sort(a.begin(), a.end());

// 降序
sort(a.begin(), a.end(), greater<>());           // C++14+
sort(a.begin(), a.end(), [](auto& x, auto& y) { return x > y; });

// 对下标排序
vector<int> idx(n); iota(idx.begin(), idx.end(), 0);
sort(idx.begin(), idx.end(), [&](int i, int j) { return a[i] < a[j]; });

// 多关键字
sort(a.begin(), a.end(), [](auto& x, auto& y) {
    if (x.a != y.a) return x.a < y.a;   // 第一关键字
    if (x.b != y.b) return x.b > y.b;   // 第二关键字
    return x.c < y.c;                    // 第三关键字
});

// 离散化
sort(a.begin(), a.end());
a.erase(unique(a.begin(), a.end()), a.end());

// 只取前 K 小
partial_sort(a.begin(), a.begin() + k, a.end());

// 第 K 小（不关心顺序）
nth_element(a.begin(), a.begin() + k, a.end());

// C++20 ranges
ranges::sort(a, {}, &Obj::field);              // 升序按成员
ranges::sort(a, ranges::greater{}, &Obj::field); // 降序按成员
```

---

*最后更新：2026-07*
