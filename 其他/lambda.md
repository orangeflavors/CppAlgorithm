# 系统讲解 C++ 中的 Lambda 表达式
好的，我们来详细、系统地讲解一下 C++ 中的 Lambda 表达式。

Lambda 表达式是 C++11 标准引入的一个重要特性，它允许你就地定义匿名函数对象。这使得代码更加简洁、清晰，特别是在需要传递短小的函数逻辑给 STL 算法时，不再需要额外定义一个函数或者函数对象。

---

### 1. Lambda 表达式的基本语法

一个完整的 Lambda 表达式的语法如下：

```cpp
[capture_list] (parameter_list) -> return_type {
    function_body
}
```

让我们逐一分解每个部分：

*   **`[capture_list]` （捕获列表）**：
    *   位于 Lambda 表达式的开头。
    *   它定义了 Lambda 体内部可以访问哪些**外部作用域**的变量，以及如何访问它们（按值还是按引用）。
    *   这是 Lambda 与普通函数最核心的区别之一。

*   **`(parameter_list)` （参数列表）**：
    *   和普通函数的参数列表一样，指定 Lambda 函数接收哪些参数。
    *   如果不需要参数，可以省略，写成 `[]{}`，但通常保留空括号 `[](){}` 以增加可读性。

*   **`-> return_type` （返回类型）**：
    *   指定 Lambda 表达式的返回类型。
    *   **在 C++11 中，如果函数体只包含一个 `return` 语句，或者没有返回值（返回 `void`），编译器可以自动推导返回类型，此时可以省略 `-> return_type`**。
    *   在 C++14 中，返回类型推导的能力更强，通常都可以省略。

*   **`{ function_body }` （函数体）**：
    *   和普通函数一样，包含 Lambda 被调用时要执行的代码。

---

### 2. 捕获列表 `[capture_list]` 详解

捕获列表是 Lambda 的核心与难点。它规定了 Lambda 如何“捕获”外部的变量。

#### 2.1 主要的捕获方式：

*   **`[]` （不捕获任何变量）**：
    *   Lambda 体内不能使用任何外部变量，只能使用参数和全局变量。
    ```cpp
    int global = 10;
    void func() {
        int local = 20;
        auto f = []() { return global; }; // 正确，可以使用全局变量
        // auto f2 = []() { return local; }; // 错误！不能捕获局部变量 local
    }
    ```

*   **`[=]` （按值捕获所有外部变量）**：
    *   Lambda 体内会获得所有外部变量的**副本**。在 Lambda 内部修改这些副本不会影响外部的原始变量。
    *   **注意**：按值捕获的变量在 Lambda 被创建时就已经固定了，而不是在调用时。
    ```cpp
    int a = 1, b = 2;
    auto f = [=]() { return a + b; }; // 捕获创建时 a 和 b 的值
    a = 10; // 修改外部 a
    std::cout << f(); // 输出 3 (1+2)，而不是 12
    ```

*   **`[&]` （按引用捕获所有外部变量）**：
    *   Lambda 体内使用的是外部变量的**引用**。修改这些变量会影响外部的原始变量。
    ```cpp
    int a = 1, b = 2;
    auto f = [&]() { a = 10; b = 20; };
    f();
    std::cout << a << ", " << b; // 输出 10, 20
    ```

*   **`[var]` （按值捕获特定变量 `var`）**：
    *   只按值捕获指定的变量 `var`，其他变量不捕获。
    ```cpp
    int a = 1, b = 2, c = 3;
    auto f = [a, b]() { return a + b; }; // 只捕获 a 和 b
    // auto f2 = [a, b]() { return a + c; }; // 错误！c 未被捕获
    ```

*   **`[&var]` （按引用捕获特定变量 `var`）**：
    *   只按引用捕获指定的变量 `var`。
    ```cpp
    int a = 1, b = 2;
    auto f = [&a]() { a = 100; }; // 只按引用捕获 a
    f();
    std::cout << a; // 输出 100
    ```

