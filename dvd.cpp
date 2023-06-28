/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#include "dvd.h"

Dvd::Dvd(QString title) : Media(title, DVD) {}

Dvd::Dvd(QString title, QString director) : Media(title, DVD)
{
    this->director = director;
}

QString Dvd::getDirector() {
    return director;
}

void Dvd::setDirector(QString director) {
    this->director = director;
}
