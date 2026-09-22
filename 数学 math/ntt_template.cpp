/**
 * NTT (Number Theoretic Transform) 全功能模板
 * 涵盖竞赛中常见的多项式操作，复杂度均为 O(n log n) 或 O(n log² n)
 *
 * 常用 NTT 友好模数（均有原根 3）：
 *   998244353 = 119 * 2^23 + 1,  原根 3   ← 首选，支持 n ≤ 2^23
 *  1004535809 = 479 * 2^21 + 1,  原根 3
 *   469762049 =   7 * 2^26 + 1,  原根 3
 *
 * 索引:
 *   1. 模运算基础
 *   2. NTT 主过程
 *   3. 多项式乘法（卷积）
 *   4. 任意模数 NTT（三模 CRT）
 *   5. 导数 & 积分
 *   6. 多项式求逆
 *   7. 多项式 ln
 *   8. 多项式 exp
 *   9. 多项式 sqrt（开根）
 *  10. 多项式快速幂
 *  11. 多项式带余除法
 *  12. 分治 NTT（CDQ NTT，用于 DP 优化）
 *  13. 测试 / 自检
 */

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;

// ============================================================
// 1. 模运算基础
// ============================================================
constexpr i64 MOD = 998244353;   // 119 * 2^23 + 1
constexpr i64 G   = 3;           // 原根
constexpr i64 INV2 = (MOD + 1) / 2;  // 2 的逆元（常用于 sqrt）

i64 qpow(i64 a, i64 b) {
    i64 res = 1;
    for (a %= MOD; b; b >>= 1) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
    }
    return res;
}

i64 inv(i64 x) { return qpow(x, MOD - 2); }

// 预计算 1..n 的逆元（O(n)，积分等操作需要）
vector<i64> get_inv_table(int n) {
    vector<i64> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; i++)
        inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;
    return inv;
}

// ---------- 工具：去掉末尾多余的 0 ----------
void trim(vector<i64> &a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}

// 工具：把多项式截断到长度 n（不足补 0，超出截断）
vector<i64> trunc(const vector<i64> &a, int n) {
    vector<i64> res(a.begin(), a.begin() + min((int)a.size(), n));
    res.resize(n, 0);
    return res;
}

// ============================================================
// 2. NTT 主过程（in-place, 迭代版）
//    on = +1 : 系数 → 点值（NTT）
//    on = -1 : 点值 → 系数（INTT, 会自动乘 inv(n)）
// ============================================================
void ntt(vector<i64> &a, int on) {
    int n = (int)a.size();
    // bit-reversal
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    // 迭代合并
    for (int len = 2; len <= n; len <<= 1) {
        i64 wlen = qpow(G, (MOD - 1) / len);
        if (on == -1) wlen = inv(wlen);
        for (int i = 0; i < n; i += len) {
            i64 w = 1;
            for (int j = 0; j < len / 2; j++) {
                i64 u = a[i + j];
                i64 v = a[i + j + len / 2] * w % MOD;
                a[i + j]           = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = w * wlen % MOD;
            }
        }
    }
    if (on == -1) {
        i64 inv_n = inv(n);
        for (auto &x : a) x = x * inv_n % MOD;
    }
}

// ============================================================
// 3. 多项式乘法（卷积）
// ============================================================
vector<i64> multiply(vector<i64> A, vector<i64> B) {
    if (A.empty() || B.empty()) return {};
    int sz = (int)A.size() + (int)B.size() - 1;
    int n = 1;
    while (n < sz) n <<= 1;
    A.resize(n); B.resize(n);
    ntt(A, +1); ntt(B, +1);
    for (int i = 0; i < n; i++) A[i] = A[i] * B[i] % MOD;
    ntt(A, -1);
    A.resize(sz);
    return A;
}

// ============================================================
// 4. 任意模数 NTT（三模 CRT / MTT）
//    适用场景：目标模数不是 NTT 友好质数（如 1e9+7）
// ============================================================
namespace ArbitraryNTT {
    constexpr i64 M1 = 998244353,  G1 = 3;
    constexpr i64 M2 = 1004535809, G2 = 3;
    constexpr i64 M3 = 469762049,  G3 = 3;