*   **混合捕获**：
    *   可以组合使用上述方式，用逗号分隔。
    *   `[=, &var]`：默认按值捕获，但对变量 `var` 按引用捕获。
    *   `[&, var]`：默认按引用捕获，但对变量 `var` 按值捕获。
    ```cpp
    int a = 1, b = 2, c = 3;
    auto f1 = [=, &c]() { c = a + b; }; // a, b 是副本，c 是引用
    auto f2 = [&, a]() { b = a + c; };  // b, c 是引用，a 是副本
    ```

#### 2.2 `mutable` 关键字

*   默认情况下，按值捕获的变量在 Lambda 体内部是 **`const`** 的，不能被修改。
*   使用 `mutable` 关键字可以移除这个 const 限制，允许修改按值捕获的变量的**副本**。
*   **注意**：`mutable` 不会影响外部的原始变量。

```cpp
int count = 0;
// 错误：没有 mutable，不能修改按值捕获的 count
// auto f = [count]() { count++; };

// 正确：使用 mutable
auto f = [count]() mutable { 
    count++; 
    std::cout << "Inner: " << count << std::endl;
};
f(); // 输出 "Inner: 1"
f(); // 输出 "Inner: 2"
std::cout << "Outer: " << count << std::endl; // 输出 "Outer: 0"，外部 count 未变
```

---

### 3. Lambda 表达式的类型与存储

Lambda 表达式的类型是一个**唯一的、未命名的、编译器生成的函数对象类型**（闭包类型）。你不能直接写出它的类型，必须使用 `auto` 来声明。

```cpp
auto lambda = []() { std::cout << "Hello"; };
```

如果你想存储 Lambda 或者将其作为参数传递，有几种方式：

1.  **使用 `auto`**（最简单，最常用）。
2.  **使用 `std::function`**：
    *   `std::function` 是一个通用的、可调用的目标包装器。它可以存储任何可调用对象（函数、函数指针、Lambda、bind 表达式等）。
    *   当你需要显式声明类型或将不同类型的 Lambda 存入同一容器时，非常有用。

```cpp
#include <functional>

// 声明一个接收 int，返回 bool 的 callable 对象
std::function<bool(int)> filter;

// 存储一个 Lambda
filter = [](int x) { return x > 10; };

// 存储另一个 Lambda
filter = [](int x) { return x % 2 == 0; };

std::vector<std::function<void()>> tasks;
tasks.push_back([]{ std::cout << "Task1\n"; });
tasks.push_back([]{ std::cout << "Task2\n"; });
```

---

### 4. Lambda 的高级用法与新标准特性

#### 4.1 C++14：泛型 Lambda 和初始化捕获

*   **泛型 Lambda**：
    *   可以使用 `auto` 作为参数类型，让 Lambda 成为模板函数。
    ```cpp
    auto adder = [](auto a, auto b) { return a + b; };
    std::cout << adder(1, 2) << std::endl;     // 3 (int)
    std::cout << adder(1.5, 2.3) << std::endl; // 3.8 (double)
    std::string s1 = "Hello, ", s2 = "World!";
    std::cout << adder(s1, s2) << std::endl;   // "Hello, World!" (std::string)
    ```

*   **初始化捕获（又称广义 Lambda 捕获）**：
    *   允许你在捕获列表中直接初始化成员变量。这对于移动捕获或使用表达式初始化捕获变量非常有用。
    ```cpp
    std::unique_ptr<int> ptr = std::make_unique<int>(42);
    
    // C++11 无法直接移动捕获，需要借助 std::bind
    // C++14 可以：
    auto f = [p = std::move(ptr)]() { // 将 ptr 移动到 Lambda 内部的 p
        std::cout << *p << std::endl;
    };
    f();
    // 此时 ptr 为 nullptr
    ```

#### 4.2 C++17：`constexpr` Lambda

*   如果 Lambda 符合要求（例如，函数体可以在编译期计算），它会被隐式地声明为 `constexpr`。
*   你也可以显式地使用 `constexpr` 关键字。

```cpp
// 隐式 constexpr
auto square = [](int n) { return n * n; };
static_assert(square(5) == 25); // 在编译期计算

// 显式 constexpr
auto constexpr_max = [](int a, int b) constexpr -> int {
    return a > b ? a : b;
};
```

#### 4.3 C++20：模板参数列表和 `consteval`

