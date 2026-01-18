/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-23
 **/
#ifndef HOMEWORKPROGRAM1_BOOKMANAGER_H
#define HOMEWORKPROGRAM1_BOOKMANAGER_H
#include "../Program1/MyAList.h"
#include "Book.h"
#include <string>
using namespace std;
/**
 * 图书管理类，基于MyAList实现图书管理功能
 */
class BookManager
{
private:
    MyAList bookList;//存储图书指针的顺序表
    const string DATA_FILE = "books.dat";//数据文件路径
    //从文件加载图书数据
    void loadFromFile();
    //保存图书数据到文件
    void saveToFile();
public:
    BookManager();
    ~BookManager();
    /**
     * 图书上架
     *  book 要添加的图书对象
     */
     void addBook(const Book&book);
    /**
   * 按书名查找图书
   *  name 要查找的书名
   */
    void findBookByName(const string& name);

    /**
     * 按ISBN查找图书
     * isbn 要查找的ISBN号
     */
    Book* findBookByIsbn(const string& isbn);

    /**
     * 图书下架（懒删除）
     * isbn 要下架的图书ISBN号
     */
    bool takeDownBook(const string& isbn);

    /**
     * 彻底删除图书
     * isbn 要删除的图书的ISBN号
     */
    bool deleteBook(const string& isbn);

    /**
     * 显示所有图书
     */
    void displayAllBooks();

    /**
     * 显示菜单
     */
    void showMenu();

    /**
     * 运行管理系统
     */
    void run();
};



#endif //HOMEWORKPROGRAM1_BOOKMANAGER_H
