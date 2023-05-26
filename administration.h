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
    QMap<int,User*> userList;
    QMap<int,Media*> mediaList;
    int nextUserId = 0;
    int nextMediaId = 0;
    bool checkDataForCorruption();
public:
    Administration() {};
    void registerUser(User* user);
    void addMedia(Media* media);
    bool deleteUser(int userId);
    bool deleteMedia(int mediaId);
    Media* getMedia(int mediaId);
    User* getUser(int userId);
    bool lendMedia(int mediaId, int userId);
    bool handBackMedia(int mediaId, int userId);
    bool saveUsers();
    bool saveMedia();
    bool loadUsers();
    bool loadMedia();
    void clearUsers();
    void clearMedia();
    bool clearFiles();
};

#endif // USERADMINISTRATION_H
