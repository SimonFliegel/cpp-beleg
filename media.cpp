#include "media.h"

Media::Media(QString title, MediaType type) {
    this->title = title;
    this->type = type;
    this->userId = -1;
}

QString Media::getTitle() {
    return title;
}

MediaType Media::getType() {
    return type;
}

int Media::getUserId() {
    return userId;
}

void Media::setTitle(QString title) {
    this->title = title;
}

void Media::setUserId(int userId) {
    this->userId = userId;
}




