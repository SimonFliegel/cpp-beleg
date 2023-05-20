#include "administration.h"

void Administration::registerUser(User* user) {
    user->setId(nextUserId);
    userList.insert(nextUserId, *user);
    nextUserId++;
}

void Administration::addMedia(Media* media) {
    media->setId(nextMediaId);
    mediaList.insert(media->getId(), *media);
    nextMediaId++;
}

bool Administration::deleteUser(int userId) {
    if (userList.remove(userId)) {
        return true;
    }
    return false;
}

bool Administration::deleteMedia(int mediaId) {
    if (mediaList.remove(mediaId)) {
        return true;
    }
    return false;
}

bool Administration::lendMedia(int mediaId, int userId) {
    if (mediaList.contains(mediaId) && userList.contains(userId)) {
        lendList.insert(mediaId, userId);
        return true;
    }
    return false;
}

bool Administration::handBackMedia(int mediaId) {
    if (lendList.contains(mediaId)) {
        lendList.remove(mediaId);
        return true;
    }
    return false;
}

QList<Media> getLendedMedia(int userId) {
    QList lendedMedia;
    QListIterator<Media>
}


