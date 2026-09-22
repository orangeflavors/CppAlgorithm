### std::string
现代 $C++$ 的 std::string 已经可以高效地修改字符串。
例如：
```cpp
std::string s;

for (int i = 0; i < 100000; i++) {
    s += "abc";
}
```
这里不会每次都重新申请内存。
因为 std::string 和 std::vector 一样，都维护 $capacity$（容量）、$size$（当前长度）
例如 $size = 15、capacity = 32$
继续追加字符时 $size = 16、capacity = 32$
不会重新分配内存。
只有 $size > capacity$ 时，才会重新申请一块更大的空间（通常扩大为 1.5~2 倍）。
因此追加很多次：
```cpp
+=
append()
push_back()
```
都是均摊 $O(1)$ 的。

如果知道最终长度，可以提前申请。
例如：
```cpp
std::string s;
s.reserve(1000000);

for (int i = 0; i < 1000000; i++)
    s += 'a';
```
$1$ 次内存分配,$0$ 次扩容

使用 [] 修改字符串不会复制整个字符串，因为 std::string 是可变字符串。
但插入删除时
例如：
```cpp
s.insert(pos, "abc");
s.erase(pos, len);
```
后面的字符都需要整体移动，因此时间复杂度是 $O(n)$ 任何连续内存的字符串都无法避免这一点。

### 数字 → 字符串
#### std::to_string（最常用，C++11）
支持
```cpp
int
long
long long
float
double
long double
```
注意 to_string(double) 默认会保留 $6$ 位小数。

#### ostringstream（可控制格式）
```cpp
#include <sstream>

double pi = 3.1415926;

ostringstream out;
out.precision(2);
out << fixed << pi;

string s = out.str();
```
结果为 $3.14$，适合需要：保留几位小数、十六进制、科学计数法等格式控制。

#### std::format（C++20）
类似 Python：
```cpp
#include <format>

string s = std::format("{:.2f}", 3.14159);
```
结果为 $3.14$，目前并不是所有编译器都完整支持。

### 字符串 → 数字
#### stoi（最常用）
```cpp
string s = "123";

int x = stoi(s);

cout << x;
```
不同版本：
| 函数  | 返回类型 |
| ----  | ---- |
| stoi  | int |
| stol  | long |
| stoll | long long |
| stof  | float |
| stod  | double |
| stold | long double |

#### 支持不同进制
例如：
```cpp
string s = "1010";

int x = stoi(s, nullptr, 2);

cout << x;
```
输出 $10$
stoi 第三个参数表示进制

#### 获取未转换部分
```cpp
string s = "123abc";

size_t pos;

int x = stoi(s, &pos);

cout << x << endl;
cout << pos << endl;
```
输出
```
123
3
```
说明：
```
123abc
   ^
   pos
```
前三个字符成功转换，剩余 $abc$ 没有参与转换。

如果转换失败：
```cpp
stoi("abc");
```
会抛出
```cpp
std::invalid_argument
```
如果数字过大：
```cpp
stoi("999999999999999999");
```
会抛出
```cpp
std::out_of_range
```

#### from_chars（高性能，C++17）
如果追求性能（如竞赛、高性能服务器），推荐使用 \<charconv> 中的 std::from_chars。
```cpp
#include <charconv>

std::string s = "12345";
int x;

auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), x);

if (ec == std::errc()) {
    // 转换成功
}
```