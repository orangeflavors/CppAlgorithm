#include <iostream>
#include <chrono>
#include <cmath>

int main() {
    // 获取起始时间点
    auto start = std::chrono::high_resolution_clock::now();

    // 计算密集型操作，使用一个循环进行大量的数学计算
    long long count = 0;
    for (long long i = 0; ; i++) {
        // 进行一些简单的数学计算，比如平方和开方
        double result = std::sqrt(i) * std::sqrt(i);
        count++;
        // 获取当前时间点
        auto current = std::chrono::high_resolution_clock::now();
        // 计算已经过去的时间，单位为毫秒
        long long duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - start).count();
        if (duration >= 300) {
            break;
        }
    }

    // 获取结束时间点
    auto end = std::chrono::high_resolution_clock::now();
    // 计算总共消耗的时间，单位为毫秒
    long long total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "计算密集型操作消耗了: " << total_duration << "ms" << std::endl;
    std::cout << "循环次数: " << count << std::endl;

    return 0;
}