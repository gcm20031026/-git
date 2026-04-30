#include "userstorage.h"

#include <QApplication>
#include <QDir>

namespace
{
UserRecord g_currentUser;
}

void UserStorage::setCurrentUser(const UserRecord &user)
{
    g_currentUser = user;
    ensureCurrentUserDirs();
}

UserRecord UserStorage::currentUser()
{
    return g_currentUser;
}

QString UserStorage::safeUserName()
{
    QString name = g_currentUser.username.trimmed();
    if (name.isEmpty())
    {
        name = QStringLiteral("guest");
    }

    name.replace("/", "_");
    name.replace("\\", "_");
    name.replace(":", "_");
    name.replace("*", "_");
    name.replace("?", "_");
    name.replace("\"", "_");
    name.replace("<", "_");
    name.replace(">", "_");
    name.replace("|", "_");
    return name;
}

QString UserStorage::userRootDir()
{
    return QDir(QApplication::applicationDirPath()).filePath(QString("users/%1").arg(safeUserName()));
}

QString UserStorage::videoDir()
{
    return QDir(userRootDir()).filePath("videos");
}

QString UserStorage::pictureDir()
{
    return QDir(userRootDir()).filePath("pictures");
}

QString UserStorage::recordDbPath()
{
    return QDir(userRootDir()).filePath("monitor_records.db");
}

bool UserStorage::ensureCurrentUserDirs()
{
    QDir dir;
    bool ok = dir.mkpath(videoDir());
    ok = dir.mkpath(pictureDir()) && ok;
    return ok;
}
