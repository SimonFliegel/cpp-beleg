#include <QDebug>
#include <QFile>
#include <QTextStream>
#include "administration.h"
#include "book.h"
#include "cd.h"
#include "dvd.h"

void Administration::registerUser(User* user) {
    userList.insert(nextUserId, user);
    nextUserId++;
}

void Administration::addMedia(Media* media) {
    mediaList.insert(nextMediaId, media);
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

Media* Administration::getMedia(int mediaId) {
    return mediaList.value(mediaId);
}

User* Administration::getUser(int userId) {
    return userList.value(userId);
}

bool Administration::lendMedia(int mediaId, int userId) {
    if (mediaList.contains(mediaId) && userList.contains(userId)) {
        User* user = userList.value(userId);
        if (user->lendMediaByUser(mediaId)) {
            return true;
        }
    }
    qDebug() << "mediaId: " << mediaId << " or userId: " << userId << " not in List";
    return false;
}

bool Administration::handBackMedia(int mediaId, int userId) {
    if (mediaList.contains(mediaId) && userList.contains(userId)) {
        User* user = userList.value(userId);
        Media* media = mediaList.value(mediaId);
        if (!media->getAvailability()) {
            media->changeAvailability();
            return user->handBackMediaByUser(mediaId);
        } else {
            qDebug() << "corrupted data: media not lent";
        }
    }
    return false;
}

bool Administration::saveUsers() {
    QFile file(userFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open user.csv for writing";
        return false;
    }
    QTextStream stream(&file);
    QString lentMediaString = "";
    QMap<int,User*>::const_iterator it_userList;
    QList<int>::const_iterator it_lentMedia;
    for (it_userList = userList.constBegin(); it_userList != userList.constEnd(); ++it_userList) {
        User* user = it_userList.value();
        for (it_lentMedia = user->getLentMedia().constBegin(); it_lentMedia != user->getLentMedia().constEnd(); ++it_lentMedia) {
            lentMediaString += QString::number(*it_lentMedia) +  " ";
        }
        qDebug() << "lentMediaString: " << lentMediaString; // test output
        // id,name,surname,lentMediaIds
        stream << it_userList.key() << "," << user->getName() << "," << user->getSurname() << "," << lentMediaString << Qt::endl;
    }
    file.close();
    return true;
}

bool Administration::saveMedia() {
    QFile file(mediaFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to open media.csv for writing";
        return false;
    }
    QTextStream stream(&file);
    QMap<int,Media*>::const_iterator it_mediaList;
    for (it_mediaList = mediaList.constBegin(); it_mediaList != mediaList.constEnd(); ++it_mediaList) {
        Media* media = it_mediaList.value();
        // id,title,availability[0,1],type[0..3],author/interpret/director/-1
        stream << it_mediaList.key() << "," << media->getTitle() << "," << (media->getAvailability() ? 0 : 1) << "," << media->getType() << ",";
        switch(media->getType()) {
        case BOOK:
            stream << static_cast<Book*>(media)->getAuthor();
            break;
        case CD:
            stream << static_cast<Cd*>(media)->getInterpret();
            break;
        case DVD:
            stream << static_cast<Dvd*>(media)->getDirector();
            break;
        default: // CUSTOM
            stream << QString::number(-1);
            break;
        }
        stream << Qt::endl;
    }
    file.close();
    return true;
}

bool Administration::loadUsers() {
    int mediaId;
    int userId;
    QString name, surname;
    QFile file(userFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open user.csv for reading";
        return false;
    }
    clearUsers();
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString dataLine = stream.readLine();
        QStringList values = dataLine.split(',');
        userId = values[0].toInt();
        name = values[1];
        surname = values[2];
        User user(name, surname);
        QStringList lentMediaIds = values[3].split(" ");
        foreach (const QString& str, lentMediaIds) {
            mediaId = str.toInt();
            user.lendMediaByUser(mediaId);
        }
        userList.insert(userId, &user);
        nextUserId = userId + 1;
    }
    return true;
}

bool Administration::loadMedia() {
    int mediaId;
    QString title;
    MediaType type;
    bool isAvailable;
    QString specialAttr;
    QFile file(mediaFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open media.csv for reading";
        return false;
    }
    clearMedia();
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString dataLine = stream.readLine();
        QStringList values = dataLine.split(',');
        mediaId = values[0].toInt();
        title = values[1];
        isAvailable = values[2].toInt();
        type = (MediaType)values[3].toInt();
        specialAttr = values[4];
        Media media(title, type);
        media.setAvailability(isAvailable);
        switch (type) {
        case BOOK:
            static_cast<Book*>(&media)->setAuthor(specialAttr);
            break;
        case CD:
            static_cast<Cd*>(&media)->setInterpret(specialAttr);
            break;
        case DVD:
            static_cast<Dvd*>(&media)->setDirector(specialAttr);
            break;
        default: //CUSTOM
            break;
        }
        mediaList.insert(mediaId, &media);
        nextMediaId = mediaId + 1;
    }
    return true;
}

void Administration::clearUsers() {
    qDebug() << "Clearing all users!";
    qDeleteAll(userList.begin(), userList.end());
    userList.clear();
}

void Administration::clearMedia() {
    qDebug() << "Clearing all media!";
    qDeleteAll(mediaList.begin(), mediaList.end());
    mediaList.clear();
}

bool Administration::clearFiles() {
    qDebug() << "Deleting files containing data!";
    if (!QFile::remove(userFilePath)) {
        return false;
    }
    if (!QFile::remove(mediaFilePath)) {
        return false;
    }
    return true;
}

bool checkDataForCorruption() {
    // @TODO
    return true;
}



