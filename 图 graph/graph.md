#### 其他图论问题
##### “must pass through a and b” 类割点统计题
给定无向连通图和两个特殊点 a,b，统计多少对点 u,v 满足从 u 到 v 的路径必须经过 a 和 b
[CF 1276B](https://codeforces.com/contest/1276/problem/B)
[解题思路](https://www.geeksforgeeks.org/dsa/number-of-pairs-such-that-path-between-pairs-has-the-two-vertices-a-and-b/)
如果有多组特殊点 a,b
需要Tarjan + 点双连通 + block-cut tree