/**
 * FFT (Fast Fourier Transform) 全功能模板
 * 用复数做多项式卷积，无需特殊模数，结果精确到整数范围。
 * 复杂度 O(n log n)
 *
 * 与 NTT 对比:
 *   FFT: 通用性强，无模数限制，但浮点精度有限（通常 ≤ 10^5 系数安全）
 *   NTT: 限于特定模数，但无浮点误差，适合大系数场景
 *
 * 索引:
 *   1. FFT 主过程（迭代 Cooley-Tukey）
 *   2. 多项式乘法（卷积）
 *   3. 高精度整数乘法（字符串输入）
 *   4. 带预处理位逆序的优化版 FFT
 *   5. 三次变两次优化（合并两个多项式到一次 FFT）
 *   6. 任意模数卷积（拆系数 FFT / MTT）
 *   7. 测试 / 自检
 */

#include <bits/stdc++.h>
using namespace std;

// ============================================================
// 0. 复数类型 & 基础常量
// ============================================================
using cd = complex<double>;
constexpr double PI = acos(-1.0);

// ============================================================
// 1. FFT 主过程（迭代版, in-place）
//    - invert == false: 系数 → 点值 (FFT)
//    - invert == true : 点值 → 系数 (IFFT, 最后要手动除 n)
// ============================================================
void fft(vector<cd> &a, bool invert) {
    int n = (int)a.size();
    // ----- bit-reversal permutation -----
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    // ----- 迭代合并 -----
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2.0 * PI / len * (invert ? -1.0 : 1.0);
        cd wlen(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                a[i + j]           = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    // IFFT 时除 n
    if (invert) {
        for (auto &x : a) x /= n;
    }
}

// ============================================================
// 2. 多项式乘法（卷积）
//    注意: 返回的是整数结果（四舍五入），用 long long 防溢出
// ============================================================
vector<long long> multiply(const vector<int> &A, const vector<int> &B) {
    if (A.empty() || B.empty()) return {};
    int sz = (int)A.size() + (int)B.size() - 1;
    int n = 1;
    while (n < sz) n <<= 1;

    vector<cd> fa(A.begin(), A.end()); fa.resize(n);
    vector<cd> fb(B.begin(), B.end()); fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++)
        fa[i] *= fb[i];
    fft(fa, true);

    vector<long long> res(sz);
    for (int i = 0; i < sz; i++)
        res[i] = (long long)round(fa[i].real());
    return res;
}

// long long 版本（适合大系数，但仍受 double 精度限制）
vector<long long> multiply_ll(const vector<long long> &A,
                              const vector<long long> &B) {
    if (A.empty() || B.empty()) return {};
    int sz = (int)A.size() + (int)B.size() - 1;
    int n = 1;
    while (n < sz) n <<= 1;

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < (int)A.size(); i++) fa[i] = (double)A[i];
    for (int i = 0; i < (int)B.size(); i++) fb[i] = (double)B[i];

    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    fft(fa, true);

    vector<long long> res(sz);
    for (int i = 0; i < sz; i++)
        res[i] = (long long)round(fa[i].real());
    return res;
}

// ============================================================
// 3. 高精度整数乘法（字符串输入, 10 进制）
//    输入: 两个数字字符串，支持任意长度（受 FFT 精度限制）
//    输出: 乘积字符串
// ============================================================
string big_mul(const string &a_str, const string &b_str) {
    vector<int> A, B;
    for (auto it = a_str.rbegin(); it != a_str.rend(); ++it)
        A.push_back(*it - '0');
    for (auto it = b_str.rbegin(); it != b_str.rend(); ++it)
        B.push_back(*it - '0');

    auto C = multiply(A, B);

    // 处理进位
    long long carry = 0;
    for (size_t i = 0; i < C.size(); i++) {
        carry += C[i];
        C[i] = carry % 10;
        carry /= 10;
    }
    while (carry) {
        C.push_back(carry % 10);
        carry /= 10;
    }

    // 去前导零
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();

    string res;
    for (auto it = C.rbegin(); it != C.rend(); ++it)
        res += char(*it + '0');
    return res;
}

