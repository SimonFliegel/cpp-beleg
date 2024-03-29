/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#ifndef CD_H
#define CD_H

#include "media.h"

class Cd : public Media
{
private:
    QString interpret = "";
public:
    Cd(QString title);
    Cd(QString title, QString interpret);
    QString getInterpret();
    void setInterpret(QString);
};

#endif // CD_H
