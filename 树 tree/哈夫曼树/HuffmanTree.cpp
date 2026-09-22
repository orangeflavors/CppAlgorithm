// #include<bits/stdc++.h>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

// 哈夫曼树节点
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
};

// 用于优先队列的比较函数
struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// 构建哈夫曼树
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    
    // 为每个字符创建叶节点并加入优先队列
    for (auto& pair : freqMap) {
        pq.push(new Node(pair.first, pair.second));
    }
    
    // 处理只有一个字符的特殊情况
    if (pq.size() == 1) {
        Node* onlyNode = pq.top();
        Node* root = new Node('\0', onlyNode->freq);
        root->left = onlyNode;
        return root;
    }

    // 构建哈夫曼树
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        
        pq.push(parent);
    }
    
    return pq.top();
}

// 释放内存
void deleteHuffmanTree(Node* root) {
    if (!root) return;
    deleteHuffmanTree(root->left);
    deleteHuffmanTree(root->right);
    delete root;
}

// 生成哈夫曼编码
void generateHuffmanCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) return;
    
    // 如果是叶节点
    if (!root->left && !root->right) {
        // 处理只有一个字符的情况，编码为"0"
        if (str.empty()) {
            huffmanCode[root->ch] = "0";
        } else {
            huffmanCode[root->ch] = str;
        }
        return;
    }
    
    generateHuffmanCodes(root->left, str + "0", huffmanCode);
    generateHuffmanCodes(root->right, str + "1", huffmanCode);
}

// 计算原始ASCII编码的总比特数
int calculateOriginalBits(const string& text) {
    return text.length() * 8;
}

// 计算哈夫曼编码后的总比特数
int calculateHuffmanBits(const string& text, const unordered_map<char, string>& huffmanCode) {
    int totalBits = 0;
    for (char ch : text) {
        totalBits += huffmanCode.at(ch).length();
    }
    return totalBits;
}

// 打印字符频率和编码
void printFrequencyAndCodes(const unordered_map<char, int>& freqMap, const unordered_map<char, string>& huffmanCode) {
    cout << "字符频率和哈夫曼编码:" << endl;
    cout << "==============================================" << endl;
    cout << setw(12) << "字符" << setw(10) << "频率" << setw(20) << "哈夫曼编码" << setw(12) << "编码长度" << endl;
    cout << "----------------------------------------------" << endl;
    
    // 将字符按频率排序（频率高的在前）
    vector<pair<char, int>> sortedChars(freqMap.begin(), freqMap.end());
    sort(sortedChars.begin(), sortedChars.end(), 
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });
    
    for (auto& pair : sortedChars) {
        char ch = pair.first;
        int freq = pair.second;
        string code = huffmanCode.at(ch);
        string displayChar;
        
        // 处理空格和特殊字符的显示
        if (ch == ' ') {
            displayChar = "空格";
        } else if (ch == ',') {
            displayChar = "逗号";
        } else if (ch == '.') {
            displayChar = "句点";
        } else {
            displayChar = string(1, ch);
        }
        
        cout << setw(8) << displayChar 
             << setw(10) << freq 
             << setw(15) << code 
             << setw(10) << code.length() << endl;
    }
    cout << "==============================================" << endl;
}

int main() {
    // string text = "the coronavirus outbreak is a test of our systems, values and humanity.";
    string text = "the pandemic of covid is a test of social system";
    // string text;
    // cin >> text;
    
    if (text.empty()) {
        cout << "输入文本为空！" << endl;
        return 0;
    }

    cout << "原始文本: " << text << endl;
    cout << "文本长度: " << text.length() << " 个字符" << endl << endl;
    
    // 统计字符频率
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }
    
    // 构建哈夫曼树
    Node* root = buildHuffmanTree(freqMap);
    
    // 生成哈夫曼编码
    unordered_map<char, string> huffmanCode;
    generateHuffmanCodes(root, "", huffmanCode);
    
    // 打印频率和编码信息
    printFrequencyAndCodes(freqMap, huffmanCode);
    
    // 计算结果
    int originalBits = calculateOriginalBits(text);
    int huffmanBits = calculateHuffmanBits(text, huffmanCode);
    double compressionRatio = (1.0 - (double)huffmanBits / originalBits) * 100;
    
    cout << "\n计算结果:" << endl;
    cout << "==============================================" << endl;
    cout << "原始ASCII编码总比特数: " << originalBits << " bits" << endl;
    cout << "哈夫曼编码后总比特数: " << huffmanBits << " bits" << endl;
    cout << "压缩率: " << fixed << setprecision(2) << compressionRatio << "%" << endl;
    cout << "节省空间: " << (originalBits - huffmanBits) << " bits" << endl;
    cout << "==============================================" << endl;
    
    // 显示编码后的文本
    cout << "\n哈夫曼编码后的文本:" << endl;
    string encodedText;
    for (char ch : text) {
        encodedText += huffmanCode.at(ch);
    }
    cout << encodedText << endl;
    cout << "编码后长度: " << encodedText.length() << " bits" << endl;
    
    deleteHuffmanTree(root);
    
    return 0;
}