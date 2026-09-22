#include<bits/stdc++.h>
using namespace std;

// 高精度计时器类 - 支持多种时间单位
class HighPrecisionTimer {
private:
    chrono::time_point<chrono::high_resolution_clock> startTime;
    bool isRunning;
    
public:
    HighPrecisionTimer() : isRunning(false) {}
    
    // 开始计时
    void start() {
        startTime = chrono::high_resolution_clock::now();
        isRunning = true;
    }
    
    // 停止计时并返回结果
    double stop(const string& unit = "ms") {
        if (!isRunning) {
            cerr << "计时器未启动！" << endl;
            return 0.0;
        }
        
        auto endTime = chrono::high_resolution_clock::now();
        auto elapsed = endTime - startTime;
        isRunning = false;
        
        // 根据单位返回时间
        if (unit == "ns") {
            return chrono::duration<double, nano>(elapsed).count();
        } else if (unit == "us" || unit == "μs") {
            return chrono::duration<double, micro>(elapsed).count();
        } else if (unit == "ms") {
            return chrono::duration<double, milli>(elapsed).count();
        } else if (unit == "s") {
            return chrono::duration<double>(elapsed).count();
        } else {
            cerr << "未知时间单位，默认使用毫秒(ms)" << endl;
            return chrono::duration<double, milli>(elapsed).count();
        }
    }
    
    // 直接测量一个函数的执行时间
    template<typename Func, typename... Args>
    static double measure(Func func, Args&&... args, const string& unit = "ms") {
        HighPrecisionTimer timer;
        timer.start();
        func(forward<Args>(args)...);
        return timer.stop(unit);
    }
};

// 格式化时间输出的函数
string formatTime(double time, const string& unit) {
    ostringstream oss;
    oss << fixed << setprecision(6);
    
    if (unit == "ns") {
        if (time < 1000) {
            oss << time << " ns";
        } else if (time < 1e6) {
            oss << time / 1000.0 << " μs";
        } else if (time < 1e9) {
            oss << time / 1e6 << " ms";
        } else {
            oss << time / 1e9 << " s";
        }
    } else if (unit == "us" || unit == "μs") {
        if (time < 1000) {
            oss << time << " μs";
        } else if (time < 1e6) {
            oss << time / 1000.0 << " ms";
        } else {
            oss << time / 1e6 << " s";
        }
    } else if (unit == "ms") {
        if (time < 1000) {
            oss << time << " ms";
        } else {
            oss << time / 1000.0 << " s";
        }
    } else {
        oss << time << " s";
    }
    
    return oss.str();
}