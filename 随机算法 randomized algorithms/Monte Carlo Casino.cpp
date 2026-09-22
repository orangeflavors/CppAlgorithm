#include<bits/stdc++.h>

using namespace std;
using ll = long long;
using ld = long double;
using ull = unsigned long long;
using i128 = __int128;
using PII = pair<int, int>;
using PLL = pair<ll, ll>;
const int N = 1e5 + 5;
const int inf = 0x3f3f3f3f;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const int MOD = 1e9 + 7;
//const int MOD = 998244353;
const double pi = acos(-1.0);
const long double pi_ld = acosl(-1.0L);
#define endl "\n"

// random_device rd;
// mt19937 gen(rd());
// uniform_int_distribution<int> dis(1, 1000000);

// template<typename T>
// T randint(T l, T r) {
//     static mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
//     uniform_int_distribution<T> dis(l, r);
//     return dis(gen);
// }

/** 玩家单局胜率 — 这是蒙特卡洛模拟的核心输入参数。
 *  设为 0.49 表示庄家有 2% 的优势 (0.51 - 0.49)。
 *  修改此值可以观察不同庄家优势对结果的影响。 */
const double PLAYER_WIN_PROB = 0.49;

// ---------- 数据结构 ----------

/** 一次批量模拟的统计结果 */
struct Result {
    vector<int> finalAmounts;  // 每个玩家的最终金额 (原始数据)
    double mean;                    // 均值 — 反映整体盈亏趋势
    double median;                  // 中位数 — 比均值更抗极端值干扰
    double stddev;                  // 标准差 — 反映个体差异程度
    int minAmount;                  // 最倒霉的玩家
    int maxAmount;                  // 最幸运的玩家
    int bankruptCount;              // 破产人数 (金额 ≤ 0)
    int profitCount;                // 盈利人数 (最终 > 初始)
};

// ---------- 蒙特卡洛模拟器 ----------

class CasinoSimulator {
    // Mersenne Twister 19937 — 高质量的伪随机数生成器
    // 用 hardware entropy (random_device) 初始化种子, 保证每次运行结果不同
    mt19937 rng{random_device{}()};

    // Bernoulli 分布: 以 PLAYER_WIN_PROB 的概率返回 true (玩家赢)
    // 这是蒙特卡洛采样的核心 — 每次调用相当于一次下注
    bernoulli_distribution playerWinDist{PLAYER_WIN_PROB};

public:
    /**
     * 模拟单个玩家的完整赌博过程 (单条蒙特卡洛采样路径)
     *
     * @param money       初始金额
     * @param maxRounds   最大下注次数
     * @return            最终金额 (≤0 表示破产)
     *
     * 循环的退出条件体现了"赌徒破产"边界的处理:
     *   money > 0  →  还有钱, 继续赌
     *   money ≤ 0  →  破产, 该玩家提前退出
     */
    int simulateOne(int money, int maxRounds) {
        for (int r = 0; r < maxRounds && money > 0; r++)
            money += playerWinDist(rng) ? 1 : -1;  // 赢则 +1, 输则 -1
        return money;
    }

    /**
     * 批量蒙特卡洛采样 — 对 numPeople 个玩家各自独立模拟
     *
     * 每个玩家的模拟路径是独立的 (独立同分布采样),
     * 因此结果可用于估计整体的统计分布。
     */
    vector<int> simulateBatch(int numPeople, int money, int rounds) {
        vector<int> res(numPeople);
        for (int i = 0; i < numPeople; i++)
            res[i] = simulateOne(money, rounds);    // 独立采样
        return res;
    }

