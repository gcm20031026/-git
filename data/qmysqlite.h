#ifndef QMYSQLITE_H
#define QMYSQLITE_H

#include <QString>
#include <QVector>
#include <QMap>
#include "sqlite3.h"

struct VideoRecord
{
    int id = 0;
    QString cameraName;
    QString filePath;
    QString saveTime;
    QString format;
};

struct UserRecord
{
    int id = 0;
    QString username;
    QString displayName;
    QString createdAt;
    QString lastLoginAt;
};

struct AlarmDbRecord
{
    int id = 0;
    QString time;
    QString level;
    QString source;
    QString type;
    QString status;
};

class QMySqlite
{
public:
    static QMySqlite* getInstance(QString fileName);

    int doSql(QString strsql, char** &presult, int &nrow, int &ncolumn);

    bool initVideoRecordTable();
    bool insertVideoRecord(const QString &cameraName, const QString &filePath, const QString &saveTime, const QString &format);
    QVector<VideoRecord> queryVideoRecords(const QString &keyword = QString(), const QString &format = QString());

    bool initAlarmRecordTable();
    bool insertAlarmRecord(const QString &level, const QString &source, const QString &type, const QString &status = QString());
    QVector<AlarmDbRecord> queryAlarmRecords(const QString &status = QString());
    bool updateAlarmStatus(int id, const QString &status);
    bool pendingAlarmExists(const QString &source, const QString &type);

    bool initUserTable();
    bool registerUser(const QString &username, const QString &password, QString *errorMessage = nullptr);
    bool validateUser(const QString &username, const QString &password, UserRecord *user = nullptr, QString *errorMessage = nullptr);
    bool userExists(const QString &username);

    void closeDB();

private:
    QMySqlite();
    explicit QMySqlite(QString fileName);

    static QMap<QString, QMySqlite*> sqliteInstances;
    QString fileName;
    sqlite3 *myDB = nullptr;
};

#endif // QMYSQLITE_H
