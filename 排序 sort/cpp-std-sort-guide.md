# C++ `std::sort()` 详解

## 目录

1. [概述](#概述)
2. [头文件与函数签名](#头文件与函数签名)
3. [基本用法](#基本用法)
4. [自定义排序规则](#自定义排序规则)
5. [复杂度与性能](#复杂度与性能)
6. [内部实现：Introsort](#内部实现introsort)
7. [稳定性与 `std::stable_sort`](#稳定性与-stdstable_sort)
8. [与其他排序算法对比](#与其他排序算法对比)
9. [C++11/14/17/20 的演进](#c11141720-的演进)
10. [常见陷阱与最佳实践](#常见陷阱与最佳实践)
11. [性能测试示例](#性能测试示例)

---

## 概述

`std::sort()` 是 C++ 标准库中最常用的排序算法，定义在 `<algorithm>` 头文件中。它采用 **Introsort（内省排序）** 作为内部实现，这是一种混合排序算法，结合了**快速排序（Quicksort）**、**堆排序（Heapsort）** 和**插入排序（Insertion Sort）** 三种算法的优点：

| 特性 | 说明 |
|------|------|
| **平均时间复杂度** | O(N·log N) |
| **最坏时间复杂度** | O(N·log N)（由堆排序兜底） |
| **空间复杂度** | O(log N)（递归栈） |
| **稳定性** | **不稳定**（相等元素的相对顺序可能改变） |
| **排序方式** | 原地排序（in-place） |
| **C++ 版本** | C++98 引入，后续标准持续优化 |

---

## 头文件与函数签名

```cpp
#include <algorithm>
```

### 签名一：默认升序（使用 `operator<`）

```cpp
template< class RandomIt >
void sort( RandomIt first, RandomIt last );
```

- `first`：指向待排序范围起始位置的随机访问迭代器
- `last`：指向待排序范围结束位置（**不包含在排序中**）的随机访问迭代器
- 要求元素类型支持 `operator<`

### 签名二：自定义比较函数

```cpp
template< class RandomIt, class Compare >
void sort( RandomIt first, RandomIt last, Compare comp );
```

- `comp`：二元谓词，签名为 `bool cmp(const Type1& a, const Type2& b)`
- 当 `a` 应该排在 `b` **之前**时返回 `true`
- 必须满足**严格弱序（Strict Weak Ordering）**

### 签名三（C++17 起）：执行策略

```cpp
template< class ExecutionPolicy, class RandomIt >
void sort( ExecutionPolicy&& policy, RandomIt first, RandomIt last );

template< class ExecutionPolicy, class RandomIt, class Compare >
void sort( ExecutionPolicy&& policy, RandomIt first, RandomIt last, Compare comp );
```

支持并行排序：

```cpp
#include <execution>
std::sort(std::execution::par, vec.begin(), vec.end());        // 并行
std::sort(std::execution::par_unseq, vec.begin(), vec.end());  // 并行+向量化
std::sort(std::execution::seq, vec.begin(), vec.end());        // 顺序（默认）
```

### 迭代器要求

`std::sort()` **必须**接受 **随机访问迭代器（RandomAccessIterator）**。以下容器可以直接使用：

| 容器 | 是否支持 | 说明 |
|------|----------|------|
| `std::vector` | ✅ | 最常用 |
| `std::array` | ✅ | 定长数组 |
| `std::deque` | ✅ | 双端队列 |
| C 风格数组 | ✅ | 指针本身是随机访问迭代器 |
| `std::string` | ✅ | 字符排序 |
| `std::list` | ❌ | 双向迭代器，请用 `list::sort()` |
| `std::forward_list` | ❌ | 前向迭代器，请用 `forward_list::sort()` |
| `std::set/map` | ❌ | 已排序，无需也无法排序 |
| `std::unordered_set/map` | ❌ | 桶结构，无法排序 |

---

## 基本用法

### 示例 1：对 `vector<int>` 升序排序

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};

    std::sort(v.begin(), v.end());

    for (int x : v) {
        std::cout << x << ' ';  // 输出: 1 2 3 4 5 6 7 8 9
    }
}
```

### 示例 2：对 C 风格数组排序

```cpp
#include <algorithm>
#include <iostream>

int main() {
    int arr[] = {5, 2, 8, 1, 9, 3};
    int n = sizeof(arr) / sizeof(arr[0]);

    std::sort(arr, arr + n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << ' ';  // 输出: 1 2 3 5 8 9
    }
}
```

### 示例 3：对子范围排序

```cpp
std::vector<int> v = {5, 2, 8, 1, 9, 3, 7, 4, 6};

// 只排序前 5 个元素
std::sort(v.begin(), v.begin() + 5);
// v: {1, 2, 5, 8, 9, 3, 7, 4, 6}
```

### 示例 4：降序排序（使用 `std::greater`）

```cpp
#include <functional>

std::vector<int> v = {5, 2, 8, 1, 9, 3};

// 方法 1：使用标准函数对象
std::sort(v.begin(), v.end(), std::greater<int>());

// 方法 2：使用 Lambda（C++11）
std::sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;
});
// 结果: 9 8 5 3 2 1
```

### 示例 5：对 `std::string` 排序

```cpp
std::vector<std::string> words = {"banana", "apple", "cherry", "date"};

std::sort(words.begin(), words.end());
// 结果: "apple", "banana", "cherry", "date"（字典序）
```

---

## 自定义排序规则

### Lambda 表达式（推荐，C++11+）

```cpp
// 按绝对值排序
std::vector<int> v = {-5, 2, -8, 1, -9, 3};

std::sort(v.begin(), v.end(), [](int a, int b) {
    return std::abs(a) < std::abs(b);
});
// 结果: 1, 2, 3, -5, -8, -9
```

### 函数指针

```cpp
bool compAbs(int a, int b) {
    return std::abs(a) < std::abs(b);
}

std::sort(v.begin(), v.end(), compAbs);
```

### 函数对象（Functor）

```cpp
struct AbsComparator {
    bool operator()(int a, int b) const {
        return std::abs(a) < std::abs(b);
    }
};

std::sort(v.begin(), v.end(), AbsComparator());
```

### 对结构体/类排序

```cpp
struct Person {
    std::string name;
    int age;
    double salary;
};

std::vector<Person> people = {
    {"Alice", 30, 50000.0},
    {"Bob",   25, 60000.0},
    {"Charlie", 35, 45000.0},
};

// 按年龄升序
std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.age < b.age;
});

// 按薪资降序
std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.salary > b.salary;
});

// 多级排序：先年龄升序，再薪资降序
std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    if (a.age != b.age) return a.age < b.age;
    return a.salary > b.salary;
});
```

### 使用 C++20 投影（`std::ranges::sort`）

C++20 引入了 ranges 版本，支持投影（projection），大幅简化成员排序：

```cpp
#include <algorithm>
#include <vector>

struct Person { std::string name; int age; };

std::vector<Person> people = {{"Alice", 30}, {"Bob", 25}, {"Charlie", 35}};

// C++20：按 age 升序排序——无需手写 lambda 提取成员！
std::ranges::sort(people, {}, &Person::age);

// C++20：按 age 降序
std::ranges::sort(people, std::ranges::greater{}, &Person::age);

// C++20：按 name 长度排序
std::ranges::sort(people, {}, [](const Person& p) { return p.name.size(); });
```

投影是 C++20 Ranges 最实用的特性之一——`&Person::age` 告诉 sort "在比较之前先把每个元素投影成 age"，然后对投影结果用默认的 `<` 进行比较。

---

## 复杂度与性能

### 时间复杂度

| 场景 | 复杂度 | 说明 |
|------|--------|------|
| 平均情况 | O(N·log N) | 快速排序主导 |
| 最坏情况 | O(N·log N) | 快排递归过深时切换为堆排序 |
| 最优情况 | O(N) | 使用三数取中且数据已接近有序（实际仍为 O(N·log N)） |

### 比较次数

C++ 标准对 `std::sort()` 的比较次数有明确要求：

- **平均**：约 N·log₂N 次比较
- **最坏**：不超过 N·log₂N + O(N) 次比较

### 空间复杂度

| 项目 | 大小 |
|------|------|
| 递归深度 | O(log N) |
| 辅助空间 | O(log N)（仅递归栈，不分配额外数组） |

### 影响性能的因素

1. **元素大小**：元素越大，交换成本越高
   - 缓解方案：排序指针而非对象本身
   ```cpp
   std::vector<LargeObject*> ptrs;
   // 或配合智能指针
   std::vector<std::unique_ptr<LargeObject>> ptrs;
   ```

2. **比较函数的开销**：复杂比较函数拖慢速度
   - 将比较所需数据缓存到元素中
   - 使用 inline lambda 避免函数调用开销

3. **缓存局部性**：`std::vector` 的连续内存布局在排序时更有优势

4. **分支预测**：简单、可预测的比较（如整数比较）比复杂字符串比较快得多

---

## 内部实现：Introsort

`std::sort()` 的主流实现采用 **Introsort（内省排序）**，由 David Musser 于 1997 年发明，解决了传统快速排序的最坏情况 O(N²) 问题。

### 算法流程

```
┌─────────────────────────────────────────────────────────────┐
│                    std::sort(first, last)                    │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
                   ┌─────────────────────┐
                   │   N <= 阈值(≈16) ?  │
                   └─────────────────────┘
                      │            │
                     YES          NO
                      │            │
                      ▼            ▼
              ┌──────────┐  ┌─────────────┐
              │ 插入排序  │  │  快速排序    │
              │(小数组)  │  │ (分区递归)   │
              └──────────┘  └──────┬──────┘
                                   │
                          ┌────────▼────────┐
                          │ 递归深度 >       │
                          │ 2·log₂(N) ?     │
                          └────────┬────────┘
                             │          │
                            YES        NO
                             │          │
                             ▼          │
                     ┌──────────┐       │
                     │  堆排序   │       │
                     │(兜底保证) │       │
                     └──────────┘       │
                              ▲         │
                              │         │
                              └────┬────┘
                                   │
                                   ▼
                            继续快排分区
```

### 三数取中（Median-of-Three）枢轴选择

为避免快排在已排序/逆序数据上的 O(N²) 退化，实现通常取 `[first, mid, last-1]` 三个位置的中位数作为枢轴：

```cpp
// 概念演示：三数取中
auto median_of_three = [](auto a, auto b, auto c, auto comp) {
    if (comp(a, b)) {
        if (comp(b, c)) return b;
        return comp(a, c) ? c : a;
    } else {
        if (comp(a, c)) return a;
        return comp(b, c) ? c : b;
    }
};
```

### 各阶段的作用

| 阶段 | 算法 | 作用 |
|------|------|------|
| 小数组 | 插入排序 | 小规模数据（N ≤ 16）上插入排序比快排更快（缓存友好，分支开销低） |
| 主排序 | 快速排序 | 平均 O(N·log N)，常数因子小，缓存局部性好 |
| 退化保护 | 堆排序 | 当递归深度超过 2·⌊log₂N⌋ 时，说明枢轴选择持续糟糕，切换为堆排序保证 O(N·log N) |

这就是 Introsort 名称的由来：算法"自省"递归深度，一旦发现快排在退化，立即切换策略。

---

## 稳定性与 `std::stable_sort`

### `std::sort()` — 不稳定排序

```cpp
struct Student { std::string name; int score; };

std::vector<Student> students = {
    {"Alice", 90},
    {"Bob",   85},
    {"Charlie", 90},
};

// 按分数排序（不稳定！）
std::sort(students.begin(), students.end(),
    [](const Student& a, const Student& b) { return a.score < b.score; });

// 可能的结果：
// {"Bob", 85}, {"Alice", 90}, {"Charlie", 90}
// 或
// {"Bob", 85}, {"Charlie", 90}, {"Alice", 90}  ← 顺序变了！
```

等价元素的相对顺序不保证保持。

### `std::stable_sort()` — 稳定排序

```cpp
// 稳定排序：等价元素保持原始相对顺序
std::stable_sort(students.begin(), students.end(),
    [](const Student& a, const Student& b) { return a.score < b.score; });

// 保证结果：{"Bob", 85}, {"Alice", 90}, {"Charlie", 90}  ← 顺序不变！
```

**实现**：`std::stable_sort()` 通常采用归并排序（Mergesort）。

| 指标 | `std::sort` | `std::stable_sort` |
|------|-------------|-------------------|
| 平均复杂度 | O(N·log N) | O(N·log N) |
| 最坏复杂度 | O(N·log N) | O(N·log²N)，或 O(N·log N)* |
| 额外内存 | O(log N) | O(N/2) 或 O(N) |
| 稳定性 | ❌ 不稳定 | ✅ 稳定 |
| 实际速度 | 更快 | 较慢（更大常数因子和内存开销） |

> \*如果内存充足可以实现 O(N·log N) 的 stable_sort，否则退化为 O(N·log²N)。

---

## 与其他排序算法对比

```cpp
#include <algorithm>   // sort, stable_sort, partial_sort, nth_element
#include <cstdlib>     // qsort
```

| 算法 | 复杂度（平均/最坏） | 稳定 | 内存 | 适用场景 |
|------|---------------------|------|------|----------|
| `std::sort` | O(N·log N) / O(N·log N) | ❌ | O(log N) | **通用排序，默认首选** |
| `std::stable_sort` | O(N·log N) / O(N·log²N) | ✅ | O(N) | 需要保持等价元素顺序 |
| `std::partial_sort` | O(N·log K) | ❌ | O(1) | 只关心前 K 个最小的 |
| `std::nth_element` | O(N) | ❌ | O(1) | 只需第 K 个元素就位 |
| `std::qsort` (C) | O(N·log N) / O(N²) | ❌ | O(log N) | C 兼容，**不推荐使用** |
| `std::make_heap` + `sort_heap` | O(N·log N) | ❌ | O(1) | 堆排序，最坏保证好 |
| `std::list::sort` | O(N·log N) | ✅ | O(1) | 链表专用 |

### 选择指南

```
需要排序全部元素？
├── 需要稳定？ ────────────▶ std::stable_sort()
├── 链表？ ───────────────▶ list::sort()
├── 只需要 Top-K？ ───────▶ std::partial_sort()
├── 只需要中位数/分位数？ ─▶ std::nth_element()
└── 通用场景 ─────────────▶ std::sort() ✅（最快）
```

---

## C++11/14/17/20 的演进

### C++11

- **移动语义**：元素交换优先使用移动而非拷贝，大幅提升大对象排序性能
- **Lambda 表达式**：内联自定义比较函数，无需单独定义函数对象
- **`std::begin/end`**：对 C 数组更友好

```cpp
int arr[] = {3, 1, 4, 1, 5};
std::sort(std::begin(arr), std::end(arr));
```

### C++14

- **泛型 Lambda**：比较器中使用 `auto` 参数

```cpp
std::sort(v.begin(), v.end(), [](auto& a, auto& b) {
    return a.weight < b.weight;
});
```

### C++17

- **执行策略（并行算法）**：

```cpp
#include <execution>

std::vector<int> v(1'000'000);
std::sort(std::execution::par, v.begin(), v.end());        // 并行
std::sort(std::execution::par_unseq, v.begin(), v.end());  // 并行 + SIMD
```

并行 sort 在大数据量（通常 > 10万元素）上可带来数倍加速。

### C++20

- **`std::ranges::sort`**：更安全、更简洁的 Range 版本
- **投影（Projection）**：无需手动 lambda 提取比较键

```cpp
#include <algorithm>

// C++20 以前
std::sort(v.begin(), v.end(), [](const auto& a, const auto& b) {
    return a.name < b.name;
});

// C++20 ranges + 投影
std::ranges::sort(v, {}, &Person::name);                    // 按 name 升序
std::ranges::sort(v, std::ranges::greater{}, &Person::age); // 按 age 降序
```

- **`std::ranges::partial_sort`**、**`std::ranges::stable_sort`** 等也一并引入

### C++23

- 进一步扩展 ranges 算法和视图适配

---

## 常见陷阱与最佳实践

### 陷阱 1：严格弱序（Strict Weak Ordering）违规

**比较函数必须满足严格弱序**，否则导致未定义行为——程序可能**崩溃、死循环、或产生错误结果**。

严格要求：

1. **非自反性（Irreflexivity）**：`comp(a, a)` 必须为 `false`
2. **非对称性（Asymmetry）**：若 `comp(a, b)` 为 `true`，则 `comp(b, a)` 必须为 `false`
3. **传递性（Transitivity）**：若 `comp(a, b) && comp(b, c)` 为 `true`，则 `comp(a, c)` 必须为 `true`
4. **等价传递性**：若 `!comp(a,b) && !comp(b,a)`，则 `a` 和 `b` 等价。等价关系也必须传递

```cpp
// ❌ 错误：等于时也返回 true，违反非自反性
std::sort(v.begin(), v.end(), [](int a, int b) {
    return a <= b;  // 当 a == b 时返回 true，这是错误的！
});

// ✅ 正确：严格小于
std::sort(v.begin(), v.end(), [](int a, int b) {
    return a < b;
});
```

```cpp
// ❌ 错误：违反传递性
struct Person { std::string name; int id; };
std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.name < b.name || a.id < b.id;
    // 可能发生: comp(a,b)=true, comp(b,c)=true, 但 comp(a,c)=false
});

