#include <QDebug>

#include "user.h"

User::User(QString name, QString surname) {
    this->name = name;
    this->surname = surname;
}

QString User::getName() {
    return name;
}

QString User::getSurname() {
    return surname;
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
