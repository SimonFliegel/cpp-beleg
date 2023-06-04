#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include "mediaTypes.h"

class Media
{
private:
    QString title;
    int userId;
    MediaType type = CUSTOM;
public:
    Media(QString title, MediaType type = CUSTOM);
    QString getTitle();
    MediaType getType();
    int getUserId();
    void setTitle(QString title);
    void setUserId(int userId);
};

#endif // MEDIA_H