*   **模板语法**：C++20 允许在 Lambda 上使用熟悉的模板语法。
    ```cpp
    // C++14 泛型 Lambda
    auto f1 = [](auto vec) { return vec.size(); };

    // C++20 模板 Lambda
    auto f2 = []<typename T>(std::vector<T> vec) { return vec.size(); };
    ```

*   **`consteval`**：指定 Lambda 必须是编译时常量。
    ```cpp
    auto f = []() consteval { return 42; };
    ```

---

### 5. Lambda 的典型使用场景

1.  **STL 算法**：这是 Lambda 最经典的用法。
    ```cpp
    std::vector<int> nums = {5, 2, 8, 1, 9};
    // 排序
    std::sort(nums.begin(), nums.end(), [](int a, int b) { return a > b; });
    // 查找
    auto it = std::find_if(nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });
    // 计数
    int count = std::count_if(nums.begin(), nums.end(), [](int x) { return x > 5; });
    ```

2.  **异步编程与回调**：
    ```cpp
    std::thread t([&]() {
        // ... 做一些工作，可能用到外部变量
    });
    t.join();

    std::async(std::launch::async, [](int x) { return x*x; }, 10);
    ```

3.  **自定义比较器或谓词**：
    ```cpp
    std::map<std::string, int, decltype([](const std::string& a, const std::string& b) {
        return a.size() < b.size(); // 按字符串长度排序
    })> myMap;
    ```

### 总结

| 特性 | 说明 |
| :--- | :--- |
| **语法** | `[capture](params) -> ret { body }` |
| **捕获列表** | 定义如何访问外部变量 (`=`, `&`, `[x]`, `[&x]` 等) |
| **`mutable`** | 允许修改按值捕获的变量的**副本** |
| **类型** | 唯一的匿名类型，用 `auto` 或 `std::function` 存储 |
| **C++14** | 泛型 Lambda (`auto` 参数)，初始化捕获 |
| **C++17** | `constexpr` Lambda（通常隐式） |
| **C++20** | 模板 Lambda，`consteval` Lambda |

Lambda 表达式极大地提升了 C++ 在现代编程中的表达力，让函数式编程风格在 C++ 中变得更加自然和高效。熟练掌握它是编写现代 C++ 代码的关键。



# 在 C++ 的 `std::sort` 函数中使用 Lambda 表达式
在 C++ 的 `std::sort` 函数中使用 Lambda 表达式是非常常见和强大的用法。Lambda 允许你**就地定义自定义排序规则**，而不需要额外编写比较函数或函数对象。

## 基本用法

### 1. 降序排序（最简单的例子）

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums = {5, 2, 8, 1, 9, 3};
    
    // 使用 Lambda 进行降序排序
    std::sort(nums.begin(), nums.end(), [](int a, int b) {
        return a > b; // 如果 a > b，a 应该排在 b 前面
    });
    
    // 输出：9 8 5 3 2 1
    for (int n : nums) {
        std::cout << n << " ";
    }
    return 0;
}
```

### 2. 升序排序（显式定义）

```cpp
std::vector<int> nums = {5, 2, 8, 1, 9, 3};

// 升序排序 - 其实和默认排序一样，这里只是演示
std::sort(nums.begin(), nums.end(), [](int a, int b) {
    return a < b; // 如果 a < b，a 应该排在 b 前面
});
```

## Lambda 在 sort 中的工作原理

**核心规则**：Lambda 应该返回一个布尔值，表示**第一个参数是否应该在第二个参数之前**。
- 返回 `true`：`a` 应该在 `b` 之前
- 返回 `false`：`a` 不应该在 `b` 之前

## 实用的高级示例

### 3. 对自定义对象排序

```cpp
#include <algorithm>
#include <vector>
#include <string>

