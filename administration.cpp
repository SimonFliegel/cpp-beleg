/* Name: Simon Fliegel
 * Matr. Nr.: 53043
**************************************************************/

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <algorithm>

using namespace std;

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

void Administration::updateMediaType(int mediaId, MediaType newType) {
    if (!mediaList.contains(mediaId)) {
        qDebug() << "mediaId: " << mediaId << " not in List";
        return;
    }
    Media* oldMedia = getMedia(mediaId);
    Media* updatedMedia;

    switch (newType) {
    case BOOK:
        updatedMedia = new Book(oldMedia->getTitle());
        break;
    case CD:
        updatedMedia = new Cd(oldMedia->getTitle());
        break;
    case DVD:
        updatedMedia = new Dvd(oldMedia->getTitle());
        break;
    }

    delete oldMedia;
    mediaList.insert(mediaId, updatedMedia);
}

bool Administration::deleteUser(int userId) {
    if (!userList.contains(userId)) {
        return false;
    }
    returnAllMediaByUser(userId);
    delete userList.value(userId);
    userList.remove(userId);
    return true;
}

bool Administration::deleteMedia(int mediaId) {
    Media* media;
    if (!mediaList.contains(mediaId)) {
        return false;
    }
    media = mediaList.value(mediaId);
    if (media->getUserId() != -1) {
        return false;
    }
    delete mediaList.value(mediaId);
    mediaList.remove(mediaId);
    return true;
}

Media* Administration::getMedia(int mediaId) {
    return mediaList.value(mediaId);
}

User* Administration::getUser(int userId) {
    return userList.value(userId);
}

QMap<int,Media*>& Administration::getMediaList() {
    return mediaList;
}

QMap<int,User*>& Administration::getUserList() {
    return userList;
}

int Administration::getMediaCount() {
    return mediaList.count();
}

int Administration::getUserCount() {
    return userList.count();
}

/********************** private ******************************/

