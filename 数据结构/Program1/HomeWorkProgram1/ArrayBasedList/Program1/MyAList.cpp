#include "ArrayBasedList/Program2/MyAList.h"
#include <iostream>
#include <algorithm>

using namespace std;
/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-20
 **/
/**
 * 构造函数：初始化顺序表
 */
MyAList::MyAList() : capacity(DEFAULT_CAPACITY), size(0) {
    data = new string[capacity];
}

/**
 * 析构函数：释放动态分配的内存
 */
MyAList::~MyAList() {
    delete[] data;
}

/**
 * 调整容量的方法
 * 扩容条件：当元素数量达到当前容量时，将容量扩大为原来的2倍
 * 缩容条件：当元素数量小于当前容量的SHRINK_THRESHOLD（25%）且当前容量大于默认容量时，
 *          将容量缩小为原来的1/2，但不小于默认容量
 * newCapacity: 新的容量大小
 * 时间复杂度: O(n)
 */
void MyAList::resize(int newCapacity) {
    if (newCapacity < size) {
        cerr << "错误：新容量不能小于当前元素数量" << endl;
        return;
    }

    // 不允许容量小于默认容量
    newCapacity = max(newCapacity, DEFAULT_CAPACITY);

    // 如果新容量和当前容量相同，则不需要调整
    if (newCapacity == capacity) {
        return;
    }

    // 分配新的内存空间
    string* newData = new string[newCapacity];

    // 复制元素到新的内存空间
    for (int i = 0; i < size; i++) {
        newData[i] = data[i];
    }

    // 释放旧的内存空间
    delete[] data;

    // 更新数据指针和容量
    data = newData;
    capacity = newCapacity;

    cout << "已调整容量为: " << capacity << endl;
}

/**
 * 在特定位置插入元素
 * index: 插入位置的索引
 * element: 要插入的元素
 */
void MyAList::insert(int index, const string& element) {
    // 检查索引是否有效
    if (index < 0 || index > size) {
        cerr << "错误：插入位置索引无效" << endl;
        return;
    }

    // 如果容量已满，进行扩容（扩大为原来的2倍）
    if (size == capacity) {
        resize(2 * capacity);
    }

    // 将插入位置及之后的元素向后移动一位
    for (int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }

    // 插入新元素
    data[index] = element;
    size++;
}

/**
 * 移除特定位置的元素
 * index: 要移除元素的索引（0-based）
 */
void MyAList::remove(int index) {
    // 检查顺序表是否为空
    if (isEmpty()) {
        cerr << "错误：顺序表为空，无法移除元素" << endl;
        return;
    }

    // 检查索引是否有效
    if (index < 0 || index >= size) {
        cerr << "错误：移除位置索引无效" << endl;
        return;
    }

    // 将移除位置之后的元素向前移动一位
    for (int i = index; i < size - 1; i++) {
        data[i] = data[i + 1];
    }

    size--;

    // 检查是否需要缩容
    if (size <= capacity * SHRINK_THRESHOLD && capacity > DEFAULT_CAPACITY) {
        resize(capacity / 2);
    }
}

/**
 * 在最后元素后追加新元素
 * element: 要追加的元素
 */
void MyAList::append(const string& element) {
    insert(size, element);
}

/**
 * 遍历顺序表并输出所有元素
 */
void MyAList::print() const {
    cout << "顺序表元素 (" << size << "/" << capacity << "): ";
    for (int i = 0; i < size; i++) {
        cout << data[i];
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

/**
 * 查找特定元素的位置
 * element: 要查找的元素
 * return: 元素的索引，若未找到返回-1
 */
int MyAList::find(const string& element) const {
    for (int i = 0; i < size; i++) {
        if (data[i] == element) {
            return i;
        }
    }
    return -1;
}

/**
 * 获取特定位置的元素
 * index: 要获取元素的索引
 * return: 该位置的元素，若索引无效返回空字符串
 */
string MyAList::position(int index) const {
    if (index < 0 || index >= size) {
        cerr << "错误：位置索引无效" << endl;
        return"";
    }
    return data[index];
}

/**
 * 判定顺序表是否为空
 * return: 若为空返回true，否则返回false
 */
bool MyAList::isEmpty() const
{
    return size == 0;
}

/**
 * 获得顺序表的元素数量
 * return: 元素数量
 */
int MyAList::length() const
{
    return size;
}

/**
 * 获取当前容量
 * return: 当前容量
 */
int MyAList::getCapacity() const
{
    return capacity;
}
