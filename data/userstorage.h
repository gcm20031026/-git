#ifndef USERSTORAGE_H
#define USERSTORAGE_H

#include <QString>
#include "data/qmysqlite.h"

class UserStorage
{
public:
    static void setCurrentUser(const UserRecord &user);
    static UserRecord currentUser();

    static QString userRootDir();
    static QString videoDir();
    static QString pictureDir();
    static QString recordDbPath();
    static bool ensureCurrentUserDirs();

private:
    static QString safeUserName();
};

#endif // USERSTORAGE_H
