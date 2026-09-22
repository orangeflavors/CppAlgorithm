### ST表(Sparse Table)
适用范围：
如果A区间和B区间可能有重叠的部分，但是并不影响A+B区间的答案,能通过 A区间答案 和 B区间答案 就加工出来，那么对应的区间询问,就是一个 **可重复贡献问题**
区间最大值,区间最小值、区间公约数、区间按位与、区间按位或等,ST表都能高效地解决
但是区间求和就不符合这个要求

ST表的优势和劣势
RMQ问题(Range Maximum/Minimum Query)可以用ST表维护,也可以用线段树等结构维护
ST表的优势:构建过程时间复杂度O(n * logn),单次查询时间复杂度O(1),代码量较小
ST表的劣势:需要空间较大,能维护的信息非常有限,不支持修改操作

[luoguP4155](https://www.luogu.com.cn/problem/P4155)
[luoguP2880](https://www.luogu.com.cn/problem/P2880) RMQ
[luoguP1890](https://www.luogu.com.cn/problem/P1890) 区间gcd
[UVA11235](https://vjudge.net/problem/UVA-11235) RMQ