#include "user.h"
#include "administration.h"

User::User(QString name, QString surname) {
    this->name = name;
    this->surname = surname;
}

int User::getId() {
    return id;
}

QString User::getName() {
    return this->name;
}

QString User::getSurname() {
    return this->surname;
}

void User::setId(int id) {
    this->id = id;
}

void User::setName(QString name) {
    this->name = name;
}

void User::setSurname(QString surname) {
    this->surname = surname;
}

bool lendMedia(int mediaId) {
    if (media)
}
