#include<bits/stdc++.h>

using namespace std;

const int N = 42;
int dp[N];

int fib(int n){
	if(n == 1 || n == 2) return 1;
	return (fib(n - 1) + fib(n - 2));
}

int fibonacci(int n){
	if(n == 1 || n == 2) return 1;
	if(dp[n] != 0) return dp[n];
	dp[n] = fibonacci(n - 1) + fibonacci(n - 2);
	return dp[n];
}

int main(){
	cout << fib(10) << endl;
}
