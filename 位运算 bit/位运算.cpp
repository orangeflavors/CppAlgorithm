#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;

// 获取 x 的第 k 位
inline int getBit(ull x, int k){
    return (x >> k) & 1ULL;
}

// 将 x 的第 k 位设置为 1
inline ull setBit(ull x, int k){
    return x | (1ULL << k);
}

// 将 x 的第 k 位设置为 0
inline ull clearBit(ull x, int k){
    return x & ~(1ULL << k);
}

// 将 x 的第 k 位翻转
inline ull toggleBit(ull x, int k){
    return x ^ (1ULL << k);
}

// 判断 x 的第 k 位是否为 1
inline bool hasBit(ull x, int k){
    return (x >> k) & 1ULL;
}

// x 的二进制中 1 的个数
inline int popcount(ull x){
    return __builtin_popcountll(x);
}

// x 的二进制中 0 的个数,按照 64 位计算
inline int countZero(ull x){
    return 64 - __builtin_popcountll(x);
}

// 获取最低位 1 的值
inline ull lowbit(ull x){
    return x & -x;
}

// 获取最低位 1 的下标
// 要求 x != 0
inline int lowpos(ull x){
    return __builtin_ctzll(x);
}

// 删除最低位的 1
inline ull removeLowbit(ull x){
    return x & (x - 1);
}

// 获取最高位 1 的值
// 要求 x != 0
inline ull highbit(ull x){
    return 1ULL << (63 - __builtin_clzll(x));
}

// 获取最高位 1 的下标
// 要求 x != 0
inline int highpos(ull x) {
    return 63 - __builtin_clzll(x);
}

// 返回 log2(x) ,ceil = 0 时为下取整,1 时为上取整
// floor(log2(x)) 即最大的 k，使得 2^k <= x 等价于 <= x 的最大 2 的幂的指数
// ceil(log2(x)) 即最小的 k，使得 2^k >= x 等价于 >= x 的最小 2 的幂的指数
inline int log2(ull x, int ceil = 0){
    if(ceil){// 上取整
        if (x <= 1) return 0;
        return 64 - __builtin_clzll(x - 1);
    }
    else{
        return 63 - __builtin_clzll(x);
    }
}

// 手写 log2(x) 下取整
inline int log2(ull x){
    int power = 0;
    while((1ULL << power) <= (x >> 1)){
        power++;
    }
    return power;
}

// 判断 x 是否为 2 的幂
// 正整数 x 满足 x & (x - 1) == 0
inline bool isPower2(ull x){
    return (x > 0 && (x & (x - 1)) == 0);
}

// 保留最低 k 位
// k = 0 时返回 0
// k >= 64 时返回整个 x
inline ull lowBits(ull x, int k){
    if (k <= 0) return 0;
    if (k >= 64) return x;
    return x & ((1ULL << k) - 1);
}

// 判断 x 的低 k 位是否全部为 1
inline bool lowBits1(ull x, int k){
    if (k <= 0) return true;
    if (k >= 64) return x == ULLONG_MAX;
    return (x & ((1ULL << k) - 1)) == ((1ULL << k) - 1);
}

// 判断 x 是否是 y 的子集
// 将 x、y 看成集合,x 中出现的 1 必须全部出现在 y 中
inline bool isSubset(ull x, ull y){
    return (x & y) == x;
}

// 判断 x 和 y 是否存在公共的 1
inline bool hasCommonBit(ull x, ull y){
    return (x & y) != 0;
}

// x 与 y 的公共 1 的个数
inline int commonBitCount(ull x, ull y){
    return popcount(x & y);
}

// 枚举 x 的所有子集
// 复杂度：O(2^popcount(x))
template<class F>
void enumerateSubsets(ull x, F func){
    for(ull sub = x;; sub = (sub - 1) & x){
        func(sub);
        if (sub == 0) break;
    }
}

// 枚举所有严格子集
template<class F>
void enumerateProperSubsets(ull x, F func){
    for(ull sub = (x - 1) & x; sub; sub = (sub - 1) & x){
        func(sub);
    }
}

// 枚举一个数中的所有 1 的位置
template<class F>
void enumerateBits(ull x, F func){
    while (x) {
        int p = __builtin_ctzll(x);
        func(p);
        x &= x - 1;
    }
}

// Brian Kernighan 算法,手动计算 1 的个数
inline int popcount_manual(ull x){
    int res = 0;
    while(x){
        x &= x - 1;
        ++res;
    }
    return res;
}