struct Person {
    std::string name;
    int age;
    double salary;
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25, 50000},
        {"Bob", 30, 60000},
        {"Charlie", 22, 45000},
        {"Diana", 30, 65000}
    };
    
    // 按年龄升序排序
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;
    });
    
    // 按薪水降序排序
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.salary > b.salary;
    });
    
    // 多级排序：先按年龄，年龄相同按薪水降序
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        if (a.age != b.age) {
            return a.age < b.age; // 年龄不同，按年龄升序
        }
        return a.salary > b.salary; // 年龄相同，按薪水降序
    });
    
    return 0;
}
```

### 4. 按字符串长度排序

```cpp
#include <algorithm>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> words = {"apple", "cat", "elephant", "bee", "dog"};
    
    // 按字符串长度排序（短的在前面）
    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    });
    
    // 输出：cat bee dog apple elephant
    for (const auto& word : words) {
        std::cout << word << " ";
    }
    
    return 0;
}
```

### 5. 使用捕获列表

```cpp
#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int pivot = 5;
    
    // 使用捕获：将小于 pivot 的数放在前面，大于的放在后面
    std::sort(nums.begin(), nums.end(), [pivot](int a, int b) {
        bool a_less = (a < pivot);
        bool b_less = (b < pivot);
        
        if (a_less && !b_less) return true;  // a 在前
        if (!a_less && b_less) return false; // b 在前
        return a < b; // 都在同一边，正常排序
    });
    
    // 输出：1 2 3 4 5 6 7 8 9 10（小于5的在前且有序，大于等于5的在后且有序）
    for (int n : nums) {
        std::cout << n << " ";
    }
    
    return 0;
}
```

### 6. 对指针或智能指针容器排序

```cpp
#include <algorithm>
#include <vector>
#include <memory>

struct Item {
    int value;
    Item(int v) : value(v) {}
};

int main() {
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<Item>(30));
    items.push_back(std::make_unique<Item>(10));
    items.push_back(std::make_unique<Item>(20));
    
    // 对智能指针容器排序，需要解引用
    std::sort(items.begin(), items.end(), [](const std::unique_ptr<Item>& a, const std::unique_ptr<Item>& b) {
        return a->value < b->value;
    });
    
    return 0;
}
```

## 性能考虑

1. **尽量按引用传递参数**，特别是对于大型对象：
   ```cpp
   // 好：按 const 引用传递，避免拷贝
   std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
       return a.age < b.age;
   });
   
   // 不好：按值传递，可能产生不必要的拷贝
   std::sort(people.begin(), people.end(), [](Person a, Person b) {
       return a.age < b.age;
   });
   ```

2. **简单的 Lambda 通常会被编译器内联**，性能与手写比较函数相当。

## 完整示例：综合运用

```cpp
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

struct Student {
    std::string name;
    int score;
    int age;
};

int main() {
    std::vector<Student> students = {
        {"Alice", 85, 20},
        {"Bob", 92, 19},
        {"Charlie", 85, 21},
        {"Diana", 78, 20},
        {"Eve", 92, 19}
    };
    
    // 多级排序：先按分数降序，分数相同按年龄升序，年龄相同按名字升序
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        if (a.score != b.score) {
            return a.score > b.score; // 分数高的在前
        }
        if (a.age != b.age) {
            return a.age < b.age;     // 年龄小的在前
        }
        return a.name < b.name;       // 名字字典序
    });
    
    // 输出结果
    for (const auto& student : students) {
        std::cout << student.name << " (Score: " << student.score 
                  << ", Age: " << student.age << ")" << std::endl;
    }
    
    return 0;
}
```

## 总结

在 `std::sort` 中使用 Lambda 表达式的要点：

1. **返回值**：返回 `true` 表示第一个参数应该在第二个参数之前
2. **参数传递**：使用 `const &` 避免不必要的拷贝
3. **多级排序**：使用 if-else 逻辑实现复杂的排序规则
4. **捕获列表**：可以在 Lambda 中访问外部变量
5. **类型安全**：编译器会检查 Lambda 参数类型与容器元素类型的兼容性

Lambda 让自定义排序变得非常简洁和直观，是现代 C++ 编程中不可或缺的工具。



# 在 STL 容器中使用 Lambda 表达式

是的，`map`、`set`、`multimap`、`multiset` 和 `priority_queue` 这些 STL 数据结构都可以使用 lambda 表达式，主要作为**自定义比较器**。下面我详细讲解每种容器的使用方法。

## 🗺️ 1. Map 和 Set 系列

### 基本用法

这些关联容器在构造时可以接受一个比较器对象，用于定义元素的排序规则。

```cpp
#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    // 1. 使用 Lambda 作为 map 的比较器
    auto map_cmp = [](const std::string& a, const std::string& b) {
        return a.length() < b.length(); // 按字符串长度排序
    };
    
    std::map<std::string, int, decltype(map_cmp)> length_map(map_cmp);
    length_map["apple"] = 1;
    length_map["banana"] = 2;
    length_map["cherry"] = 3;
    
    // 输出：按长度排序：apple(5) -> cherry(6) -> banana(6)
    for (const auto& pair : length_map) {
        std::cout << pair.first << "(" << pair.first.length() << ") -> " 
                  << pair.second << std::endl;
    }
    
    // 2. 使用 Lambda 作为 set 的比较器
    auto set_cmp = [](int a, int b) {
        return a > b; // 降序排序
    };
    
    std::set<int, decltype(set_cmp)> descending_set(set_cmp);
    descending_set.insert({3, 1, 4, 1, 5, 9});
    
    // 输出：9 5 4 3 1
    for (int n : descending_set) {
        std::cout << n << " ";
    }
    
    return 0;
}
```

### 使用捕获的 Lambda

```cpp
#include <map>
#include <string>
#include <iostream>

