#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>
#include <QDate>

class User
{
private:
    QString name;
    QString surname;
    QList<int> lentMedia;

public:
    User(QString name, QString surname);
    QString getName();
    QString getSurname();
    QList<int>& getLentMedia();
    void setName(QString name);
    void setSurname(QString surname);
    bool lendMediaByUser(int mediaId);
    bool handBackMediaByUser(int mediaId);
};

#endif // USER_H
