#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User
{
private:
    int id;
    QString name;
    QString surname;
    QList<int> lendedMedia;

public:
    User(QString name, QString surname);
    int getId();
    QString getName();
    QString getSurname();
    void setId(int id);
    void setName(QString name);
    void setSurname(QString surname);
    bool lendMedia(int mediaId);
};

#endif // USER_H
