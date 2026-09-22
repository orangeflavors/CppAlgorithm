#include<bits/stdc++.h>
using namespace std;

class GaussXOR{
public:
    using ll = long long;
    const int BIT = 63;// 最高位数
    vector<ll> basis, p;// basis线性基行阶梯矩阵,p最简行阶梯矩阵
    int n, cnt;
    bool zero;// 能否异或出0

    GaussXOR(){
        basis.resize(BIT + 1);
        p.resize(BIT + 1);
        n = 0, cnt = 0;
        zero = false;
    }
    ~GaussXOR(){}

    // 在线高斯消元法
    // 竞赛用模板
/*
    行阶梯形保证“空间正确”,最简行阶梯形保证“表示唯一”
    在线线性基平时只需要空间正确,只有在需要排序枚举时才需要表示唯一
*/

    // 向线性基中插入num,如果线性基增加返回true,否则返回false
    bool insert(ll num){
        for(int i = BIT - 1; i >= 0; i--){// 遍历每一位
            if((num >> i) & 1){
                if(basis[i] == 0){// 线性基空缺,插入
                    basis[i] = num;
                    return true;
                }
                num ^= basis[i];// 消去i位的1
            }
        }
        zero = true;// 可以异或出0
        return false;// 插入失败
    }

    // 判断num是否能被异或线性基线性表示
    // 逻辑和insert()一模一样,只是不插入.用基向量消掉num,若最终为0可表示,否则不可
    bool check(ll num){
        for(int i = BIT - 1; i >= 0; i--){// 遍历每一位
            if((num >> i) & 1){
                if(basis[i] == 0){
                    return false;// 不能被消为0
                }
                num ^= basis[i];// 消去i位的1
            }
        }
        return true;// 可以被消为0
    }

    // 返回最大异或和
    // 从大到小对线性基中的所有基向量贪心,如果异或某个基向量可使结果变大,就异或它,否则忽略
    // 因为最大基向量位数最高,其他基向量的异或结果不会超过它,第2大同理
    ll getMax(){
        ll res = 0;
        for(int i = BIT - 1; i >= 0; i--){
            if((res ^ basis[i]) > res) res ^= basis[i];
        }
        return res;
    }

    // 返回最小异或和
    // 若简化阶梯矩阵中有全0的行,说明最小异或和为0
    // 除了0,最小异或和为最小的基向量,即位数最小的,因为这个基向量与其他向量异或必然会增大
    ll getMin(){
        if(zero) return 0;// 可以异或成0
        for(int i = 0; i <= BIT - 1; i++){
            if(basis[i] != 0) return basis[i];
        }
    }

    // 查询第k小异或和前预处理,将矩阵化为最简行阶梯矩阵
    void rebuild(){
        for(int i = BIT - 1; i >= 0; i--){
            for(int j = i - 1; j >= 0; j--){
                if((basis[i] >> j) & 1) basis[i] ^= basis[j];
            }
        }
        for(int i = 0; i <= BIT - 1; i++){
            if(basis[i] != 0) p[cnt++] = basis[i];
        }
    }

    // 查询第k小异或和,如不存在返回-1
    ll kthMinQuery(ll k){
        if(zero) k--;// 当zero == true有线性相关时0为第1小
        if(!k) return 0;// 当zero == false即无线性相关时不将0算作第1小
        // 线性无关时0只由空集产生,一般认为空集不算合法选择.但线性相关时非空集合也能得到0,这时0是合法答案
        ll res = 0;
        if(k >= (1LL << cnt)) return -1;
        for(int i = BIT - 1; i >= 0; i--){
            if((k >> i) & 1) res ^= p[i];
        }
        return res;
    }

    // 查询第k大异或和为kthQuery((1LL << cnt) - k + 1)
    ll kthMaxQuery(ll k){
        ll tot = 1LL << cnt;
        if(k > tot) return -1;// 越界
        return kthMinQuery(tot - k + 1);
    }

    // 合并两个线性基
    void Merge(const GaussXOR &b){
        for(int i = BIT - 1; i >= 0; i--){
            if(b.basis[i] != 0) insert(b.basis[i]);// 将b的基向量逐个插入当前线性基
        }
    }
};

// linear basis
class XORLB{
public:
    using ll = long long;
    const int BIT = 63;
    vector<ll> basis;
    int n;
    bool zero;

    XORLB(){
        basis.resize(BIT + 1);
        n = 0;
        zero = false;
    }
    ~XORLB(){}

    // 向线性基中插入num,如果线性基增加返回true,否则返回false
    bool insert(ll num){
        for(int i = BIT; i >= 0; i--){// 遍历每一位
            if((num >> i) == 1){
                if(basis[i] == 0){// 线性基空缺,插入
                    basis[i] = num;
                    return true;
                }
                num ^= basis[i];// 消去i位的1
            }
        }
        zero = true;// 可以异或出0
        return false;// 插入失败
    }
};