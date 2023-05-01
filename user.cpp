#include "user.h"

user::user(QString name, QString surname, QString email, QDate bday)
{
    this->name = name;
    this->surname = surname;
    this->email = email; // validate
    this->bday = bday;
}

QString user::getName() {
    return this->name;
}

QString user::getSurname() {
    return this->surname;
}

QString user::getEmail() {
    return this->email;
}

QDate user::getBirthday() {
    return this->bday;
}

void user::setName(QString name) {
    this->name = name;
}

void user::setSurname(QString surname) {
    this->surname = surname;
}

void user::setEmail(QString email) {
    // validate
    this->email = email;
}

void user::setBirthday(QDate bday) {
    this->bday = bday;
}