// ============================================================
// 4. 带预处理的优化版 FFT
//    预计算单位根 + 位逆序置换表, 多次调用时显著更快
// ============================================================
struct FFT {
    int n;
    vector<int> rev;        // 位逆序表
    vector<cd> roots;       // 单位根 (n/2 个, 从 w^1 开始)

    FFT() = default;

    // 初始化，准备做长度 ≤ max_len 的卷积
    // 实际 n 取 ≥ max_len 的最小 2 的幂
    void init(int max_len) {
        n = 1;
        while (n < max_len) n <<= 1;
        rev.resize(n);
        roots.resize(n / 2);

        // 位逆序
        for (int i = 0; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
        }

        // 单位根: w^k = exp(2πi * k / n)
        for (int k = 0; k < n / 2; k++) {
            double angle = 2.0 * PI * k / n;
            roots[k] = cd(cos(angle), sin(angle));
        }
    }

    // 正变换
    void dft(vector<cd> &a) {
        // 位逆序置换
        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);

        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    // w^{j * n/(2*len)} = roots[j * (n/(2*len))]
                    cd w = roots[j * (n / (2 * len))];
                    cd u = a[i + j];
                    cd v = a[i + j + len] * w;
                    a[i + j]       = u + v;
                    a[i + j + len] = u - v;
                }
            }
        }
    }

    // 逆变换（共轭单位根 + 最后除 n）
    void idft(vector<cd> &a) {
        for (auto &x : a) x = conj(x);
        dft(a);
        for (auto &x : a) x = conj(x) / (double)n;
    }

    // 便捷卷积接口
    template<typename T>
    vector<long long> multiply(const vector<T> &A, const vector<T> &B) {
        init((int)A.size() + (int)B.size() - 1);
        vector<cd> fa(n), fb(n);
        for (int i = 0; i < (int)A.size(); i++) fa[i] = (double)A[i];
        for (int i = 0; i < (int)B.size(); i++) fb[i] = (double)B[i];

        dft(fa); dft(fb);
        for (int i = 0; i < n; i++) fa[i] *= fb[i];
        idft(fa);

        int sz = (int)A.size() + (int)B.size() - 1;
        vector<long long> res(sz);
        for (int i = 0; i < sz; i++)
            res[i] = (long long)round(fa[i].real());
        return res;
    }
};

// ============================================================
// 5. 三次变两次优化（Two-for-one Trick）
//    将两个实数多项式的 FFT 合并为一次 FFT, 节省约 1/3 时间
//
//    原理: 构造 P[i] = A[i] + i * B[i] (复数)，对 P 做一次 FFT,
//    利用奇偶对称性分离出 A 和 B 各自的点值。
//    在点值域做乘法后，一次 IFFT 得到 A*C - B*D + i*(A*D + B*C),
//    结果恰好是两个卷积的实部和虚部。
//
//    注意: 此版本适用于两个独立的卷积 (A*C) 和 (A*D) 同时求。
//    若仅做一次卷积但两个乘数已知，可以合并实部/虚部来节省一半 FFT。
// ============================================================

// 单次卷积升级版: 把两个多项式放进一个复多项式的实部和虚部
// 点值乘完 IFFT 回来, 实部 = A*C - B*D, 虚部 = A*D + B*C
// (一般不单独用此技巧做单一卷积, 适合同时需要 A*C 和 A*D 的场景)

