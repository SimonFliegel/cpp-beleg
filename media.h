#ifndef MEDIA_H
#define MEDIA_H

#include <QString>

class Media
{
private:
    int id;
    QString title;
public:
    Media(QString title);
    QString getTitle();
    int getId();
    void setTitle(QString title);
    void setId(int id);
};

#endif // MEDIA_H
