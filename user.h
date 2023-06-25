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
    QString email;
    QDate birthdate;
    QList<int> lentMedia;

public:
    User(QString name, QString surname, QString email, QDate birthdate);
    QString getName();
    QString getSurname();
    QString getEmail();
    QDate getBirthdate();
    QList<int>& getLentMedia();
    void setName(QString name);
    void setSurname(QString surname);
    void setEmail(QString email);
    void setBirthdate(QDate birthdate);
    bool lendMediaByUser(int mediaId);
    bool handBackMediaByUser(int mediaId);
};

#endif // USER_H
