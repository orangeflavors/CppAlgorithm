#include<bits/stdc++.h>
using namespace std;

const int N = 1e5;
char str[N], pattern[N];
int Next[N];
int cnt;

void getNext(char * p, int plen){
    Next[0] = 0; Next[1] = 0;
    for(int i = 1; i < plen; i++){
        int j = Next[i];
        while(j && p[i] != p[j])
            j = Next[j];
        if(p[i] == p[j])
            Next[i + 1] = j + 1;
        else
            Next[i + 1] = 0;
    }
}

void kmp(char * s, char * p){
    cnt = 0;
    int slen = strlen(s), plen = strlen(p);
    getNext(p, plen);
    int j = 0;
    for(int i = 0; i < slen; i++){
        while(j && s[i] != p[j])// 失配
            j = Next[j];// j滑动到Next[j]
        if(s[i] == p[j])
            j++;
        if(j == plen){// j到了p的末尾,找到了1个匹配
            // 该匹配在s中的起点是i + 1 - plen,末尾是i
            // printf("at location = %d, %s\n", i + 1 - plen, &s[i + 1 - plen]);
            cnt++;
        }
    }
}