// ✅ 正确：按优先级比较
std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    if (a.name != b.name) return a.name < b.name;
    return a.id < b.id;
});
```

### 陷阱 2：迭代器失效

排序过程中任何导致迭代器失效的操作都是未定义行为：

```cpp
// ❌ 危险：在比较函数中修改容器
std::sort(v.begin(), v.end(), [&v](int a, int b) {
    v.push_back(0);  // 可能触发 reallocation，使迭代器失效！
    return a < b;
});
```

### 陷阱 3：对关联容器使用 `std::sort`

```cpp
std::set<int> s = {3, 1, 4, 2};
// std::sort(s.begin(), s.end());  // ❌ 编译错误：set 迭代器不是随机访问的
// set 和 map 内部已排序，无需排序
```

### 陷阱 4：浮点数比较中的 NaN

NaN 会破坏严格弱序：

```cpp
std::vector<double> v = {1.0, NAN, 3.0, 2.0};
// std::sort(v.begin(), v.end());  // ❌ 未定义行为——NAN 的比较总返回 false
// 既不满足 <，也不满足 >=，所有与 NaN 的比较都返回 false
```

### 陷阱 5：未初始化的比较函数捕获

```cpp
// ❌ 危险：bool 返回但逻辑不正确
std::vector<int> v = {3, 1, 2};
auto cmp = [](int a, int b) -> bool {
    // 实际逻辑……
    // 如果所有路径不都返回值，行为未定义
};
```

### 最佳实践总结

| 原则 | 做法 |
|------|------|
| **用 Lambda** | C++11+ 优先用 lambda 而非函数指针（更易内联，性能更好） |
| **const 引用传参** | 比较函数参数用 `const T&`，避免不必要拷贝 |
| **标记 `const`** | 函数对象重载 `operator()` 时加 `const` |
| **用 `std::ranges::sort`** | C++20+ 优先使用 ranges 版本，配合投影更安全简洁 |
| **考虑 `stable_sort`** | 需要稳定排序时用 `stable_sort`，不要依赖 `sort` 的实现细节 |
| **大数据用并行** | C++17+ 对大规模数据使用 `std::execution::par` |
| **sort 指针/索引** | 对象很大时排序指针数组（或 `vector<unique_ptr>`）而非对象本身 |

---

## 性能测试示例

### 基本基准测试

```cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main() {
    constexpr size_t N = 1'000'000;
    std::vector<int> v(N);
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 1'000'000);

    auto benchmark = [&](const char* label, auto sort_fn) {
        // 每次重置数据
        for (auto& x : v) x = dist(rng);

        auto start = std::chrono::high_resolution_clock::now();
        sort_fn();
        auto end = std::chrono::high_resolution_clock::now();

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << label << ": " << ms << " ms\n";

        // 验证排序正确性
        std::cout << "  是否有序: " << std::is_sorted(v.begin(), v.end()) << '\n';
    };

    benchmark("std::sort (默认升序)", [&] {
        std::sort(v.begin(), v.end());
    });

    benchmark("std::sort (降序)", [&] {
        std::sort(v.begin(), v.end(), std::greater<int>());
    });

    benchmark("std::stable_sort", [&] {
        std::stable_sort(v.begin(), v.end());
    });

    // C++17 并行排序
    // benchmark("std::sort (并行)", [&] {
    //     std::sort(std::execution::par, v.begin(), v.end());
    // });
}
```

### 测试不同数据分布下的性能

```cpp
#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

