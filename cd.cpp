#include "cd.h"

Cd::Cd(QString title, QString interpret) : Media(title)
{
    this->interpret = interpret;
}

QString Cd::getInterpret() {
    return interpret;
}

void Cd::setInterpret(QString interpret) {
    this->interpret = interpret;
}