/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include "mediaTypes.h"

class Media
{
private:
    QString title;
    int userId;
    MediaType type;
public:
    Media(QString title, MediaType type = BOOK);
    QString getTitle();
    MediaType getType();
    int getUserId();
    void setTitle(QString title);
    void setUserId(int userId);
};

#endif // MEDIA_H