int main() {
    std::string base = "prefix_";
    
    // Lambda 捕获外部变量
    auto cmp_with_prefix = [base](const std::string& a, const std::string& b) {
        std::string a_with_prefix = base + a;
        std::string b_with_prefix = base + b;
        return a_with_prefix < b_with_prefix;
    };
    
    std::map<std::string, int, decltype(cmp_with_prefix)> prefixed_map(cmp_with_prefix);
    prefixed_map["zebra"] = 1;
    prefixed_map["apple"] = 2;
    prefixed_map["monkey"] = 3;
    
    // 输出会按 "prefix_apple", "prefix_monkey", "prefix_zebra" 排序
    for (const auto& pair : prefixed_map) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }
    
    return 0;
}
```

## 🔄 2. Multimap 和 Multiset

用法与普通 map/set 类似，但允许多个相同键的元素：

```cpp
#include <map>
#include <set>
#include <iostream>

int main() {
    // 1. Multimap 使用 Lambda 比较器
    auto multi_cmp = [](const std::string& a, const std::string& b) {
        // 忽略大小写比较
        std::string a_lower, b_lower;
        for (char c : a) a_lower += std::tolower(c);
        for (char c : b) b_lower += std::tolower(c);
        return a_lower < b_lower;
    };
    
    std::multimap<std::string, int, decltype(multi_cmp)> case_insensitive_map(multi_cmp);
    case_insensitive_map.insert({"Apple", 1});
    case_insensitive_map.insert({"apple", 2});
    case_insensitive_map.insert({"BANANA", 3});
    case_insensitive_map.insert({"banana", 4});
    
    // "Apple" 和 "apple" 被视为相同键
    for (const auto& pair : case_insensitive_map) {
        std::cout << pair.first << " -> " << pair.second << std::endl;
    }
    
    // 2. Multiset 使用 Lambda 比较器
    auto set_multi_cmp = [](int a, int b) {
        // 按绝对值排序
        return std::abs(a) < std::abs(b);
    };
    
    std::multiset<int, decltype(set_multi_cmp)> abs_set(set_multi_cmp);
    abs_set.insert({-3, 2, -1, 3, -2, 1});
    
    // 输出：1, -1, 2, -2, -3, 3 (按绝对值排序)
    for (int n : abs_set) {
        std::cout << n << " ";
    }
    
    return 0;
}
```

## 🎯 3. Priority Queue

`priority_queue` 的使用方式略有不同，需要在模板参数和构造函数中都指定比较器。

### 基本用法

```cpp
#include <queue>
#include <vector>
#include <iostream>

