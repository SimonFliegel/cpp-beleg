#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include "mediaTypes.h"

class Media
{
private:
    QString title;
    bool isAvailable;
    MediaType type = CUSTOM;
public:
    Media(QString title, MediaType type = CUSTOM);
    QString getTitle();
    MediaType getType();
    bool getAvailability();
    void setTitle(QString title);
    void setAvailability(bool isAvailable);
    void changeAvailability();
};

#endif // MEDIA_H