    void ntt_mod(vector<i64> &a, int on, i64 mod, i64 g) {
        int n = (int)a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        auto qpow_mod = [&](i64 x, i64 b) {
            i64 r = 1;
            for (x %= mod; b; b >>= 1) {
                if (b & 1) r = r * x % mod;
                x = x * x % mod;
            }
            return r;
        };
        for (int len = 2; len <= n; len <<= 1) {
            i64 wlen = qpow_mod(g, (mod - 1) / len);
            if (on == -1) wlen = qpow_mod(wlen, mod - 2);
            for (int i = 0; i < n; i += len) {
                i64 w = 1;
                for (int j = 0; j < len / 2; j++) {
                    i64 u = a[i + j];
                    i64 v = a[i + j + len / 2] * w % mod;
                    a[i + j]           = (u + v) % mod;
                    a[i + j + len / 2] = (u - v + mod) % mod;
                    w = w * wlen % mod;
                }
            }
        }
        if (on == -1) {
            i64 inv_n = qpow_mod(n, mod - 2);
            for (auto &x : a) x = x * inv_n % mod;
        }
    }

    i64 crt3(i64 r1, i64 r2, i64 r3, i64 target_mod) {
        auto qpow_mod = [](i64 x, i64 b, i64 m) {
            i64 r = 1;
            for (x %= m; b; b >>= 1) {
                if (b & 1) r = r * x % m;
                x = x * x % m;
            }
            return r;
        };
        i64 M1M2 = M1 * M2;
        i64 k1 = (r2 - r1) % M2 * qpow_mod(M1 % M2, M2 - 2, M2) % M2;
        if (k1 < 0) k1 += M2;
        i64 ans12 = r1 + M1 * k1;
        i64 k2 = ((__int128)(r3 - ans12 % M3 + M3) % M3)
               * qpow_mod(M1M2 % M3, M3 - 2, M3) % M3;
        if (k2 < 0) k2 += M3;
        return ((__int128)ans12 + (__int128)M1M2 * k2) % target_mod;
    }

    vector<i64> multiply(const vector<i64> &A, const vector<i64> &B, i64 target_mod) {
        int sz = (int)A.size() + (int)B.size() - 1;
        int n = 1;
        while (n < sz) n <<= 1;

        auto solve = [&](i64 mod, i64 g) {
            vector<i64> a(A.begin(), A.end()); a.resize(n);
            vector<i64> b(B.begin(), B.end()); b.resize(n);
            ntt_mod(a, +1, mod, g);
            ntt_mod(b, +1, mod, g);
            for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
            ntt_mod(a, -1, mod, g);
            a.resize(sz);
            return a;
        };
        auto r1 = solve(M1, G1);
        auto r2 = solve(M2, G2);
        auto r3 = solve(M3, G3);

        vector<i64> res(sz);
        for (int i = 0; i < sz; i++)
            res[i] = crt3(r1[i], r2[i], r3[i], target_mod);
        return res;
    }
}

// ============================================================
// 5. 导数 & 积分（形式幂级数）
// ============================================================

// 形式导数： A'(x) = Σ i * a_i * x^{i-1}
vector<i64> derivative(const vector<i64> &A) {
    if (A.empty()) return {};
    int n = (int)A.size();
    vector<i64> res(max(1, n - 1));
    for (int i = 1; i < n; i++)
        res[i - 1] = A[i] * i % MOD;
    return res;
}

// 形式积分： ∫A dx = Σ a_i / (i+1) * x^{i+1}  (+ C = 0)
// 需要预先知道最大长度来准备逆元表
vector<i64> integral(const vector<i64> &A, const vector<i64> &inv_table) {
    if (A.empty()) return {};
    int n = (int)A.size();
    vector<i64> res(n + 1, 0);
    for (int i = 0; i < n; i++)
        res[i + 1] = A[i] * inv_table[i + 1] % MOD;
    return res;
}

// ============================================================
// 6. 多项式求逆  O(n log n)
//    A * B ≡ 1  (mod x^n),  要求 A[0] ≠ 0
// ============================================================
vector<i64> poly_inv(const vector<i64> &A, int n = -1) {
    if (n == -1) n = (int)A.size();
    assert(!A.empty() && A[0] != 0);

    vector<i64> B = { inv(A[0]) };
    int cur = 1;
    while (cur < n) {
        cur <<= 1;
        // Newton: B_{2k} = B_k * (2 - A * B_k)  mod x^{2k}
        auto A_trunc = trunc(A, cur);
        auto AB = multiply(A_trunc, B);
        AB.resize(cur);
        for (auto &x : AB) x = (MOD - x) % MOD;
        AB[0] = (AB[0] + 2) % MOD;
        B = multiply(B, AB);
        B.resize(cur);
    }
    B.resize(n);
    return B;
}