int main() {
    // 1. 最小堆（默认是最大堆）
    auto min_heap_cmp = [](int a, int b) {
        return a > b; // 注意：这里返回 a > b 表示小的优先级高
    };
    
    // priority_queue 的模板参数：<元素类型, 底层容器类型, 比较器类型>
    std::priority_queue<int, std::vector<int>, decltype(min_heap_cmp)> 
        min_heap(min_heap_cmp);
    
    min_heap.push(5);
    min_heap.push(2);
    min_heap.push(8);
    min_heap.push(1);
    
    // 输出：1 2 5 8 (从小到大)
    while (!min_heap.empty()) {
        std::cout << min_heap.top() << " ";
        min_heap.pop();
    }
    std::cout << std::endl;
    
    return 0;
}
```

### 复杂数据类型的优先队列

```cpp
#include <queue>
#include <vector>
#include <string>
#include <iostream>

struct Task {
    std::string name;
    int priority;
    int duration;
    
    Task(const std::string& n, int p, int d) : name(n), priority(p), duration(d) {}
};

int main() {
    // 自定义比较：先按优先级（高优先），优先级相同按持续时间（短优先）
    auto task_cmp = [](const Task& a, const Task& b) {
        if (a.priority != b.priority) {
            return a.priority < b.priority; // 注意：priority_queue 是最大堆
        }
        return a.duration > b.duration;     // 持续时间短的优先
    };
    
    std::priority_queue<Task, std::vector<Task>, decltype(task_cmp)> 
        task_queue(task_cmp);
    
    task_queue.push(Task("Low Task", 1, 30));
    task_queue.push(Task("High Task", 3, 10));
    task_queue.push(Task("Medium Task", 2, 20));
    task_queue.push(Task("Urgent Task", 3, 5));
    
    // 输出顺序：High Task(3,10) -> Urgent Task(3,5) -> Medium Task(2,20) -> Low Task(1,30)
    while (!task_queue.empty()) {
        Task task = task_queue.top();
        std::cout << task.name << " (priority:" << task.priority 
                  << ", duration:" << task.duration << ")" << std::endl;
        task_queue.pop();
    }
    
    return 0;
}
```

## ⚠️ 重要注意事项

### 1. 比较器的严格弱序要求

比较器必须满足**严格弱序**关系：
- 反对称性：如果 `comp(a, b) == true`，则 `comp(b, a) == false`
- 传递性：如果 `comp(a, b) == true` 且 `comp(b, c) == true`，则 `comp(a, c) == true`
- 不可比较性的传递性

**错误示例**：
```cpp
// 错误：不满足严格弱序
auto bad_cmp = [](int a, int b) {
    return a <= b;  // 应该是 <，而不是 <=
};
```

### 2. 优先队列的比较器逻辑

`priority_queue` 默认是**最大堆**，比较器返回 `true` 表示第一个参数应该排在第二个参数**后面**：

```cpp
// 最大堆：a < b 时返回 true，表示 a 的优先级比 b 低
auto max_heap_cmp = [](int a, int b) { return a < b; };

// 最小堆：a > b 时返回 true，表示 a 的优先级比 b 低  
auto min_heap_cmp = [](int a, int b) { return a > b; };
```

### 3. 使用 std::function 简化声明

如果觉得 `decltype` 繁琐，可以使用 `std::function`：

```cpp
#include <functional>

// 使用 std::function 声明比较器类型
std::function<bool(int, int)> cmp = [](int a, int b) { return a > b; };

// 在模板参数中使用 std::function 类型
std::priority_queue<int, std::vector<int>, std::function<bool(int, int)>> 
    pq(cmp);
```

### 4. C++20 的简化写法

C++20 引入了模板参数推导的改进，可以更简洁：

```cpp
// C++20: 可以直接传递 lambda，不需要 decltype
auto cmp = [](int a, int b) { return a > b; };
std::priority_queue pq(cmp, std::vector<int>{}); // 模板参数自动推导
```

## 📊 总结表格

| 数据结构 | 使用 Lambda 的方式 | 关键点 |
|---------|-------------------|--------|
| **map/set** | 作为模板第三参数：`map<K,V,decltype(lambda)>` | 需要在构造函数中传入 lambda 对象 |
| **multimap/multiset** | 同 map/set | 允许多个相同键，比较器决定顺序 |
| **priority_queue** | 作为模板第三参数：`priority_queue<T,Container,decltype(lambda)>` | 比较器逻辑与排序相反（最大堆） |

使用 lambda 表达式作为这些容器的比较器，可以让代码更加灵活和表达性强，是现代 C++ 编程的重要技巧。