// 判断是否只有一个 bit
// 等价于判断是否为 2 的幂
inline bool has_only_one_bit(ull x){
    return x && !(x & (x - 1));
}

// 判断二进制中至少有两个 1
inline bool has_at_least_two_bits(ull x){
    return x && (x & (x - 1));
}

// 64 位循环左移
inline ull rotl64(ull x, int k){
    k &= 63;
    if (k == 0) return x;
    return (x << k) | (x >> (64 - k));
}

// 64 位循环右移
inline ull rotr64(ull x, int k){
    k &= 63;
    if (k == 0) return x;
    return (x >> k) | (x << (64 - k));
}

// 64位二进制分治反转
inline ull reverse_bits(ull x){
    x = ((x & 0x5555555555555555ULL) << 1)
      | ((x >> 1) & 0x5555555555555555ULL);

    x = ((x & 0x3333333333333333ULL) << 2)
      | ((x >> 2) & 0x3333333333333333ULL);

    x = ((x & 0x0F0F0F0F0F0F0F0FULL) << 4)
      | ((x >> 4) & 0x0F0F0F0F0F0F0F0FULL);

    x = ((x & 0x00FF00FF00FF00FFULL) << 8)
      | ((x >> 8) & 0x00FF00FF00FF00FFULL);

    x = ((x & 0x0000FFFF0000FFFFULL) << 16)
      | ((x >> 16) & 0x0000FFFF0000FFFFULL);

    x = (x << 32) | (x >> 32);

    return x;
}

//32位二进制分治反转
unsigned int reverse(unsigned int x) {
    x=((x&0x55555555u)<<1)|((x>>1)&0x55555555u);
    x=((x&0x33333333u)<<2)|((x>>2)&0x33333333u);
    x=((x&0x0F0F0F0Fu)<<4)|((x>>4)&0x0F0F0F0Fu);
    x=((x&0x00FF00FFu)<<8)|((x>>8)&0x00FF00FFu);
    x=(x<<16)|(x>>16);
    return x;
}
/*
另一种写法
unsigned int
reverse(register unsigned int x)
{
	x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
	x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
	x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
	return((x >> 16) | (x << 16));

}
*/

// 位掩码低 k 位全部为 1
inline ull lowMask(int k){
    if (k <= 0) return 0;
    if (k >= 64) return ULLONG_MAX;
    return (1ULL << k) - 1;
}

// 位掩码第 l ~ r 位全部为 1
inline ull rangeMask(int l, int r){
    if(l > r) return 0;
    if(r == 63) return ULLONG_MAX << l;
    return ((1ULL << (r - l + 1)) - 1) << l;
}

// 取 x 的 [l, r] 位
inline ull getRangeBits(ull x, int l, int r){
    return (x >> l) & lowMask(r - l + 1);
}

// 将 x 的 [l, r] 位全部置 1
inline ull setRangeBits(ull x, int l, int r){
    return x | rangeMask(l, r);
}

// 将 x 的 [l, r] 位全部置 0
inline ull clearRangeBits(ull x, int l, int r){
    return x & ~rangeMask(l, r);
}

// 将 x 的 [l, r] 位全部翻转
inline ull toggleRangeBits(ull x, int l, int r){
    return x ^ rangeMask(l, r);
}

int main(){
    unsigned int x;
    cin >> x;

    // 返回x二进制下含1的数量，例如x=15=(1111)时答案为4
    cout << __builtin_popcount(x) << endl;

    // 返回x右数第一个1的位置(1-idx)，1(1) 返回 1，8(1000) 返回 4，26(11010) 返回 2
    cout << __builtin_ffs(x) << endl;

    // 返回x二进制下32位二进制前导0的个数，Count Leading Zeros，要求x != 0
    cout << __builtin_clz(x) << endl;

    // 返回x二进制下后导0的个数，1(1) 返回 0，8(1000) 返回 3
    cout << __builtin_ctz(x) << endl;

    // 以上函数的long long版本在函数名后加上 ll（例如 __builtin_popcountll(x) )，unsigned long long 加上 ull

    // 返回x二进制下的位数，9(1001) 返回 4，26(11010) 返回 5，要求无符号整数
    cout << bit_width(x) << endl;
    return 0;
}
/*
c++20可以使用
#include <bit>

std::popcount(x);
std::countl_zero(x);
std::countr_zero(x);
std::bit_floor(x);
std::bit_ceil(x);
std::has_single_bit(x);
std::rotl(x, k);
std::rotr(x, k);
std::bit_width(x);
*/