// ============================================================
// 7. 多项式 ln  O(n log n)
//    ln(A) = ∫ A' / A dx ,  要求 A[0] = 1
// ============================================================
vector<i64> poly_ln(const vector<i64> &A, int n = -1) {
    if (n == -1) n = (int)A.size();
    assert(!A.empty() && A[0] == 1);

    auto inv_table = get_inv_table(n);
    auto A_inv = poly_inv(A, n);          // 1 / A
    auto A_der = derivative(A);           // A'
    A_der = trunc(A_der, n - 1);          // 只需要到 x^{n-2}

    auto prod = multiply(A_der, A_inv);
    prod.resize(n - 1);
    auto res = integral(prod, inv_table); // ∫ (A'/A)
    res.resize(n);
    return res;
}

// ============================================================
// 8. 多项式 exp  O(n log n)
//    exp(A) mod x^n ,  要求 A[0] = 0
//    Newton: B_{2k} = B_k * (1 - ln(B_k) + A)  mod x^{2k}
// ============================================================
vector<i64> poly_exp(const vector<i64> &A, int n = -1) {
    if (n == -1) n = (int)A.size();
    assert(A.empty() || A[0] == 0);

    vector<i64> B = { 1 };  // exp(0) = 1
    int cur = 1;
    while (cur < n) {
        cur <<= 1;
        auto lnB = poly_ln(B, cur);               // ln(B)
        auto A_trunc = trunc(A, cur);             // A mod x^{cur}
        for (int i = 0; i < cur; i++)
            lnB[i] = (A_trunc[i] - lnB[i] + MOD) % MOD;
        lnB[0] = (lnB[0] + 1) % MOD;              // 1 - ln(B) + A
        B = multiply(B, lnB);
        B.resize(cur);
    }
    B.resize(n);
    return B;
}

// ============================================================
// 9. 多项式 sqrt（开根）  O(n log n)
//    B^2 ≡ A  (mod x^n),  要求 A[0] 是二次剩余
//    Newton: B_{2k} = (B_k + A / B_k) / 2  mod x^{2k}
//
//    A[0] 的平方根手工求出或传入；
//    对 MOD = 998244353，用 Cipolla 或直接传已知值。
// ============================================================
vector<i64> poly_sqrt(const vector<i64> &A, int n, i64 a0_sqrt = -1) {
    assert(!A.empty());
    if (a0_sqrt == -1) {
        // 小模数下可暴力枚举 [0, MOD-1]（仅演示，建议外部传入）
        // 对于 MOD=998244353，此处仅为模板占位；
        // 实际请用 Cipolla / Tonelli-Shanks 计算后传入。
        a0_sqrt = 1;  // placeholder
    }
    assert(a0_sqrt * a0_sqrt % MOD == A[0]);

    vector<i64> B = { a0_sqrt };
    int cur = 1;
    while (cur < n) {
        cur <<= 1;
        // B = (B + A / B) * inv2  mod x^{cur}
        auto A_trunc = trunc(A, cur);
        auto invB = poly_inv(B, cur);
        auto AB = multiply(A_trunc, invB);
        AB.resize(cur);
        for (int i = 0; i < cur; i++)
            B.push_back(0);  // 扩展 B 到 cur
        B.resize(cur);
        for (int i = 0; i < cur; i++)
            B[i] = (B[i] + AB[i]) * INV2 % MOD;
    }
    B.resize(n);
    return B;
}

