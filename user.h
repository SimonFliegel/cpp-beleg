#ifndef USER_H
#define USER_H

#include <QString>
#include <QDate>

class user
{
private:
    QString name;
    QString surname;
    QString email;
    QDate bday;

public:
    user(QString name, QString surname, QString email, QDate bday);

    QString getName();
    QString getSurname();
    QString getEmail();
    QDate getBirthday();
    void setName(QString name);
    void setSurname(QString surname);
    void setEmail(QString email);
    void setBirthday(QDate bday);
};

#endif // USER_H
