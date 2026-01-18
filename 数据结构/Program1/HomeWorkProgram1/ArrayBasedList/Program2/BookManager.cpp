/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-23
 **/
#include "BookManager.h"
#include "Book.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

BookManager::BookManager() {
    loadFromFile();
}
BookManager::~BookManager() {
    saveToFile();
    // 释放动态分配的Book对象
    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        delete book;
    }
}
void BookManager::loadFromFile() {
    ifstream file(DATA_FILE, ios::in);
    if (!file.is_open()) {
        // 首次运行时文件不存在，创建初始数据
        cout << "未找到数据文件，将创建新文件" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string isbn, name, author, publisher, priceStr, printRunStr;

        getline(ss, isbn, ',');
        getline(ss, name, ',');
        getline(ss, author, ',');
        getline(ss, priceStr, ',');
        getline(ss, publisher, ',');
        getline(ss, printRunStr, ',');

        double price = stod(priceStr);
        int printRun = stoi(printRunStr);

        Book* book = new Book(isbn, name, author, price, publisher, printRun);
        // 将指针转换为字符串存储
        bookList.append(to_string(reinterpret_cast<unsigned long long>(book)));
    }
    file.close();
    cout << "成功加载 " << bookList.length() << " 本图书数据" << endl;
}
void BookManager::saveToFile() {
    ofstream file(DATA_FILE, ios::out);
    if (!file.is_open()) {
        cerr << "无法打开文件进行写入" << endl;
        return;
    }

    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        if (book) {
            file << book->getIsbn() << ","
                 << book->getName() << ","
                 << book->getAuthor() << ","
                 << book->getPrice() << ","
                 << book->getPublisher() << ","
                 << book->getPrintRun() << endl;
        }
    }
    file.close();
    cout << "已保存 " << bookList.length() << " 本图书数据" << endl;
}
void BookManager::addBook(const Book& book) {
    // 检查ISBN是否已存在
    if (findBookByIsbn(book.getIsbn())) {
        cout << "错误：该ISBN已存在" << endl;
        return;
    }

    Book* newBook = new Book(book.getIsbn(), book.getName(), book.getAuthor(),
                             book.getPrice(), book.getPublisher(), book.getPrintRun());
    bookList.append(to_string(reinterpret_cast<unsigned long long>(newBook)));
    cout << "图书上架成功！" << endl;
}
void BookManager::findBookByName(const string& name) {
    bool found = false;
    cout << left << setw(15) << "ISBN"
         << setw(20) << "书名"
         << setw(15) << "作者"
         << setw(10) << "定价"
         << setw(20) << "出版社"
         << setw(8) << "印次"
         << "状态" << endl;
    cout << string(90, '-') << endl;

    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        if (book && book->getName().find(name) != string::npos) {
            book->display();
            found = true;
        }
    }

    if (!found) {
        cout << "未找到包含 [" << name << "] 的图书" << endl;
    }
}

Book* BookManager::findBookByIsbn(const string& isbn) {
    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        if (book && book->getIsbn() == isbn) {
            return book;
        }
    }
    return nullptr;
}

bool BookManager::takeDownBook(const string& isbn) {
    Book* book = findBookByIsbn(isbn);
    if (book) {
        book->takeDown();
        cout << "图书已下架" << endl;
        return true;
    }
    cout << "未找到该图书" << endl;
    return false;
}

bool BookManager::deleteBook(const string& isbn) {
    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        if (book && book->getIsbn() == isbn) {
            delete book;
            bookList.remove(i);
            cout << "图书已删除" << endl;
            return true;
        }
    }
    cout << "未找到该图书" << endl;
    return false;
}

void BookManager::displayAllBooks() {
    if (bookList.length() == 0) {
        cout << "当前没有图书数据" << endl;
        return;
    }

    cout << left << setw(15) << "ISBN"
         << setw(20) << "书名"
         << setw(15) << "作者"
         << setw(10) << "定价"
         << setw(20) << "出版社"
         << setw(8) << "印次"
         << "状态" << endl;
    cout << string(90, '-') << endl;

    for (int i = 0; i < bookList.length(); i++) {
        Book* book = reinterpret_cast<Book*>(stoull(bookList.position(i)));
        if (book) {
            book->display();
        }
    }
}

void BookManager::showMenu() {
    cout << "\n===== 图书管理系统 =====" << endl;
    cout << "1. 图书上架" << endl;
    cout << "2. 按书名查找图书" << endl;
    cout << "3. 按ISBN查找图书" << endl;
    cout << "4. 图书下架" << endl;
    cout << "5. 删除图书" << endl;
    cout << "6. 显示所有图书" << endl;
    cout << "0. 退出系统" << endl;
    cout << "请选择操作: ";
}

void BookManager::run() {
    int choice;
    string isbn, name, author, publisher;
    double price;
    int printRun;

    do {
        showMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:  // 图书上架
                cout << "请输入ISBN号: ";
                getline(cin, isbn);
                cout << "请输入书名: ";
                getline(cin, name);
                cout << "请输入作者: ";
                getline(cin, author);
                cout << "请输入定价: ";
                cin >> price;
                cin.ignore();
                cout << "请输入出版社: ";
                getline(cin, publisher);
                cout << "请输入印次: ";
                cin >> printRun;
                cin.ignore();

                addBook(Book(isbn, name, author, price, publisher, printRun));
                break;

            case 2:  // 按书名查找
                cout << "请输入要查找的书名: ";
                getline(cin, name);
                findBookByName(name);
                break;

            case 3:  // 按ISBN查找
                cout << "请输入要查找的ISBN号: ";
                getline(cin, isbn); {
            Book* book = findBookByIsbn(isbn);
            if (book) {
                cout << left << setw(15) << "ISBN"
                     << setw(20) << "书名"
                     << setw(15) << "作者"
                     << setw(10) << "定价"
                     << setw(20) << "出版社"
                     << setw(8) << "印次"
                     << "状态" << endl;
                cout << string(90, '-') << endl;
                book->display();
            } else {
                cout << "未找到该ISBN号的图书" << endl;
            }
        }
                break;

            case 4:  // 图书下架
                cout << "请输入要下架的图书的ISBN号: ";
                getline(cin, isbn);
                takeDownBook(isbn);
                break;

            case 5:  // 彻底删除
                cout << "请输入要删除的图书的ISBN号: ";
                getline(cin, isbn);
                deleteBook(isbn);
                break;

            case 6:  // 显示所有
                displayAllBooks();
                break;

            case 0:  // 退出
                cout << "感谢使用！" << endl;
                break;

            default:
                cout << "无效选择，请重新输入" << endl;
        }
    } while (choice != 0);
}

