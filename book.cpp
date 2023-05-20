#include "book.h"

Book::Book(QString title, QString author) : Media(title)
{
    this->author = author;
}

QString Book::getAuthor()
{
    return author;
}

void Book::setAuthor(QString author)
{
    this->author = author;
}