    /**
     * 对采样结果进行统计分析
     *
     * 这是蒙特卡洛方法的"后处理"阶段:
     *   收集所有采样路径的终点, 计算经验分布的数字特征
     *
     * @param amounts   所有玩家的最终金额 (采样结果)
     * @param initMoney 初始金额 (用于判断盈亏)
     * @return          统计结果
     */
    static Result analyze(const vector<int>& amounts, int initMoney) {
        Result r;
        r.finalAmounts = amounts;

        // 排序以便计算中位数和极值
        auto sorted = amounts;
        sort(sorted.begin(), sorted.end());

        // ---- 均值 (样本均值作为总体期望的估计) ----
        double sum = accumulate(amounts.begin(), amounts.end(), 0.0);
        int n = amounts.size();
        r.mean = sum / n;

        // ---- 中位数 (50% 分位点, 稳健估计量) ----
        r.median = (n % 2) ? sorted[n / 2]
                           : (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;

        // ---- 标准差 (衡量个体差异的离散程度) ----
        double sq = 0;
        for (int a : amounts) sq += (a - r.mean) * (a - r.mean);
        r.stddev = sqrt(sq / n);

        // ---- 极值 ----
        r.minAmount = sorted.front();   // 最倒霉
        r.maxAmount = sorted.back();    // 最幸运

        // ---- 盈亏分类 ----
        r.bankruptCount = r.profitCount = 0;
        for (int a : amounts) {
            if (a <= 0) r.bankruptCount++;        // 金额归零或负 = 破产
            if (a > initMoney) r.profitCount++;   // 超过初始本金 = 盈利
        }
        return r;
    }
};


/** 打印一次模拟结果的完整报告 */
void printResult(const string& title, const Result& r,
                 int initMoney, int rounds) {
    int n = r.finalAmounts.size();
    double totalInit  = 1.0 * n * initMoney;                       // 玩家总初始资金
    double totalFinal = accumulate(r.finalAmounts.begin(),     // 玩家总最终资金
                                        r.finalAmounts.end(), 0.0);

    // ---- 表头 ----
    cout << string(60, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(60, '-') << "\n";
    cout << fixed << setprecision(2);

    // ---- 基本参数 ----
    cout << "  人数:" << setw(8) << n
              << "  初始:" << setw(6) << initMoney << "元"
              << "  局数:" << setw(6) << rounds
              << "  庄家胜率:" << setw(5) << (1 - PLAYER_WIN_PROB) * 100 << "%\n";

    // ---- 统计量 ----
    cout << "  均值:" << setw(8) << r.mean << "元"
              << "  中位数:" << setw(8) << r.median << "元"
              << "  标准差:" << setw(8) << r.stddev << "元\n";
    cout << "  最高:" << setw(8) << r.maxAmount << "元"
              << "  最低:" << setw(8) << r.minAmount << "元\n";

    // ---- 盈亏分布 ----
    cout << "  破产:" << setw(5) << r.bankruptCount << "人 ("
              << 100.0 * r.bankruptCount / n << "%)"
              << "  盈利:" << setw(5) << r.profitCount << "人 ("
              << 100.0 * r.profitCount / n << "%)\n";

    // ---- 庄家视角 ----
    cout << "  玩家总计初始:" << totalInit << "元"
              << "  最终:" << totalFinal << "元"
              << "  庄家盈利:" << totalInit - totalFinal << "元\n";

    // ---- ASCII 金额分布直方图 ----
    cout << "\n  金额分布:\n  ";

    // 自适应分桶: 将金额范围分成约 30 个区间
    int bucketSize = max(1, (r.maxAmount - max(0, r.minAmount)) / 30);
    map<int, int> buckets;         // 桶底金额 → 落在该桶的人数
    for (int a : r.finalAmounts)
        buckets[(a / bucketSize) * bucketSize]++;

    // 找出最高频桶作为缩放基准
    int maxCnt = 0;
    for (auto& [_, c] : buckets) maxCnt = max(maxCnt, c);

    // 以 # 号绘制柱状条
    for (auto& [b, c] : buckets) {
        int stars = (int)(30.0 * c / maxCnt);  // 归一化到 30 字符宽度
        cout << "\n  " << setw(4) << b << " |"
                  << string(stars, '#')
                  << string(30 - stars, ' ') << "| " << c;
    }
    cout << "\n  " << string(6, ' ')
              << " (每格" << bucketSize << "元, 初始=" << initMoney << "元)\n\n";
}

void init(){
    
}

void solve(){
    // 创建模拟器实例, 内部初始化了随机数生成器
    CasinoSimulator sim;

    // ============================================================
    // (2) 小规模模拟: 10 人, 每人 10 元, 最多赌 100 次
    //
    // 目的: 小样本观察 — 结果波动大, 可能出现庄家亏损的假象
    //       这正是蒙特卡洛的特点: 样本量不足时估计不稳定
    // ============================================================
    {
        auto res = sim.simulateBatch(10, 10, 100);       // 10 条采样路径
        auto stats = CasinoSimulator::analyze(res, 10);  // 统计后处理
        printResult("(2) 10人 × 10元 × 100局", stats, 10, 100);

        // 逐个列出, 直观感受个体差异
        cout << "  每人最终金额: ";
        for (int i = 0; i < 10; i++)
            cout << res[i] << (i < 9 ? "元, " : "元\n\n");
    }

    // ============================================================
    // (3) 大规模模拟: 1000 人, 每人 100 元, 最多赌 100 次
    //
    // 目的: 大样本观察 — 大数定律作用下, 统计量稳定收敛
    //       庄家盈利≈理论值 2000 元 (1000人 × 100元 × 2%优势)
    //       分布趋近正态 (中心极限定理)
    // ============================================================
    {
        auto res = sim.simulateBatch(1000, 100, 100);
        auto stats = CasinoSimulator::analyze(res, 100);
        printResult("(3) 1000人 × 100元 × 100局", stats, 100, 100);
    }

    // ============================================================
    // (4) 标度分析: 逐步增加人数和局数, 观察统计量的变化趋势
    //
    // 变量控制:
    //   · 人数 ↑ → 统计更稳定 (大数定律)
    //   · 局数 ↑ → 庄家累计优势更明显
    //   · 本金 ↑ → 破产率下降 (更耐久的资金缓冲)
    // ============================================================
    cout << string(60, '=') << "\n";
    cout << "  (4) 逐步增加人数和局数\n";
    cout << string(60, '-') << "\n";
    cout << "  " << left  << setw(36) << "场景"
              << right << setw(6)  << "均值"
              << setw(8)  << "破产%"
              << setw(8)  << "盈利%"
              << setw(10) << "庄家收益%\n";
    cout << string(60, '-') << "\n";

    // 场景参数: {人数, 初始金额, 最大局数, 标签}
    struct S { int n, m, r; string label; };
    for (auto& s : {
        S{10, 10, 100,    "10人 × 10元 × 100局"},       // 基准
        S{100, 10, 100,   "100人 × 10元 × 100局"},      // 人数 ×10
        S{1000, 10, 100,  "1000人 × 10元 × 100局"},     // 人数 ×100
        S{10000, 10, 100, "10000人 × 10元 × 100局"},    // 人数 ×1000
        S{1000, 50, 200,  "1000人 × 50元 × 200局"},     // 更多本金+局数
        S{1000, 100, 500, "1000人 × 100元 × 500局"},    // 高本金多局数
    }) {
        auto res = sim.simulateBatch(s.n, s.m, s.r);
        auto st  = CasinoSimulator::analyze(res, s.m);
        double totalInit  = 1.0 * s.n * s.m;
        double totalFinal = accumulate(res.begin(), res.end(), 0.0);
        cout << "  "  << left  << setw(36) << s.label
                  << right << fixed << setprecision(1)
                  << setw(7)  << st.mean
                  << setw(7)  << st.bankruptCount * 100.0 / s.n << "%"
                  << setw(7)  << st.profitCount   * 100.0 / s.n << "%"
                  << setw(9)  << (totalInit - totalFinal) / totalInit * 100 << "%\n";
    }
}

int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
    int T = 1;
    // cin >> T;
    init();
    while(T--){
        solve();
    }
    return 0;
}