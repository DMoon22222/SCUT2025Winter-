/**
 * @file huffman.cpp
 * @brief Huffman编码的实现文件
 * @details 实现符号频次统计和Huffman编解码功能
 */

#include "huffman.h"
#include <algorithm>
#include <sstream>

using namespace std;

// ==================== SymbolCounts 类实现 ====================

bool SymbolCounts::countFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "错误：无法打开文件 " << filename << endl;
        return false;
    }

    frequencyMap.clear();
    char ch;
    int totalChars = 0;

    while (file.get(ch)) {
        // 只统计26个小写英文字母，大写转为小写
        if (isalpha(ch)) {
            char lowerCh = tolower(ch);
            frequencyMap[lowerCh]++;
            totalChars++;
        }
    }

    file.close();

    if (totalChars < 6000) {
        cerr << "警告：文本中合法符号数量(" << totalChars << ")少于6000" << endl;
    }

    cout << "成功统计字符频率，总合法字符数: " << totalChars << endl;
    return true;
}

void SymbolCounts::printCounts() const {
    cout << "=== 字符频率统计结果 ===" << endl;

    // 将字符按频率排序输出
    vector<pair<char, int>> sortedChars(frequencyMap.begin(), frequencyMap.end());
    sort(sortedChars.begin(), sortedChars.end(),
         [](const pair<char, int>& a, const pair<char, int>& b) {
             return a.second > b.second;
         });

    for (const auto& pair : sortedChars) {
        cout << "字符 '" << pair.first << "': " << pair.second << " 次" << endl;
    }
    cout << "总字符数: " << sortedChars.size() << endl;
}

// ==================== MyHuffman 类私有方法实现 ====================

void MyHuffman::buildTree(const unordered_map<char, int>& frequencyMap) {
    if (frequencyMap.empty()) {
        root = nullptr;
        return;
    }

    // 使用最小堆（优先队列）构建Huffman树
    priority_queue<shared_ptr<HuffmanNode>,
            vector<shared_ptr<HuffmanNode>>,
            CompareNode> minHeap;

    // 为每个字符创建叶节点并加入最小堆
    for (const auto& pair : frequencyMap) {
        minHeap.push(make_shared<HuffmanNode>(pair.first, pair.second));
    }

    // 构建Huffman树
    while (minHeap.size() > 1) {
        // 取出频率最小的两个节点
        auto left = minHeap.top();
        minHeap.pop();
        auto right = minHeap.top();
        minHeap.pop();

        // 创建新内部节点，频率为子节点频率之和
        auto newNode = make_shared<HuffmanNode>('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    // 最后剩下的节点就是根节点
    root = minHeap.top();
}

void MyHuffman::generateCodes(const shared_ptr<HuffmanNode>& node, const string& code) {
    if (!node) return;

    // 如果是叶节点，保存编码
    if (!node->left && !node->right) {
        huffmanCodes[node->data] = code;
        reverseCodes[code] = node->data;
        return;
    }

    // 递归生成左子树和右子树的编码
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

void MyHuffman::deleteTree(shared_ptr<HuffmanNode> node) {
    // 使用智能指针，自动管理内存，无需手动删除
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
    }
}

// ==================== MyHuffman 类公有方法实现 ====================

void MyHuffman::initialize(const unordered_map<char, int>& frequencyMap) {
    // 清空之前的编码
    huffmanCodes.clear();
    reverseCodes.clear();

    // 构建Huffman树
    buildTree(frequencyMap);

    // 生成Huffman编码
    if (root) {
        generateCodes(root, "");
    }
}

bool MyHuffman::encode(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        cerr << "错误：无法打开输入文件 " << inputFile << endl;
        return false;
    }

    if (!outFile.is_open()) {
        cerr << "错误：无法创建输出文件 " << outputFile << endl;
        inFile.close();
        return false;
    }

    char ch;
    while (inFile.get(ch)) {
        if (isalpha(ch)) {
            char lowerCh = tolower(ch);
            auto it = huffmanCodes.find(lowerCh);
            if (it != huffmanCodes.end()) {
                outFile << it->second;
            }
        }
    }

    inFile.close();
    outFile.close();
    cout << "编码完成，结果保存在: " << outputFile << endl;
    return true;
}

bool MyHuffman::decode(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile.is_open()) {
        cerr << "错误：无法打开编码文件 " << inputFile << endl;
        return false;
    }

    if (!outFile.is_open()) {
        cerr << "错误：无法创建解码文件 " << outputFile << endl;
        inFile.close();
        return false;
    }

    string code;
    char bit;
    auto currentNode = root;

    while (inFile.get(bit)) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else if (bit == '1') {
            currentNode = currentNode->right;
        } else {
            cerr << "错误：编码文件中包含非法字符 '" << bit << "'" << endl;
            inFile.close();
            outFile.close();
            return false;
        }

        // 如果到达叶节点，输出字符并回到根节点
        if (!currentNode->left && !currentNode->right) {
            outFile << currentNode->data;
            currentNode = root;
        }
    }

    inFile.close();
    outFile.close();
    cout << "解码完成，结果保存在: " << outputFile << endl;
    return true;
}

double MyHuffman::calculateAverageCost(const unordered_map<char, int>& frequencyMap) const {
    int totalChars = 0;
    double totalBits = 0.0;

    for (const auto& pair : frequencyMap) {
        totalChars += pair.second;
        auto it = huffmanCodes.find(pair.first);
        if (it != huffmanCodes.end()) {
            totalBits += pair.second * it->second.length();
        }
    }

    if (totalChars == 0) return 0.0;

    return totalBits / totalChars;
}

void MyHuffman::printCodeTable() const {
    cout << "=== Huffman编码表 ===" << endl;

    vector<pair<char, string>> sortedCodes(huffmanCodes.begin(), huffmanCodes.end());
    sort(sortedCodes.begin(), sortedCodes.end(),
         [](const pair<char, string>& a, const pair<char, string>& b) {
             return a.first < b.first;
         });

    for (const auto& pair : sortedCodes) {
        cout << "字符 '" << pair.first << "': " << pair.second << endl;
    }
}
