#include<vector>
#include<cassert>
using namespace std;

/*
分组树状数组
1-indexed
有很多个组，每个组内部都要做“单点取最大值 + 前缀最大值查询”，但每个组的坐标数量不同，希望总空间只有 O(n)
单点取最大值，查询前缀最大值
*/
template<class T>
class GroupBIT{
public:
    int n;// 节点数
    T default_val;
    vector<int> start;// start[g] 分组 g 在树状数组中的起始位置
    vector<int> len;// len[g] 分组 g 的树状数组长度
    vector<T> tree;// 所有分组的树状数组拼接到一起
    inline int lowbit(int x){return x & -x;}

    BIT() = default;
    BIT(int n_){
        init(n_);
    }

    void init(int n_, T val = numeric_limits<T>::lowest()){
        n = n_;
        default_val = val;
        tree.assign(n + 1, val);
    }

    void build()

    // 单点取最大值
    void modify(int x, T v){
        assert(1 <= x && x <= n);
        for(; x <= n; x += lowbit(x)){
            tree[x] = max(tree[x], v);
        }
    }

    // 查询前缀最大值 [1, r]
    int queryMax(int r){
        assert(0 <= r && r <= n);

        T ans = default_val;

        for(; r > 0; r -= lowbit(r)){
            ans = max(ans, tree[r]);
        }

        return ans;
    }
};


const int NEG = -1000000000;

/*
    对固定 group：
        每个 group 内有一组 coord 坐标
        Fenwick 维护这些坐标上的最大值

    支持：
        update(group, pos, val)
            单点取 max

        query(group, pos)
            查询该组前缀 [1, pos] 的最大值

    group 的实际值范围：
        [-n, n]
    使用 offset=n 映射到：
        [0, 2n]
*/
class GroupFenwickMax {
public:
    int n;
    int offset;

    // start[g]：
    // group g 在平面化 bit 数组中的起始位置
    vector<int> start;

    // len[g]：
    // group g 的 Fenwick 长度
    vector<int> len;

    // 所有组的 Fenwick 树拼接到一起
    vector<int> bit;

    GroupFenwickMax() {}

    /*
        group[i]：第 i 个点所属的组
        coord[i]：第 i 个点在组中的另一个坐标
        pos[i]：输出，第 i 个点对应的 Fenwick 位置

        这里按照 (group, coord) 排序，
        同一个 group 中相同 coord 的点共用一个位置。
    */
    void build(
        const vector<int>& group,
        const vector<int>& coord,
        vector<int>& pos,
        int offset_
    ) {
        offset = offset_;
        n = (int)group.size() - 1;

        int G = 2 * offset + 1;

        start.assign(G, -1);
        len.assign(G, 0);
        pos.assign(n + 1, 0);

        vector<int> ord(n + 1);
        iota(ord.begin(), ord.end(), 0);

        sort(ord.begin(), ord.end(),
            [&](int a, int b) {
                if (group[a] != group[b])
                    return group[a] < group[b];
                if (coord[a] != coord[b])
                    return coord[a] < coord[b];
                return a < b;
            }
        );

        /*
            每个组只存实际出现的 coord。
        */
        int total = 0;

        for (int l = 0; l <= n; ) {
            int r = l + 1;

            while (r <= n &&
                   group[ord[r]] == group[ord[l]]) {
                ++r;
            }

            int gid = group[ord[l]] + offset;

            start[gid] = total;

            int k = 0;
            int lastCoord = 0;
            bool first = true;

            for (int p = l; p < r; ++p) {
                int id = ord[p];

                if (first || coord[id] != lastCoord) {
                    ++k;
                    lastCoord = coord[id];
                    first = false;
                }

                pos[id] = k;
            }

            len[gid] = k;

            /*
                Fenwick 下标从 1 开始，
                所以这里给每个组多留一个位置。
            */
            total += k + 1;

            l = r;
        }

        bit.assign(total, NEG);
    }

    // 单点取 max
    inline void update(
        int groupValue,
        int p,
        int val
    ) {
        int gid = groupValue + offset;

        int base = start[gid];
        int k = len[gid];

        for (int x = p; x <= k; x += x & -x) {
            bit[base + x] =
                max(bit[base + x], val);
        }
    }

    // 查询 [1,p] 最大值
    inline int query(
        int groupValue,
        int p
    ) const {
        int gid = groupValue + offset;

        int base = start[gid];

        int res = NEG;

        for (int x = p; x > 0; x -= x & -x) {
            res = max(res, bit[base + x]);
        }

        return res;
    }
};