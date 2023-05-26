#include "media.h"

Media::Media(QString title, MediaType type) {
    this->title = title;
    this->type = type;
    this->isAvailable = true;
}

QString Media::getTitle() {
    return title;
}

MediaType Media::getType() {
    return type;
}

bool Media::getAvailability() {
    return isAvailable;
}

void Media::setTitle(QString title) {
    this->title = title;
}

void Media::setAvailability(bool isAvailable) {
    this->isAvailable = isAvailable;
}

void Media::changeAvailability() {
    if (isAvailable) {
        isAvailable = false;
    } else {
        isAvailable = true;
    }
}