template<typename F>
long long measure(F&& sort_fn) {
    auto start = std::chrono::high_resolution_clock::now();
    sort_fn();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}

int main() {
    constexpr size_t N = 100'000;
    std::vector<int> v(N);

    // 1. 随机数据
    std::iota(v.begin(), v.end(), 1);
    std::random_shuffle(v.begin(), v.end());
    auto v_rand = v;

    // 2. 已排序（升序）
    auto v_sorted = v;  std::sort(v_sorted.begin(), v_sorted.end());

    // 3. 已排序（降序）
    auto v_rev = v_sorted;
    std::reverse(v_rev.begin(), v_rev.end());

    // 4. 大量重复元素
    auto v_dup = v;
    for (auto& x : v_dup) x = x % 10;  // 只有 0~9

    std::cout << "数据分布对 std::sort 性能的影响（N=" << N << "）:\n\n";

    auto test = [&](const char* label, std::vector<int> data) {
        auto us = measure([&] { std::sort(data.begin(), data.end()); });
        std::cout << "  " << label << ": " << us << " μs\n";
    };

    test("随机",   v_rand);
    test("已升序", v_sorted);
    test("已降序", v_rev);
    test("重复多", v_dup);
}
```

典型输出（因平台而异）：

```
数据分布对 std::sort 性能的影响（N=100000）:
  随机:   8234 μs
  已升序: 2103 μs   ← 三数取中让已排序数据很快
  已降序: 2198 μs   ← 同样受益于三数取中
  重复多: 4521 μs   ← 重复元素可提前终止分区
```

---

## 扩展阅读

- [cppreference: std::sort](https://en.cppreference.com/w/cpp/algorithm/sort)
- [cppreference: std::stable_sort](https://en.cppreference.com/w/cpp/algorithm/stable_sort)
- [cppreference: std::ranges::sort](https://en.cppreference.com/w/cpp/algorithm/ranges/sort)
- [Musser, David R. "Introspective Sorting and Selection Algorithms" (1997)](https://doi.org/10.1002/(SICI)1097-024X(199708)27:8<983::AID-SPE117>3.0.CO;2-%23) — Introsort 原始论文
- C++ ISO Standard: [N4950](https://wg21.link/n4950) §27.8.2 (Sorting and related operations)
