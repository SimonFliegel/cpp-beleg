#ifndef USERADMINISTRATION_H
#define USERADMINISTRATION_H

#include <QMap>
#include <QList>
#include "user.h"
#include "media.h"

class Administration
{
private:
    QMap<int,User> userList;
    QMap<int,Media> mediaList;
    int nextUserId = 0;
    int nextMediaId = 0;
public:
    Administration() {};
    void registerUser(User* user);
    void addMedia(Media* media);
    bool deleteUser(int userId);
    bool deleteMedia(int mediaId);
    bool lendMedia(int mediaId, int userId);
    bool handBackMedia(int mediaId);
    QList<Media> getLendedMedia(int userId);
};

#endif // USERADMINISTRATION_H
