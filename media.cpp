#include "media.h"

Media::Media(QString title) {
    this->title = title;
}

QString Media::getTitle() {
    return title;
}

int Media::getId() {
    return id;
}

void Media::setTitle(QString title) {
    this->title = title;
}

void Media::setId(int id) {
    this->id = id;
}
