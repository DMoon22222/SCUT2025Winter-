
#ifndef HOMEWORKPROGRAM3_HUFFMAN_H
#define HOMEWORKPROGRAM3_HUFFMAN_H
/**
 * @file huffman.h
 * @brief Huffman编码的头文件定义
 * @details 实现Huffman编码的类声明和数据结构定义
 *
 * 合法符号：26个小写英文字母(a-z)，不区分大小写（统一转为小写）
 */


#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <memory>
using namespace std;

// Huffman树节点类
class HuffmanNode {
public:
    char data;          // 字符（内部节点为'\0'）
    int freq;           // 频率
    std::shared_ptr<HuffmanNode> left;   // 左子节点
    std::shared_ptr<HuffmanNode> right;  // 右子节点

    HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}

    // 比较操作符重载（用于优先队列）
    bool operator>(const HuffmanNode& other) const {
        return freq > other.freq;
    }
};

// 比较函数对象（用于优先队列）
struct CompareNode {
    bool operator()(const std::shared_ptr<HuffmanNode>& a,
                    const std::shared_ptr<HuffmanNode>& b) const {
        return a->freq > b->freq;
    }
};

// 符号频次统计类
class SymbolCounts {
private:
    std::unordered_map<char, int> frequencyMap;  // 字符频率映射

public:
    /**
     * @brief 统计文本文件中字符频率
     * @param filename 输入文件名
     * @return bool 成功返回true，失败返回false
     */
    bool countFromFile(const std::string& filename);

    /**
     * @brief 打印字符频率统计结果
     */
    void printCounts() const;

    /**
     * @brief 获取频率映射
     * @return const std::unordered_map<char, int>& 频率映射的常量引用
     */
    const std::unordered_map<char, int>& getFrequencyMap() const {
        return frequencyMap;
    }
};

// Huffman编码类
class MyHuffman {
private:
    std::shared_ptr<HuffmanNode> root;                   // Huffman树根节点
    std::unordered_map<char, std::string> huffmanCodes; // 字符到编码的映射
    std::unordered_map<std::string, char> reverseCodes; // 编码到字符的映射

    /**
     * @brief 构建Huffman树
     * @param frequencyMap 字符频率映射
     */
    void buildTree(const std::unordered_map<char, int>& frequencyMap);

    /**
     * @brief 递归生成Huffman编码
     * @param node 当前节点
     * @param code 当前编码字符串
     */
    void generateCodes(const std::shared_ptr<HuffmanNode>& node, const std::string& code);

    /**
     * @brief 递归删除Huffman树（使用智能指针，自动管理内存）
     * @param node 当前节点
     */
    void deleteTree(std::shared_ptr<HuffmanNode> node);

public:
    MyHuffman() : root(nullptr) {}
    ~MyHuffman() { deleteTree(root); }

    /**
     * @brief 使用字符频率映射初始化Huffman编码
     * @param frequencyMap 字符频率映射
     */
    void initialize(const std::unordered_map<char, int>& frequencyMap);

    /**
     * @brief 编码文本文件
     * @param inputFile 输入文件名
     * @param outputFile 输出编码文件名
     * @return bool 成功返回true，失败返回false
     */
    bool encode(const std::string& inputFile, const std::string& outputFile);

    /**
     * @brief 解码Huffman编码文件
     * @param inputFile 输入编码文件名
     * @param outputFile 输出解码文件名
     * @return bool 成功返回true，失败返回false
     */
    bool decode(const std::string& inputFile, const std::string& outputFile);

    /**
     * @brief 计算平均编码成本
     * @param frequencyMap 字符频率映射
     * @return double 平均编码成本（比特/字符）
     */
    double calculateAverageCost(const std::unordered_map<char, int>& frequencyMap) const;

    /**
     * @brief 打印Huffman编码表
     */
    void printCodeTable() const;
};


#endif //HOMEWORKPROGRAM3_HUFFMAN_H
