#ifndef CLION_ARRAYBASEDLIST01_H
#define CLION_ARRAYBASEDLIST01_H

#include <string>

using namespace std;

/**
 * 变长顺序表类，用于存储和管理String类型的数据
 * 学生: 周子超
 * 学号: 202430552999
 * 时间：2025-09-20
 */
class MyAList {
private:
    const int DEFAULT_CAPACITY = 5;  // 默认初始容量
    const double SHRINK_THRESHOLD = 0.25;  // 当数组中的元素为数组长度的0.25倍时进行缩容
    string* data;   // 存储数据的数组
    int capacity;        // 当前容量
    int size;            // 当前元素数量

    // 调整容量的辅助函数
    void resize(int newCapacity);

public:
    /**
     * 初始化顺序表
     * 时间复杂度: O(1)
     */
    MyAList();

    /**
     * 析构函数，释放动态分配的内存
     * 时间复杂度: O(1)
     */
    ~MyAList();

    /**
     * insert
     * 在特定位置插入元素
     *  index: 插入位置的索引
     *  element: 要插入的元素
     * 时间复杂度: O(n)，
     */
    void insert(int index, const string& element);

    /**
     * remove
     * 移除特定位置的元素
     * index: 要移除元素位置的索引
     * 时间复杂度: O(n)
     */
    void remove(int index);

    /**
     * append
     * 在最后元素后追加新元素
     * element: 要追加的元素
     * 时间复杂度: O(1)
     */
    void append(const string& element);

    /**
     * print
     * 遍历顺序表并输出所有元素
     * 时间复杂度: O(n)
     */
    void print() const;

    /**
     * find
     * 查找特定元素的位置（使用顺序检索）
     * element: 要查找的元素
     * return: 元素的索引（0-based），若未找到返回-1
     * 时间复杂度: O(n)
     */
    int find(const string& element) const;

    /**
     * position
     * 获取特定位置的元素
     * index: 要获取元素的索引（0-based）
     * return: 该位置的元素
     * 时间复杂度: O(1)
     */
    string position(int index) const;

    /**
     * isEmpty
     * 判定顺序表是否为空
     * @return: 若为空返回true，否则返回false
     * 时间复杂度: O(1)
     */
    bool isEmpty() const;

    /**
     * length
     * 获得顺序表的元素数量
     * @return: 元素数量
     * 时间复杂度: O(1)
     */
    int length() const;

    /**
     * getCapacity
     * 获取当前容量
     * @return: 当前容量
     * 时间复杂度: O(1)
     */
    int getCapacity() const;
};
#endif //CLION_ARRAYBASEDLIST01_H

