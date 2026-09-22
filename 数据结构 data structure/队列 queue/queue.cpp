const int N = 10000;

//手写队列
struct HandwrittenQueue{
	int a[N];
	int head = 0;
	int tail = -1;
	int size(){
		return tail - head + 1;
	}
	void push(int data){
		a[++tail] = data;
	}
	void pop(){
		head++;
	}
	int front(){
		return a[head];
	}
};
//该手写队列存在缺陷：如果进入队列的数据太多，使得tail超过N数组将会溢出，导致出错


#include<queue>
#include<iostream>
using namespace std;

int main(){
	//STL queue
	queue<int> que;//定义队列
	for(int i = 0; i < 11; i++){
		que.push(i);//在队列末尾添加一个元素
	}
	que.pop();//移除队列的第一个元素
	int first = que.front();//返回队列的第一个元素的引用
	int last = que.back();//返回队列的最后一个元素的引用
	int que_size = que.size();//返回队列中元素的数量
	while(!que.empty()){//检查队列是否为空
		cout << que.front() << endl;
		que.pop();
	}
	return 0;
}