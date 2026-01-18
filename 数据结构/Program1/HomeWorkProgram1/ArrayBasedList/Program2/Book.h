
#ifndef HOMEWORKPROGRAM1_BOOK_H
#define HOMEWORKPROGRAM1_BOOK_H
#include <string>
using namespace std;

/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-23
 **/

/**
 * 图书类，存储图书的信息
 */
class Book
{
private:
    string isbn;
    string name;
    string author;
    string publisher;
    double price;
    int printRun;
    bool isActive;
public:
    Book(string isbn, string name, string author,double price, string publisher, int printRun);
    //获取ISBN号
    string getIsbn() const;
    //获取书名
    string getName() const;
    //获取作者
    string getAuthor() const;
    //获取出版社
    string getPublisher() const;
    //获取打印次数
    int getPrintRun() const;
    //获取价格
    double getPrice() const;
    //检查是否上架
    bool isBookActive() const;
    //图书下架
    void takeDown();
    //图书上架
    void putUp();
    //打印图书信息
    void display() const;

};

#endif //HOMEWORKPROGRAM1_BOOK_H