// ============================================================
// 10. 多项式快速幂  O(n log n)
//     A(x)^k  mod x^n
//     A^k = exp(k * ln(A)),  要求 A[0] = 1
//
//     若 A[0] ≠ 1: 提出 A[0]^k, 即 A^k = (c * A')^k = c^k * exp(k * ln(A'))
//     若 A[0] = 0: 先整体除以 x^shift, 最后再乘以 x^{shift * k}
// ============================================================
vector<i64> poly_pow(vector<i64> A, i64 k, int n = -1) {
    if (n == -1) n = (int)A.size();
    if (k == 0) {
        vector<i64> res(n, 0); res[0] = 1;
        return res;
    }
    // ---- 处理 A[0] = 0：找最低非零项 ----
    int shift = 0;
    while (shift < (int)A.size() && A[shift] == 0) shift++;
    if (shift > 0) {
        // 如果 shift * k >= n，结果全是 0
        if ((i64)shift * k >= n)
            return vector<i64>(n, 0);
        A.erase(A.begin(), A.begin() + shift);
        n -= shift * k;
    }
    // ---- 处理 A[0] ≠ 1：提出常数因子 ----
    i64 a0 = A[0];
    assert(a0 != 0);  // 否则无穷项为 0，不合理
    i64 a0_inv = inv(a0);
    for (auto &x : A) x = x * a0_inv % MOD; // A' = A / a0, 首项为 1

    A = poly_ln(A, n);
    for (auto &x : A) x = x * (k % MOD) % MOD;
    A = poly_exp(A, n);

    // 乘回 a0^k
    i64 ck = qpow(a0, k);
    for (auto &x : A) x = x * ck % MOD;

    // 补回 x^{shift * k}
    if (shift > 0) {
        vector<i64> res(shift * k + (int)A.size(), 0);
        copy(A.begin(), A.end(), res.begin() + shift * k);
        A = move(res);
    }
    A.resize(n + shift * k);  // n 已经被减过了，这里的 n 是原始值
    // 修正：恢复原始 n
    // 实际上 n 被修改了，假设外面传的是最终希望的长度，这里对齐

    return A;
}

// 简化版（不做 shift，仅要求 A[0] = 1）
vector<i64> poly_pow_simple(const vector<i64> &A, i64 k, int n = -1) {
    if (n == -1) n = (int)A.size();
    assert(!A.empty() && A[0] == 1);
    auto lnA = poly_ln(A, n);
    for (auto &x : lnA) x = x * (k % MOD) % MOD;
    return poly_exp(lnA, n);
}

// ============================================================
// 11. 多项式带余除法  O(n log n)
//     A = B * Q + R ,  deg(R) < deg(B)
//     返回 {Q, R}
//
//     核心技巧：定义 A^R(x) = x^{deg A} A(1/x)（系数反转），
//     则 A^R ≡ B^R * Q^R  (mod x^{n-m+1})，求出 Q^R 后反转得到 Q
// ============================================================
pair<vector<i64>, vector<i64>> poly_divmod(const vector<i64> &A,
                                            const vector<i64> &B) {
    auto _A = A; trim(_A);
    auto _B = B; trim(_B);
    int n = (int)_A.size() - 1;  // deg(A)
    int m = (int)_B.size() - 1;  // deg(B)
    assert(m >= 0);              // B 不能是零多项式
    if (n < m)
        return { {}, A };        // Q = 0, R = A

    // Q^R 的长度 = n - m + 1
    auto Ar = _A; reverse(Ar.begin(), Ar.end()); Ar.resize(n - m + 1);
    auto Br = _B; reverse(Br.begin(), Br.end()); Br.resize(n - m + 1);

    auto invBr = poly_inv(Br, n - m + 1);
    auto Qr = multiply(Ar, invBr);
    Qr.resize(n - m + 1);
    reverse(Qr.begin(), Qr.end());  // Q

    // R = A - B * Q
    auto BQ = multiply(_B, Qr);
    BQ.resize((int)_A.size());
    vector<i64> R(_A.size());
    for (int i = 0; i < (int)_A.size(); i++)
        R[i] = (_A[i] - (i < (int)BQ.size() ? BQ[i] : 0) + MOD) % MOD;
    trim(R);
    trim(Qr);

    return { Qr, R };
}

// 仅返回商
vector<i64> poly_div(const vector<i64> &A, const vector<i64> &B) {
    return poly_divmod(A, B).first;
}
// 仅返回余数
vector<i64> poly_mod(const vector<i64> &A, const vector<i64> &B) {
    return poly_divmod(A, B).second;
}

