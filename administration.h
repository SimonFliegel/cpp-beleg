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
    std::vector<std::pair<int,Media*>> mediaListToStdVector();
    std::vector<std::pair<int,User*>> userListToStdVector();
    void fillMediaListByVector(std::vector<std::pair<int,Media*>>& mediaVec);
    void fillUserListByVector(std::vector<std::pair<int,User*>> userVec);
    bool compareMediaByCreator(Media* m1, Media* m2);

public:
    Administration() {};
    void registerUser(User* user);
    void addMedia(Media* media);
    void updateMediaType(int mediaId, MediaType newType);
    bool deleteUser(int userId);
    bool deleteMedia(int mediaId);
    Media* getMedia(int mediaId);
    QMap<int,Media*>& getMediaList();
    void sortMediaListByType();
    void sortMediaListByTitle();
    void sortMediaListByCreator();
    User* getUser(int userId);
    void getUserList();
    void sortUserBySurname();
    void sortUserByName();
    int getMediaCount();
    int getUserCount();
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
