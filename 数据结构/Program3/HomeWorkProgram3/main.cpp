/**
 * @file main.cpp
 * @brief Huffman编码主程序
 * @details 测试符号频次统计和Huffman编解码功能
 */

#include "huffman.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== Huffman编码实现 ===" << endl;
    cout << "合法符号: 26个小写英文字母(a-z)" << endl << endl;

    // 文件路径
    const string inputFile = "F:\\untitled2\\untitled\\HomeWorkProgram3\\input.txt";           // 原始文本文件
    const string encodedFile = "F:\\untitled2\\untitled\\HomeWorkProgram3\\encoded.txt";       // 编码后文件
    const string decodedFile = "F:\\untitled2\\untitled\\HomeWorkProgram3\\decoded.txt";       // 解码后文件

    // 1. 符号频次统计
    cout << "1. 符号频次统计..." << endl;
    SymbolCounts counter;

    if (!counter.countFromFile(inputFile)) {
        cerr << "符号频次统计失败!" << endl;
        return -1;
    }

    counter.printCounts();
    cout << endl;

    // 2. Huffman编码初始化
    cout << "2. 构建Huffman树和编码表..." << endl;
    MyHuffman huffman;
    huffman.initialize(counter.getFrequencyMap());

    huffman.printCodeTable();
    cout << endl;

    // 3. 计算平均编码成本
    cout << "3. 计算平均编码成本..." << endl;
    double avgCost = huffman.calculateAverageCost(counter.getFrequencyMap());
    cout << "平均编码成本: " << avgCost << " 比特/字符" << endl;
    cout << endl;

    // 4. 编码
    cout << "4. 进行Huffman编码..." << endl;
    if (!huffman.encode(inputFile, encodedFile)) {
        cerr << "编码失败!" << endl;
        return -1;
    }
    cout << endl;

    // 5. 解码
    cout << "5. 进行Huffman解码..." << endl;
    if (!huffman.decode(encodedFile, decodedFile)) {
        cerr << "解码失败!" << endl;
        return -1;
    }
    cout << endl;

    cout << "=== Huffman编码完成 ===" << endl;
    cout << "原始文件: " << inputFile << endl;
    cout << "编码文件: " << encodedFile << endl;
    cout << "解码文件: " << decodedFile << endl;

    return 0;
}