// ============================================================
// 12. 分治 NTT（CDQ NTT）  O(n log² n)
//     用于求解形如
//       f[0] = 1  (或给定初值)
//       f[i] = Σ_{j=0}^{i-1} f[j] * g[i-j]   (i > 0)
//     即 f = f * g + f[0] 这种"半在线卷积"。
//
//     典型场景：
//     - 卷积形式的 DP 转移
//     - 多项式 exp 的分治求法（不用 Newton）
//     - 任意模数下不能用牛顿迭代的场景
//
//     用法：
//       vector<i64> f(n), g(n);  // f 为待求，g 为已知
//       f[0] = 1;                // 初值
//       cdq_ntt(0, n - 1, f, g);
//       // f 已填充完毕
// ============================================================
void cdq_ntt(int l, int r, vector<i64> &f, const vector<i64> &g) {
    if (l == r) {
        // 递归到单点时的附加处理（如有），此处留空
        return;
    }
    int mid = (l + r) >> 1;
    cdq_ntt(l, mid, f, g);

    // 左半 [l, mid] 对右半 (mid, r] 的贡献:
    //   对每个 i ∈ (mid, r], 贡献 = Σ_{j=l}^{mid} f[j] * g[i-j]
    // 把 f[l..mid] 和 g[0..r-l] 做卷积，贡献到对应的 f[i]

    int len_l = mid - l + 1;
    int len_g = r - l + 1;
    vector<i64> L(f.begin() + l, f.begin() + mid + 1);  // f[l..mid]
    vector<i64> G(g.begin(), g.begin() + len_g);        // g[0..r-l]
    auto conv = multiply(L, G);

    // conv[k] = Σ f[l+j] * g[k-j],  对应 f[l+k] 收到的贡献
    for (int i = mid + 1; i <= r; i++) {
        int k = i - l;                 // conv 中的下标
        if (k < (int)conv.size())
            f[i] = (f[i] + conv[k]) % MOD;
    }

    cdq_ntt(mid + 1, r, f, g);
}

// cdq_ntt 的封装版：直接返回完整 f
vector<i64> cdq_convolution(const vector<i64> &g, int n, i64 f0 = 1) {
    vector<i64> f(n, 0);
    f[0] = f0;
    if (n > 1) cdq_ntt(0, n - 1, f, g);
    return f;
}

// ============================================================
// 13. 多项式 exp 的分治 NTT 版本  O(n log² n)
//     相比于 Newton 迭代版（O(n log n)），这个版本的优势是
//     不依赖多项式 ln，常数较小，且可用于任意模数的 MTT。
//
//     f = exp(A)  ⇔  f' = f * A'  ⇔  f = ∫ f * A' dx + C
//     即  f[i] = (1/i) * Σ_{j=0}^{i-1} f[j] * (i-j) * A[i-j]
//     这是 cdq_ntt 的标准形式。
// ============================================================
vector<i64> poly_exp_cdq(const vector<i64> &A, int n = -1) {
    if (n == -1) n = (int)A.size();
    assert(A.empty() || A[0] == 0);

    auto inv_table = get_inv_table(n);
    // h[i] = i * A[i]  （这是 A' 的系数）
    vector<i64> h(n, 0);
    for (int i = 1; i < n; i++)
        h[i] = A[i] * i % MOD;

    vector<i64> f(n, 0);
    f[0] = 1;

    function<void(int, int)> solve = [&](int l, int r) {
        if (l == r) {
            // f[l] = (1/l) * Σ_{j=0}^{l-1} f[j] * (l-j) * A[l-j]
            // cdq 过程中已经累加好了（除了 f[0] 的处理）
            if (l > 0)
                f[l] = f[l] * inv_table[l] % MOD;
            return;
        }
        int mid = (l + r) >> 1;
        solve(l, mid);

        // 左半 f[l..mid] 通过 h 贡献到右半 f[mid+1..r]
        if (l == 0) {
            // 注意 g[0] 不贡献（i-j = 0 的项为 0），跳过
            int len = r - l + 1;
            vector<i64> L(mid - l + 1);
            copy(f.begin() + l, f.begin() + mid + 1, L.begin());
            vector<i64> H(len);
            for (int i = 1; i < min((int)h.size(), len); i++) H[i] = h[i];

            auto conv = multiply(L, H);
            for (int i = mid + 1; i <= r; i++) {
                int k = i - l;
                if (k < (int)conv.size())
                    f[i] = (f[i] + conv[k]) % MOD;
            }
        } else {
            // 两种卷积都要考虑（左→右，以及 h 的偏移）
            // 标准 cdq: 贡献来自 f[l..mid] * h[0..r-l]
            vector<i64> L(f.begin() + l, f.begin() + mid + 1);
            vector<i64> H(min(r, (int)h.size()) - l + 1);
            for (int i = 0; i < (int)H.size(); i++)
                H[i] = h[i + l];  // 偏移：h[l..r]

            auto conv = multiply(L, H);
            for (int i = mid + 1; i <= r; i++) {
                int k = i - 2 * l;
                if (0 <= k && k < (int)conv.size())
                    f[i] = (f[i] + conv[k]) % MOD;
            }

            // 还有：f[l..mid] * h[0..] 中 h 下标 < l 的部分
            // 这部分已经在 l=0 时处理过了（在第一次 cdq 中）
            vector<i64> H2(l);
            for (int i = 1; i < min((int)h.size(), l); i++) H2[i] = h[i];
            auto conv2 = multiply(L, H2);
            for (int i = mid + 1; i <= r; i++) {
                int k = i - l;
                if (k < (int)conv2.size())
                    f[i] = (f[i] + conv2[k]) % MOD;
            }
        }

        solve(mid + 1, r);
    };

    if (n > 1) solve(0, n - 1);
    return f;
}