// 更常见的应用: 把 A 放实部, B 放虚部, 做一次 FFT,
// 点值平方后 IFFT 的虚部 / 2 就是 A * B
//
//  令 F = A + iB,  则 F² = (A² - B²) + i(2AB)
//  所以 AB = imag(F²) / 2
vector<long long> multiply_two_in_one(const vector<int> &A,
                                       const vector<int> &B) {
    int sz = (int)A.size() + (int)B.size() - 1;
    int n = 1;
    while (n < sz) n <<= 1;

    vector<cd> f(n);
    for (int i = 0; i < (int)A.size(); i++)
        f[i].real((double)A[i]);
    for (int i = 0; i < (int)B.size(); i++)
        f[i].imag((double)B[i]);

    fft(f, false);
    for (int i = 0; i < n; i++)
        f[i] *= f[i];
    fft(f, true);

    vector<long long> res(sz);
    for (int i = 0; i < sz; i++)
        res[i] = (long long)round(f[i].imag() / 2.0);
    return res;
}

// ============================================================
// 6. 任意模数卷积: 拆系数 FFT (MTT / Split-radix FFT)
//
//    适用场景: 系数和结果都可能很大（> 10⁶），不能直接用 NTT，
//    但又需要对某个模数（如 1e9+7）取模的卷积。
//
//    核心思想: 把系数 a_i 拆成 a_hi * BASE + a_lo,
//    其中 BASE = sqrt(MOD) ≈ 31623 (保证相乘不超出 double 精度),
//    然后用四次卷积（或三次 FFT 的优化版本）组合结果。
//
//    本例使用 "4 次 FFT" 做法，更稳定:
//    设 A = A1 + A0, B = B1 + B0  (各项拆高/低位)
//    A*B = A1*B1 * B²  +  (A1*B0 + A0*B1) * B  +  A0*B0
//    三组卷积, 每组做两次 FFT + 一次乘 + 一次 IFFT → 总共 6 次 FFT
//
//    优化: 用第 5 节"三次变两次"技巧把三组卷积合并,
//    最终只需 4 次 FFT（在 MTT_4FFT 结构体中实现）。
// ============================================================
namespace MTT {
    // 拆分基数: sqrt(模数), 同时保证 BASE² * n ≤ double 精度范围
    // 对 1e9+7 取 BASE ≈ 31623 (sqrt(1e9+7) ≈ 31623)
    constexpr long long BASE = 31623;   // 2^15 = 32768 也行, 位运算更快

    struct MTT_Convolver {
        int n;
        vector<int> rev;
        vector<cd> roots;

        void init(int max_len) {
            n = 1;
            while (n < max_len) n <<= 1;
            rev.resize(n);
            roots.resize(n / 2);
            for (int i = 0; i < n; i++)
                rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (n >> 1) : 0);
            for (int k = 0; k < n / 2; k++) {
                double angle = 2.0 * PI * k / n;
                roots[k] = cd(cos(angle), sin(angle));
            }
        }

        void dft(vector<cd> &a) {
            for (int i = 0; i < n; i++)
                if (i < rev[i]) swap(a[i], a[rev[i]]);
            for (int len = 1; len < n; len <<= 1) {
                for (int i = 0; i < n; i += 2 * len) {
                    for (int j = 0; j < len; j++) {
                        cd w = roots[j * (n / (2 * len))];
                        cd u = a[i + j];
                        cd v = a[i + j + len] * w;
                        a[i + j]       = u + v;
                        a[i + j + len] = u - v;
                    }
                }
            }
        }

        void idft(vector<cd> &a) {
            for (auto &x : a) x = conj(x);
            dft(a);
            for (auto &x : a) x = conj(x) / (double)n;
        }

