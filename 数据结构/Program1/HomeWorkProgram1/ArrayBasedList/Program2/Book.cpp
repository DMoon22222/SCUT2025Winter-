
/**
* 学生: 周子超
* 学号: 202430552999
* 时间: 2025-09-23
 **/
#include<string>
#include<iostream>
#include<iomanip>
#include"Book.h"

using namespace std;
Book::Book(string isbn,string name,string author,double price,string publisher,int printRun)
:isbn(isbn),name(name),author(author),price(price),publisher(publisher),printRun(printRun),isActive(true){};
string Book::getIsbn() const {
    return isbn;
}

string Book::getName() const {
    return name;
}

string Book::getAuthor() const {
    return author;
}

double Book::getPrice() const {
    return price;
}

string Book::getPublisher() const {
    return publisher;
}

int Book::getPrintRun() const {
    return printRun;
}

bool Book::isBookActive() const {
    return isActive;
}

void Book::takeDown() {
    isActive = false;
}

void Book::putUp() {
    isActive = true;
}
void Book::display() const {
    cout << left
         << setw(15) << isbn
         << setw(20) << name
         << setw(15) << author
         << setw(10) << price
         << setw(20) << publisher
         << setw(8) << printRun
         << (isActive ? "上架" : "下架") << endl;
}