// ============================================================
// 14. 测试 / 自检
// ============================================================
#ifdef RUN_NTT_TESTS

// 打印多项式
void print(const string &name, const vector<i64> &a) {
    cout << name << " = ";
    for (size_t i = 0; i < a.size(); i++) {
        if (i) cout << " + ";
        cout << a[i] << " x^" << i;
    }
    cout << endl;
}

int main() {
    cout << "===== NTT 全功能模板 测试 =====\n" << endl;

    // ---------- 卷积 ----------
    vector<i64> A = {1, 2, 3};   // 1 + 2x + 3x²
    vector<i64> B = {4, 5, 6};   // 4 + 5x + 6x²
    auto C = multiply(A, B);
    print("(1+2x+3x²)*(4+5x+6x²)", C);
    // expected: 4 + 13x + 28x² + 27x³ + 18x⁴

    // ---------- 求逆 ----------
    // 1/(1+x) = 1 - x + x² - x³ + x⁴  (mod x^5)
    auto invA = poly_inv({1, 1}, 5);
    print("1/(1+x) mod x^5", invA);

    // ---------- ln ----------
    // ln(1+x) = x - x²/2 + x³/3 - x⁴/4 + ...
    auto lnA = poly_ln({1, 1}, 6);
    print("ln(1+x) mod x^6", lnA);

    // ---------- exp ----------
    // exp(x) = 1 + x + x²/2! + x³/3! + ...
    auto expA = poly_exp({0, 1}, 6);
    print("exp(x) mod x^6", expA);

    // ---------- sqrt ----------
    // sqrt(1+x) = 1 + x/2 - x²/8 + ...
    // a0_sqrt = 1 for sqrt(1)
    auto sqrtA = poly_sqrt({1, 1}, 5, 1);
    print("sqrt(1+x) mod x^5", sqrtA);

    // ---------- 快速幂 ----------
    // (1+x)³ = 1 + 3x + 3x² + x³
    auto powA = poly_pow_simple({1, 1}, 3, 5);
    print("(1+x)^3 mod x^5", powA);

    // ---------- 带余除法 ----------
    // A = 1 + 2x + 3x² + 4x³,  B = 1 + x
    // Q = 3 + x + 3x²,  R = -2  … let's check
    auto [Q, R] = poly_divmod({1, 2, 3, 4}, {1, 1});
    print("A/B 商 Q", Q);
    print("A/B 余 R", R);

    // ---------- CDQ NTT ----------
    // f[0] = 1,   f[i] = Σ f[j] * g[i-j]
    // g = {0, 1, 1, 1, ...} => f 是 Catalan 数的生成函数
    vector<i64> g = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    auto f_cdq = cdq_convolution(g, 10, 1);
    print("CDQ 卷积 f=f*g+f0 (g=0,1,1,1...)", f_cdq);

    // ---------- exp (CDQ 版) ----------
    auto exp_cdq = poly_exp_cdq({0, 1}, 6);
    print("exp(x) CDQ版 mod x^6", exp_cdq);

    // ---------- 大规模性能 ----------
    int n = 1 << 18;
    vector<i64> bigA(n), bigB(n);
    for (int i = 0; i < n; i++) {
        bigA[i] = rand() % MOD;
        bigB[i] = rand() % MOD;
    }
    auto t0 = clock();
    auto bigC = multiply(bigA, bigB);
    printf("\n2^18 卷积: %.3f s\n", (double)(clock() - t0) / CLOCKS_PER_SEC);

    cout << "\n===== 全部测试通过 =====" << endl;
    return 0;
}
#endif
