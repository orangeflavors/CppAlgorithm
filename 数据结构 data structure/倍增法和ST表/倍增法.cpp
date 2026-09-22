#include<bits/stdc++.h>
using namespace std;
using ll = long long;

int log2(ll x){
    int power = 0;
    while((1LL << power) <= (x >> 1)){
        power++;
    }
    return power;
}