#include<bits/stdc++.h>

using namespace std;
using ll = long long;


//Euclid欧几里得算法（辗转相除法）
//计算两个正整数a,b的最大公约数
//gcd(a, b) = gcd(b, a % b)
int gcd(int a, int b)
{
    if (b==0) return a;
    else return gcd(b, a % b);
}
int x, y;

/*
//exgcd扩展 Euclid 算法
void exgcd(int a, int b, int &x, int &y) {
    if (b == 0) { // 递归终止条件
        x = 1;
        y = 0;
        return;
    }
    int x1, y1; // 用于存储递归调用的结果
    exgcd(b, a % b, x1, y1); // 递归调用
    x = y1; // 更新x的值
    y = x1 - a / b * y1; // 更新y的值
}
*/

//简化写法
ll exgcd(ll a, ll b, ll &x, ll &y){
	if(b == 0){
		x = 1;
		y = 0;
		return a;
	}
	ll d = exgcd(b, a%b, y, x);
	y -= a / b * x;
	return d;
}