QMap<int,Media*> Administration::searchMediaByType(MediaType type) {
    QMap<int,Media*> results;
    QMap<int,Media*>::const_iterator it;

    for (it = mediaList.constBegin(); it != mediaList.constEnd(); ++it) {
        if (it.value()->getType() == type) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,Media*> Administration::searchMediaByTitle(QString title) {
    QMap<int,Media*> results;
    QMap<int,Media*>::const_iterator it;
    QString searchString = title.remove(' ').toLower();
    QString resultString;

    for (it = mediaList.constBegin(); it != mediaList.constEnd(); ++it) {
        resultString = it.value()->getTitle().remove(' ').toLower();
        if (resultString.contains(searchString)) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,Media*> Administration::searchMediaByCreator(QString creator) {
    QMap<int,Media*> results;
    QMap<int,Media*>::const_iterator it;
    QString searchString = creator.remove(' ').toLower();
    QString resultString;

    for (it = mediaList.constBegin(); it != mediaList.constEnd(); ++it) {
        MediaType type = it.value()->getType();
        switch (type) {
        case BOOK:
            resultString = static_cast<Book*>(it.value())->getAuthor();
            break;
        case CD:
            resultString = static_cast<Cd*>(it.value())->getInterpret();
            break;
        case DVD:
            resultString = static_cast<Dvd*>(it.value())->getDirector();
        }

        resultString = resultString.remove(' ').toLower();

        if (resultString.contains(searchString)) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,User*> Administration::searchUsersBySurname(QString surname) {
    QMap<int,User*> results;
    QMap<int,User*>::const_iterator it;
    QString searchString = surname.remove(' ').toLower();
    QString resultString;

    for (it = userList.constBegin(); it != userList.constEnd(); ++it) {
        resultString = it.value()->getSurname().remove(' ').toLower();
        if (resultString.contains(searchString)) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,User*> Administration::searchUsersByName(QString name) {
    QMap<int,User*> results;
    QMap<int,User*>::const_iterator it;
    QString searchString = name.remove(' ').toLower();
    QString resultString;

    for (it = userList.constBegin(); it != userList.constEnd(); ++it) {
        resultString = it.value()->getName().remove(' ').toLower();
        if (resultString.contains(searchString)) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,User*> Administration::searchUsersByEmail(QString email) {
    QMap<int,User*> results;
    QMap<int,User*>::const_iterator it;
    QString searchString = email.remove(' ').toLower();
    QString resultString;

    for (it = userList.constBegin(); it != userList.constEnd(); ++it) {
        resultString = it.value()->getEmail().remove(' ').toLower();
        if (resultString.contains(searchString)) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

/*********************** public *******************************/

QMap<int,Media*> Administration::searchMedia(MediaType type, QString title, QString creator) {
    QMap<int,Media*> results;
    QMap<int,Media*>::const_iterator it;
    QMap<int,Media*> typeResults;
    QMap<int,Media*> titleResults = mediaList;
    QMap<int,Media*> creatorResults = mediaList;

    if (!title.isEmpty()) {
        titleResults = searchMediaByTitle(title);
    }

    if (!creator.isEmpty()) {
        creatorResults = searchMediaByCreator(creator);
    }

    typeResults = searchMediaByType(type);

    for (it = typeResults.constBegin(); it != typeResults.constEnd(); ++it) {
        if (titleResults.contains(it.key()) && creatorResults.contains(it.key())) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

QMap<int,User*> Administration::searchUsers(QString surname, QString name, QString email) {
    QMap<int,User*> results;
    QMap<int,User*>::const_iterator it;
    QMap<int,User*> surnameResults = userList;
    QMap<int,User*> nameResults = userList;
    QMap<int,User*> emailResults = userList;

    if (!surname.isEmpty()) {
        surnameResults = searchUsersBySurname(surname);
    }

    if (!name.isEmpty()) {
        nameResults = searchUsersByName(name);
    }

    if (!email.isEmpty()) {
        emailResults = searchUsersByEmail(email);
    }

    for (it = userList.constBegin(); it != userList.constEnd(); ++it) {
        if (surnameResults.contains(it.key()) && nameResults.contains(it.key()) && emailResults.contains(it.key())) {
            results.insert(it.key(), it.value());
        }
    }

    return results;
}

bool Administration::lendMedia(int mediaId, int userId) {
    if (mediaList.contains(mediaId) && userList.contains(userId)) {
        if (userList.value(userId)->lendMediaByUser(mediaId)) {
            mediaList.value(mediaId)->setUserId(userId);
            return true;
        }
    }
    qDebug() << "mediaId: " << mediaId << " or userId: " << userId << " not in List";
    return false;
}

bool Administration::returnMedia(int mediaId, int userId) {
    if (mediaList.contains(mediaId) && userList.contains(userId)) {
        User* user = userList.value(userId);
        Media* media = mediaList.value(mediaId);
        if (media->getUserId() != -1) {
            if (user->handBackMediaByUser(mediaId)) {
                media->setUserId(-1);
                return true;
            }
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
    QString lentMediaString;
    QMap<int,User*>::const_iterator it_userList;

    for (it_userList = userList.constBegin(); it_userList != userList.constEnd(); ++it_userList) {
        User* user = it_userList.value();
        lentMediaString = "";
        if (user->getLentMedia().isEmpty()) {
            lentMediaString = "-1";
        } else {
            for (int lentMediaId : user->getLentMedia()) {
                lentMediaString += QString::number(lentMediaId) +  " ";
            }
        }

        // id,name,surname,email,birthdate,lentMediaIds/-1
        stream << it_userList.key() << "," << user->getName() << "," << user->getSurname() << "," << user->getEmail()
               << "," << user->getBirthdate().toString("dd.MM.yyyy") << "," << lentMediaString << Qt::endl;
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
    QString creator = "";

    for (it_mediaList = mediaList.constBegin(); it_mediaList != mediaList.constEnd(); ++it_mediaList) {
        Media* media = it_mediaList.value();
        // id,title,userId,type[0..3],author/interpret/director/-1
        stream << it_mediaList.key() << "," << media->getTitle() << "," << media->getUserId() << "," << media->getType() << ",";
        switch(media->getType()) {
        case BOOK:
            creator = static_cast<Book*>(media)->getAuthor();
            stream << (creator.length() > 0 ? creator : "-1");
            break;
        case CD:
            creator = static_cast<Cd*>(media)->getInterpret();
            stream << (creator.length() > 0 ? creator : "-1");
            break;
        case DVD:
            creator = static_cast<Dvd*>(media)->getDirector();
            stream << (creator.length() > 0 ? creator : "-1");
            break;
        }
        stream << Qt::endl;
    }
    file.close();
    return true;
}

bool Administration::loadUsers() {
    User* user;
    int mediaId;
    int userId;
    QString name, surname, email;
    QDate birthdate;
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
        email = values[3];
        birthdate = QDate::fromString(values[4], "dd.MM.yyyy");

        user = new User(name, surname, email, birthdate);

        QStringList lentMediaIds = values[5].split(" ");
        for (const QString& str : lentMediaIds) {
            if (str.length() > 0) {
                mediaId = str.toInt();
                if (mediaId != -1) {
                    user->lendMediaByUser(mediaId);
                }
            }
        }
        userList.insert(userId, user);
        nextUserId = userId + 1;
    }
    return true;
}

bool Administration::loadMedia() {
    int mediaId, userId;
    QString title;
    MediaType type;
    QString creator;
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
        userId = values[2].toInt();
        type = (MediaType)values[3].toInt();
        creator = (values[4] == "-1" ? "" : values[4]);
        Media* media;

        switch (type) {
        case BOOK:
            media = new Book(title, creator);
            break;
        case CD:
            media = new Cd(title, creator);
            break;
        case DVD:
            media = new Dvd(title, creator);
            break;   
        }

        media->setUserId(userId);
        mediaList.insert(mediaId, media);
        nextMediaId = mediaId + 1;
    }
    return true;
}

void Administration::clearUsers() {
    qDeleteAll(userList.begin(), userList.end());
    userList.clear();
}

void Administration::clearMedia() {
    qDeleteAll(mediaList);
    mediaList.clear();
}

bool Administration::clearFiles() {
    if (!QFile::remove(userFilePath)) {
        return false;
    }
    if (!QFile::remove(mediaFilePath)) {
        return false;
    }
    qDebug() << "Deleted files containing data!";
    return true;
}

int Administration::returnAllMediaByUser(int userId) {
    if (!userList.contains(userId)) {
        qDebug() << "Error user with Id: " << userId << "doesn't exist";
        return -1;
    }

    User* user = userList.value(userId);
    int counter = user->getLentMedia().length();

    for (int mediaId : user->getLentMedia()) {
        mediaList.value(mediaId)->setUserId(-1);
    }
    user->getLentMedia().clear();
    return counter;
}

int Administration::getItemCountFromFile(QString path) {
    int itemCount = 0;
    if (!QFile::exists(path)) {
        qDebug() << path << " doesn't exist yet";
        return -1;
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open " << path;
        return -2;
    }

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        stream.readLine();
        itemCount++;
    }

    return itemCount;
}

int Administration::getMediaCountFromFile() {
    return getItemCountFromFile(mediaFilePath);
}

int Administration::getUserCountFromFile() {
    return getItemCountFromFile(userFilePath);
}


