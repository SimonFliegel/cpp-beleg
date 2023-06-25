#include <QDebug>

#include "user.h"

User::User(QString name, QString surname, QString email, QDate birthdate) {
    this->name = name;
    this->surname = surname;
    this->email = email;
    this->birthdate = birthdate;
}

QString User::getName() {
    return name;
}

QString User::getSurname() {
    return surname;
}

QString User::getEmail() {
    return email;
}

QDate User::getBirthdate() {
    return birthdate;
}

QList<int>& User::getLentMedia() {
    return lentMedia;
}

void User::setName(QString name) {
    this->name = name;
}

void User::setSurname(QString surname) {
    this->surname = surname;
}

void User::setEmail(QString email) {
    this->email = email;
}

void User::setBirthdate(QDate birthdate) {
    this->birthdate = birthdate;
}

bool User::lendMediaByUser(int mediaId) {
    if (lentMedia.contains(mediaId)) {
        qDebug() << "mediaID: " << mediaId << " already in lentMedia-List";
        return false;
    }
    lentMedia.append(mediaId);
    return true;
}

bool User::handBackMediaByUser(int mediaId) {
    if (!lentMedia.contains(mediaId)) {
        qDebug() << "mediaID: " << mediaId << " not in lentMedia-List";
        return false;
    }
    return lentMedia.removeOne(mediaId);
}
