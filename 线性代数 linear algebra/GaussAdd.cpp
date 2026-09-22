#include<bits/stdc++.h>
using namespace std;

const double eps = 1e-7;

// 高斯消元解决加法方程组
// 时间复杂度O(n**3)
void GaussAdd(vector<vector<int>>& a){
    int n = a.size() - 1;// 1-base
    for(int i = 1; i <= n; i++){
        int max = i;
        for(int j = 1; j <= n; j++){
            if(j < i && abs(a[j][j]) >= eps)
                continue;// 跳过已经确定主元的行
            if(abs(a[j][i]) > abs(a[max][i]))
                max = j;// 找到第i列系数最大的行号
        }
        swap(a[i], a[max]);// 将max行换到i行位置上
        if(abs(a[i][i]) >= eps){
            double temp = a[i][i];
            for(int j = i; j <= n + 1; j++){
                a[i][j] /= temp;// 将主元系数变为1
            }
            for(int j = 1; j <= n; j++){// 将其余行的i列变为0
                if(i != j){
                    double rate = a[j][i] / a[i][i];
                    for(int k = i; k <= n + 1; k++){
                        a[j][k] -= a[i][k] * rate;
                    }
                }
            }
        }
    }
}


const double EPS = 1e-9;

// 方程结构：系数 a[0..n-1] 和常数项 b
struct Equation {
    std::vector<double> a;
    double b;
};

class EquationSelector {
public:
    // n 为变量个数
    EquationSelector(int n) : n(n) {}

    // 尝试加入一个方程，返回是否被接受
    bool addEquation(const Equation& eq) {
        int m = basis.size();   // 当前基的行数
        std::vector<double> row = eq.a;
        double b = eq.b;

        // 用已有的基行消去 row
        for (int i = 0; i < m; ++i) {
            int pivotCol = pivotCols[i];
            if (std::fabs(row[pivotCol]) > EPS) {
                double factor = row[pivotCol];
                for (int j = 0; j < n; ++j) {
                    row[j] -= factor * basis[i][j];
                }
                b -= factor * basisB[i];
            }
        }

        // 寻找第一个非零系数作为主元列
        int pivot = -1;
        for (int j = 0; j < n; ++j) {
            if (std::fabs(row[j]) > EPS) {
                pivot = j;
                break;
            }
        }

        if (pivot == -1) {
            // 系数全为零，检查常数项
            if (std::fabs(b) > EPS) {
                // 矛盾方程：0 = 非零
                return false;
            } else {
                // 冗余方程：0 = 0
                return false;
            }
        }

        // 线性无关，归一化新行
        double inv = 1.0 / row[pivot];
        for (int j = 0; j < n; ++j) {
            row[j] *= inv;
        }
        b *= inv;

        // 用新行消去基中所有其他行的 pivot 列
        for (int i = 0; i < m; ++i) {
            double factor = basis[i][pivot];
            if (std::fabs(factor) > EPS) {
                for (int j = 0; j < n; ++j) {
                    basis[i][j] -= factor * row[j];
                }
                basisB[i] -= factor * b;
            }
        }

        // 将新行加入基
        basis.push_back(row);
        basisB.push_back(b);
        pivotCols.push_back(pivot);
        acceptedIndices.push_back(currentIndex);
        return true;
    }

    // 设置当前方程序号（用于记录接受的方程索引，从0开始）
    void setCurrentIndex(int idx) {
        currentIndex = idx;
    }

    // 获取已接受方程的原始索引
    const std::vector<int>& getAcceptedIndices() const {
        return acceptedIndices;
    }

    // 打印当前基（简化阶梯形）
    void printBasis() const {
        for (size_t i = 0; i < basis.size(); ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << basis[i][j] << " ";
            }
            std::cout << "| " << basisB[i] << std::endl;
        }
    }

    int getRank() const { return basis.size(); }

private:
    int n;
    std::vector<std::vector<double>> basis;   // 系数矩阵的行简化阶梯形
    std::vector<double> basisB;               // 对应的常数项
    std::vector<int> pivotCols;               // 每行的主元列
    std::vector<int> acceptedIndices;         // 接受的方程原始序号
    int currentIndex = 0;
};

// 示例用法
int main() {
    int n = 3;  // 三元一次方程
    EquationSelector selector(n);

    // 假设有5个方程依次输入
    std::vector<Equation> equations = {
        {{1, 1, 1}, 6},   // x + y + z = 6
        {{1, -1, 1}, 2},  // x - y + z = 2
        {{2, 0, 2}, 8},   // 2x + 0y + 2z = 8 （与上两行线性相关？ 实际 第1+第2 = 2x+0y+2z=8，冗余）
        {{1, 1, 1}, 7},   // x+y+z=7 （矛盾）
        {{0, 1, 0}, 1}    // y = 1 （新独立方程）
    };

    for (size_t i = 0; i < equations.size(); ++i) {
        selector.setCurrentIndex(i);
        bool ok = selector.addEquation(equations[i]);
        std::cout << "方程 " << i << " : " << (ok ? "接受" : "拒绝") << std::endl;
    }

    std::cout << "\n最终基（行简化阶梯形）：\n";
    selector.printBasis();
    std::cout << "秩 = " << selector.getRank() << std::endl;

    std::cout << "被接受的方程索引：";
    for (int idx : selector.getAcceptedIndices()) {
        std::cout << idx << " ";
    }
    std::cout << std::endl;

    return 0;
}