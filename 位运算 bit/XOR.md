#### 集合中两数最大异或值
01 Trie 求最大异或

```cpp
class Solution {
public:
    // 静态数组
    const static int N = 1e6 + 5;
    int tree[N][2];// trie 结构,tree[i][c] 节点 i 的字符 c 指向哪个节点,0 表示空节点
    int pass[N];// pass[i] 节点 i 被路过几次
    int ed[N];// end[i] 几个词以节点 i 结尾
    int root = 1;// 根节点
    int cnt = 1;// 节点数量,自带 1 个根节点
    int high = 0;// 最高有效位

    // 建树
    void buildTrie(vector<int>& nums){
        root = 1;
        cnt = 1;
        int ma = *max_element(nums.begin(), nums.end());
        for(int i = 31; i >= 0; i--){
            if(((ma >> i) & 1) == 1){
                high = i;
                break;
            }
        }
        for(auto num : nums){
            insert(num);
        }
    }

    // 清除
    void clear(){
        for(int i = 0; i < cnt; i++){
            memset(tree[i], 0, sizeof(tree[i]));
            pass[i] = 0;
            ed[i] = 0;
        }
        cnt = 1;
    }

    // 将字符串 word 插入 Trie 中
    void insert(int num){
        int node = root;
        pass[node]++;
        for(int i = high; i >= 0; i--){
            int path = (num >> i) & 1;
            if(tree[node][path] == 0){
                cnt++;
                tree[node][path] = cnt;
            }
            node = tree[node][path];
            pass[node]++;
        }
        ed[node]++;
    }

    // 返回最大异或值
    int maxXOR(int num){
        int ans = 0;
        int cur = root;
        for(int i = high; i >= 0; i--){
            int bit = (num >> i) & 1;// 第 i 位的状态
            int want = bit ^ 1;// 第 i 位希望遇到的状态

            if(tree[cur][want] != 0){
                cur = tree[cur][want];
                ans |= (want << i);
            }
            else{
                cur = tree[cur][bit];
                ans |= (bit << i);
            }
        }
        return ans ^ num;
    }

    int findMaximumXOR(vector<int>& nums) {
        buildTrie(nums);
        int ans = 0;
        for(auto num : nums){
            ans = max(ans, maxXOR(num));
        }
        clear();
        return ans;
    }
};
```

哈希表求最大异或