#include "../Program2/MyAList.h"
#include <iostream>

using namespace std;
/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-20
 **/
// 测试初始化和isEmpty方法
void testInitialization(MyAList& list) {
    cout << "\n=== 测试初始化 ===" << endl;
    cout << "列表是否为空: " << (list.isEmpty() ? "是" : "否") << endl;
    cout << "初始长度: " << list.length() << endl;
    cout << "初始容量: " << list.getCapacity() << endl;
}

// 测试append和print方法
void testAppend(MyAList& list) {
    cout << "\n=== 测试追加元素 ===" << endl;

    // 一些示例学号
    string ids[] = {"202430552996", "202430552997", "202430552998", "202430553000", "202430553001"};
    int numIds = sizeof(ids) / sizeof(ids[0]);

    for (int i = 0; i < numIds; i++) {
        list.append(ids[i]);
        cout << "追加元素: " << ids[i] << endl;
        list.print();
    }

    cout << "当前长度: " << list.length() << endl;
}

// 测试insert方法
void testInsert(MyAList& list) {
    cout << "\n=== 测试插入元素 ===" << endl;

    // 在开头插入
    list.insert(0, "202430552995");
    cout << "在位置0插入202430552995后:" << endl;
    list.print();

    // 在中间插入
    list.insert(3, "202430552999");
    cout << "在位置3插入202430552999后:" << endl;
    list.print();

    // 在末尾插入（等同于append）
    list.insert(list.length(), "202430553002");
    cout << "在末尾插入202430553002后:" << endl;
    list.print();

    cout << "当前长度: " << list.length() << endl;
}

// 测试find和position方法
void testFindAndPosition(MyAList& list) {
    cout << "\n=== 测试查找和获取元素 ===" << endl;

    string testId = "202430552999";
    int index = list.find(testId);
    if (index != -1) {
        cout << "查找元素 " << testId << " 的位置: " << index << endl;
        cout << "验证位置" << index << "的元素: " << list.position(index) << endl;
    } else {
        cout << "未找到元素 " << testId << endl;
    }

    testId = "202430554000";
    index = list.find(testId);
    if (index != -1) {
        cout << "查找元素 " << testId << " 的位置: " << index << endl;
    } else {
        cout << "未找到元素 " << testId << " (正确，因为该元素不存在)" << endl;
    }

    // 测试获取特定位置的元素
    cout << "位置2的元素: " << list.position(2) << endl;
    cout << "位置" << list.length() - 1 << "的元素: " << list.position(list.length() - 1) << endl;

    if (list.length() > 0) {
        cout << "最后一个元素: " << list.position(list.length() - 1) << endl;
    }
}

// 测试remove方法
void testRemove(MyAList& list) {
    cout << "\n=== 测试删除元素 ===" << endl;
    cout << "删除前:" << endl;
    list.print();

    // 删除中间元素
    list.remove(2);
    cout << "删除位置2的元素后:" << endl;
    list.print();

    // 删除第一个元素
    list.remove(0);
    cout << "删除位置0的元素后:" << endl;
    list.print();

    // 删除最后一个元素
    list.remove(list.length() - 1);
    cout << "删除最后一个元素后:" << endl;
    list.print();

    // 连续删除多个元素，测试缩容
    cout << "\n测试连续删除，观察是否缩容:" << endl;
    while (list.length() > 1) {
        list.remove(0);
        cout << "删除位置0的元素后:" << endl;
        list.print();
    }

    cout << "当前长度: " << list.length() << endl;
}

int main() {
    cout << "=== 变长顺序表测试程序 ===" << endl;

    MyAList list;

    // 执行各项测试
    testInitialization(list);
    testAppend(list);
    testInsert(list);
    testFindAndPosition(list);
    testRemove(list);

    cout << "\n=== 所有测试完成 ===" << endl;

    return 0;
}
