#include "dvd.h"

Dvd::Dvd(QString title, QString director) : Media(title)
{
    this->director = director;
}
