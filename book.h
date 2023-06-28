/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#ifndef BOOK_H
#define BOOK_H

#include "media.h"

class Book : public Media
{
private:
    QString author = "";

public:
    Book(QString title);
    Book(QString title, QString author);
    QString getAuthor();
    void setAuthor(QString author);
};

#endif // BOOK_H