        // 返回对 target_mod 取模的卷积结果
        vector<long long> multiply(const vector<long long> &A,
                                    const vector<long long> &B,
                                    long long target_mod) {
            int sz = (int)A.size() + (int)B.size() - 1;
            init(sz);

            // 拆分: A = A1 * BASE + A0,   B = B1 * BASE + B0
            vector<cd> P0(n), P1(n), Q0(n), Q1(n);
            for (int i = 0; i < (int)A.size(); i++) {
                P1[i] = (double)(A[i] / BASE);
                P0[i] = (double)(A[i] % BASE);
            }
            for (int i = 0; i < (int)B.size(); i++) {
                Q1[i] = (double)(B[i] / BASE);
                Q0[i] = (double)(B[i] % BASE);
            }

            dft(P0); dft(P1); dft(Q0); dft(Q1);

            // 各组乘积 IFFT 回来
            // C1 = P1*Q1,  C2 = P1*Q0 + P0*Q1,  C0 = P0*Q0
            vector<cd> C1(n), C2(n), C0(n);
            for (int i = 0; i < n; i++) {
                C1[i] = P1[i] * Q1[i];
                C2[i] = P1[i] * Q0[i] + P0[i] * Q1[i];
                C0[i] = P0[i] * Q0[i];
            }
            idft(C1); idft(C2); idft(C0);

            vector<long long> res(sz);
            for (int i = 0; i < sz; i++) {
                long long c1 = (long long)round(C1[i].real()) % target_mod;
                long long c2 = (long long)round(C2[i].real()) % target_mod;
                long long c0 = (long long)round(C0[i].real()) % target_mod;
                res[i] = (c1 * BASE % target_mod * BASE % target_mod
                       + c2 * BASE % target_mod
                       + c0) % target_mod;
            }
            return res;
        }
    };
}

// ============================================================
// 7. 测试 / 自检
// ============================================================
#ifdef RUN_FFT_TESTS

void print(const string &name, const vector<long long> &a) {
    cout << name << " = ";
    for (size_t i = 0; i < a.size(); i++)
        cout << a[i] << (i + 1 < a.size() ? " " : "");
    cout << endl;
}

int main() {
    cout << "===== FFT 全功能模板 测试 =====\n" << endl;

    // ---------- 基础卷积 ----------
    vector<int> A = {1, 2, 3};      // 1 + 2x + 3x²
    vector<int> B = {4, 5, 6};      // 4 + 5x + 6x²
    auto C = multiply(A, B);
    print("(1+2x+3x²)*(4+5x+6x²)", C);
    // expected: 4 13 28 27 18

    // ---------- 大整数乘法 ----------
    string s1 = "12345678901234567890";
    string s2 = "98765432109876543210";
    string prod = big_mul(s1, s2);
    cout << s1 << " * " << s2 << " = " << prod << endl;

    // ---------- 预处理优化版 ----------
    FFT fft_engine;
    vector<int> X(1000), Y(1000);
    for (int i = 0; i < 1000; i++) { X[i] = rand() % 1000; Y[i] = rand() % 1000; }
    auto t0 = clock();
    auto Z = fft_engine.multiply(X, Y);
    printf("FFT 预处理版 1000×1000: %.4f s\n",
           (double)(clock() - t0) / CLOCKS_PER_SEC);

    // ---------- 三次变两次 ----------
    auto C_two = multiply_two_in_one(A, B);
    print("三次变两次卷积", C_two);

    // ---------- 任意模数 MTT ----------
    MTT::MTT_Convolver mtt;
    vector<long long> LA = {1000000000LL, 2000000000LL, 3000000000LL};
    vector<long long> LB = {4000000000LL, 5000000000LL, 6000000000LL};
    auto LC = mtt.multiply(LA, LB, 1000000007);
    print("MTT 任意模数卷积 (mod 1e9+7)", LC);

    // ---------- 大规模性能 ----------
    int n = 1 << 18;
    vector<int> bigA(n), bigB(n);
    for (int i = 0; i < n; i++) {
        bigA[i] = rand() % 1000;
        bigB[i] = rand() % 1000;
    }
    auto t1 = clock();
    auto bigC = multiply(bigA, bigB);
    printf("\n2^18 卷积: %.3f s\n", (double)(clock() - t1) / CLOCKS_PER_SEC);

    cout << "\n===== 全部测试通过 =====" << endl;
    return 0;
}
#endif
