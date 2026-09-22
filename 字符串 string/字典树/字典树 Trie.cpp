#include<bits/stdc++.h>
using namespace std;

// 静态数组
const int N = 1e6 + 5;
int tree[N][26];// trie 结构,tree[i][c] 节点 i 的字符 c 指向哪个节点,0 表示空节点
int pass[N];// pass[i] 节点 i 被路过几次
int ed[N];// end[i] 几个词以节点 i 结尾
int root = 1;// 根节点
int cnt = 1;// 节点数量,自带 1 个根节点

// 建树
void buildTrie(){
    root = 1;
    cnt = 1;
}

void clear(){
    for(int i = 0; i < cnt; i++){
        memset(tree[i], 0, sizeof(tree[i]));
        pass[i] = 0;
        ed[i] = 0;
    }
    cnt = 1;
}

// 将字符集映射为编号
int getPath(char c){
    return c - 'a';
}

// 将字符串 word 插入 Trie 中
void insert(string word){
    int node = root;
    pass[node]++;
    for(auto c : word){
        int path = getPath(c);
        if(tree[node][path] == 0){
            cnt++;
            tree[node][path] = cnt;
        }
        node = tree[node][path];
        pass[node]++;
    }
    ed[node]++;
}

int search(string word);
// 从 Trie 中移除字符串 word
void erase(string word){
    if(search(word) <= 0){
        return;
    }

    int node = root;
    pass[node]--;
    for(auto c : word){
        int path = getPath(c);
        if(pass[tree[node][path]] - 1 == 0){// 剪枝,如果下一个节点只被经过一次则其下所有节点全部删除
            tree[node][path] = 0;
            return;
        }
        node = tree[node][path];
        pass[node]--;
    }
    ed[node]--;
}

// 返回 Trie 中字符串 word 的实例个数
int search(string word){
    int node = root;
    for(auto c : word){
        int path = getPath(c);
        if(tree[node][path] == 0){
            return 0;
        }
        node = tree[node][path];
    }
    return ed[node];
}

// 返回 Trie 中以 prefix 为前缀的字符串个数
int prefixNumber(string prefix){
    int node = root;
    for(auto c : prefix){
        int path = getPath(c);
        if(tree[node][path] == 0){
            return 0;
        }
        node = tree[node][path];
    }
    return pass[node];
}

// 动态数组封装
class Trie{
public:
    class TrieNode{
    public:
        int pass;
        int end;
        vector<TrieNode*> next;// 路更多时使用哈希表

        TrieNode(){
            pass = 0;
            end = 0;
            next.resize(26, nullptr);
        }
    };

    TrieNode* root;

    Trie(){
        root = new TrieNode();
    }

    // 将字符串 word 插入 Trie 中
    void insert(string word){
        TrieNode* node = root;
        node->pass++;
        for(auto c : word){
            int path = c - 'a';
            if(node->next[path] == nullptr){
                node->next[path] = new TrieNode();
            }
            node = node->next[path];
            node->pass++;
        }
        node->end++;
    }

    // 从 Trie 中移除字符串 word
    void erase(string word){
        if(search(word) <= 0){
            return;
        }
        
        TrieNode* node = root;
        node->pass--;
        for(auto c : word){
            int path = c - 'a';
            if(node->next[path]->pass - 1 == 0){
                node->next[path] = nullptr;
                return;
            }
            node = node->next[path];
            node->pass--;
        }
        node->end--;
    }

    // 返回 Trie 中字符串 word 的实例个数
    int search(string word){
        TrieNode* node = root;
        for(auto c : word){
            int path = c - 'a';
            if(node->next[path] == nullptr){
                return 0;
            }
            node = node->next[path];
        }
        return node->end;
    }

    // 返回 Trie 中以 prefix 为前缀的字符串个数
    int prefixNumber(string prefix){
        TrieNode* node = root;
        for(auto c : prefix){
            int path = c - 'a';
            if(node->next[path] == nullptr){
                return 0;
            }
            node = node->next[path];
        }
        return node->pass;
    }
};

// 状态很大时的处理方法，将数字转换为字符串，按位拆开，并在每个数字后添加'#'表示数字结束
class Solution {
public:
    /**
     * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
     *
     * 
     * @param b int整型vector<vector<>> 
     * @param a int整型vector<vector<>> 
     * @return int整型vector
     */
    
    // 静态数组
    const static int N = 1e6 + 5;
    int tree[N][26];// trie 结构,tree[i][c] 节点 i 的字符 c 指向哪个节点,0 表示空节点
    int pass[N];// pass[i] 节点 i 被路过几次
    int ed[N];// end[i] 几个词以节点 i 结尾
    int root = 1;// 根节点
    int cnt = 1;// 节点数量,自带 1 个根节点

    // 建树
    void buildTrie(){
        root = 1;
        cnt = 1;
    }

    void clear(){
        for(int i = 0; i < N; i++){
            memset(tree[i], sizeof(tree[i]), 0);
            pass[i] = 0;
            ed[i] = 0;
        }
    }

    int getPath(char c){
        if(c == '#'){
            return 10;
        }
        else if(c == '-'){
            return 11;
        }
        else{
            return c - '0';
        }
    }

    // 将字符串 word 插入 Trie 中
    void insert(string word){
        int node = root;
        pass[node]++;
        for(auto c : word){
            int path = getPath(c);
            if(tree[node][path] == 0){
                cnt++;
                tree[node][path] = cnt;
            }
            node = tree[node][path];
            pass[node]++;
        }
        ed[node]++;
    }

    // 从 Trie 中移除字符串 word
    void erase(string word){
        if(search(word) <= 0){
            return;
        }

        int node = root;
        pass[node]--;
        for(auto c : word){
            int path = getPath(c);
            if(pass[tree[node][path]] - 1 == 0){// 剪枝,如果下一个节点只被经过一次则其下所有节点全部删除
                tree[node][path] = 0;
                return;
            }
            node = tree[node][path];
            pass[node]--;
        }
        ed[node]--;
    }

    // 返回 Trie 中字符串 word 的实例个数
    int search(string word){
        int node = root;
        for(auto c : word){
            int path = getPath(c);
            if(tree[node][path] == 0){
                return 0;
            }
            node = tree[node][path];
        }
        return ed[node];
    }

    // 返回 Trie 中以 prefix 为前缀的字符串个数
    int prefixNumber(string prefix){
        int node = root;
        for(auto c : prefix){
            int path = getPath(c);
            if(tree[node][path] == 0){
                return 0;
            }
            node = tree[node][path];
        }
        return pass[node];
    }

    vector<int> countConsistentKeys(vector<vector<int> >& b, vector<vector<int> >& a) {
        int m = b.size();
        int n = a.size();
        vector<int> ans(m, 0);
        vector<vector<int>> aa(n), bb(m);
        for(int i = 0; i < n; i++){
            string s;
            for(int j = 0; j < a[i].size() - 1; j++){
                s += to_string(a[i][j + 1] - a[i][j]) + '#';
            }
            insert(s);
        }

        for(int i = 0; i < m; i++){
            string s;
            for(int j = 0; j < b[i].size() - 1; j++){
                s += to_string(b[i][j + 1] - b[i][j]) + '#';
            }
            ans[i] = prefixNumber(s);
        }
        return ans;
    }
};