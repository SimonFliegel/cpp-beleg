/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#ifndef DVD_H
#define DVD_H

#include "media.h"

class Dvd : public Media
{
private:
    QString director = "";
public:
    Dvd(QString title);
    Dvd(QString title, QString director);
    QString getDirector();
    void setDirector(QString director);
};

#endif // DVD_H
