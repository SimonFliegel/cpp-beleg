#ifndef USERADMINISTRATION_H
#define USERADMINISTRATION_H

#include <QMap>
#include <QList>
#include <QString>
#include "user.h"
#include "media.h"

class Administration
{
private:
    inline static const QString userFilePath = "user.csv";
    inline static const QString mediaFilePath = "media.csv";
    QMap<int,Media*> mediaList;
    QMap<int,User*> userList;
    int nextUserId = 0;
    int nextMediaId = 0;
    int returnAllMediaByUser(int userId);
    int getItemCountFromFile(QString path);
    QMap<int,Media*> searchMediaByType(MediaType type);
    QMap<int,Media*> searchMediaByTitle(QString title);
    QMap<int,Media*> searchMediaByCreator(QString creator);
    QMap<int,User*> searchUsersBySurname(QString surname);
    QMap<int,User*> searchUsersByName(QString name);

public:
    Administration() {};
    void registerUser(User* user);
    void addMedia(Media* media);
    void updateMediaType(int mediaId, MediaType newType);
    bool deleteUser(int userId);
    bool deleteMedia(int mediaId);
    Media* getMedia(int mediaId);
    QMap<int,Media*>& getMediaList();
    User* getUser(int userId);
    QMap<int,User*>& getUserList();
    int getMediaCount();
    int getUserCount();
    QMap<int,Media*> searchMedia(MediaType type, QString title = "", QString creator = "");
    QMap<int,User*> searchUsers(QString surname = "", QString name = "");
    bool lendMedia(int mediaId, int userId);
    bool returnMedia(int mediaId, int userId);
    bool saveUsers();
    bool saveMedia();
    bool loadUsers();
    bool loadMedia();
    void clearUsers();
    void clearMedia();
    bool clearFiles();
    int getMediaCountFromFile();
    int getUserCountFromFile();
};

#endif // USERADMINISTRATION_H
