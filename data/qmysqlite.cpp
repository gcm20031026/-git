#include "qmysqlite.h"
#include <QCryptographicHash>
#include <QDateTime>
#include<QDebug>
#include <QFileInfo>
#include<QStringList>
#include <QUuid>

namespace
{
QString createSalt()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

QString passwordHash(const QString &password, const QString &salt)
{
    QByteArray data = salt.toUtf8();
    data.append(':');
    data.append(password.toUtf8());
    return QString::fromLatin1(QCryptographicHash::hash(data, QCryptographicHash::Sha256).toHex());
}

QString nowText()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}
}

QMap<QString, QMySqlite*> QMySqlite::sqliteInstances;
QMySqlite *QMySqlite::getInstance(QString fileName)
{
    QString key = QFileInfo(fileName).absoluteFilePath();
    if(QMySqlite::sqliteInstances.contains(key))
    {
        return QMySqlite::sqliteInstances.value(key);
    }
    else
    {
        QMySqlite *sqlite = new QMySqlite(key);
        QMySqlite::sqliteInstances.insert(key, sqlite);
        return sqlite;
    }
}

int QMySqlite::doSql(QString strsql, char **&presult, int &nrow, int &ncolumn)
{

    char *errmsg = nullptr;

    int res = sqlite3_get_table(myDB, strsql.toUtf8().constData(), &presult, &nrow, &ncolumn, &errmsg);
    if (res == SQLITE_OK) {
        return 0;
    } else {
        return -1;
    }


}

bool QMySqlite::initVideoRecordTable()
{
    const QString sql =
            "create table if not exists video_records ("
            "id integer primary key autoincrement,"
            "camera_name text not null,"
            "file_path text not null unique,"
            "save_time text not null,"
            "format text not null"
            ");";
    char **result = nullptr;
    int row = 0;
    int column = 0;
    int ret = doSql(sql, result, row, column);
    if (result) sqlite3_free_table(result);
    return ret == 0;
}

bool QMySqlite::insertVideoRecord(const QString &cameraName, const QString &filePath, const QString &saveTime, const QString &format)
{
    initVideoRecordTable();

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "insert or replace into video_records(camera_name, file_path, save_time, format) values(?, ?, ?, ?);";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare insert video record failed:" << sqlite3_errmsg(myDB);
        return false;
    }

    sqlite3_bind_text(stmt, 1, cameraName.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, filePath.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, saveTime.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, format.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    if (!ok)
    {
        qDebug() << "insert video record failed:" << sqlite3_errmsg(myDB);
    }
    sqlite3_finalize(stmt);
    return ok;
}

QVector<VideoRecord> QMySqlite::queryVideoRecords(const QString &keyword, const QString &format)
{
    initVideoRecordTable();

    QVector<VideoRecord> records;
    QString sql = "select id, camera_name, file_path, save_time, format from video_records";
    QStringList where;
    if (!keyword.trimmed().isEmpty())
    {
        where << "(camera_name like ? or file_path like ? or save_time like ?)";
    }

    QString trimmedFormat = format.trimmed();
    bool filterByFormat = !trimmedFormat.isEmpty()
            && trimmedFormat != "全部"
            && trimmedFormat != QString::fromUtf8("全部");
    if (filterByFormat)
    {
        where << "format = ?";
    }
    if (!where.isEmpty())
    {
        sql += " where " + where.join(" and ");
    }
    sql += " order by save_time desc, id desc;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(myDB, sql.toUtf8().constData(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare query video records failed:" << sqlite3_errmsg(myDB);
        return records;
    }

    int bindIndex = 1;
    if (!keyword.trimmed().isEmpty())
    {
        QString likeKeyword = "%" + keyword.trimmed() + "%";
        sqlite3_bind_text(stmt, bindIndex++, likeKeyword.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, bindIndex++, likeKeyword.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, bindIndex++, likeKeyword.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    }
    if (filterByFormat)
    {
        sqlite3_bind_text(stmt, bindIndex++, trimmedFormat.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        VideoRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.cameraName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        record.filePath = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        record.saveTime = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        record.format = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        records.push_back(record);
    }

    sqlite3_finalize(stmt);
    return records;
}

bool QMySqlite::initAlarmRecordTable()
{
    const QString sql =
            "create table if not exists alarm_records ("
            "id integer primary key autoincrement,"
            "time text not null,"
            "level text not null,"
            "source text not null,"
            "type text not null,"
            "status text not null"
            ");";
    char **result = nullptr;
    int row = 0;
    int column = 0;
    int ret = doSql(sql, result, row, column);
    if (result) sqlite3_free_table(result);
    return ret == 0;
}

bool QMySqlite::insertAlarmRecord(const QString &level, const QString &source, const QString &type, const QString &status)
{
    initAlarmRecordTable();

    const QString alarmStatus = status.trimmed().isEmpty() ? QString::fromUtf8(u8"待处理") : status.trimmed();
    if (alarmStatus == QString::fromUtf8(u8"待处理") && pendingAlarmExists(source, type))
    {
        return true;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "insert into alarm_records(time, level, source, type, status) values(?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare insert alarm record failed:" << sqlite3_errmsg(myDB);
        return false;
    }

    const QString alarmTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    sqlite3_bind_text(stmt, 1, alarmTime.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, level.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, source.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, type.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, alarmStatus.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    if (!ok)
    {
        qDebug() << "insert alarm record failed:" << sqlite3_errmsg(myDB);
    }
    sqlite3_finalize(stmt);
    return ok;
}

QVector<AlarmDbRecord> QMySqlite::queryAlarmRecords(const QString &status)
{
    initAlarmRecordTable();

    QVector<AlarmDbRecord> records;
    const bool filterStatus = !status.trimmed().isEmpty() && status != QString::fromUtf8(u8"全部告警");
    QString sql = "select id, time, level, source, type, status from alarm_records";
    if (filterStatus)
    {
        sql += " where status = ?";
    }
    sql += " order by time desc, id desc;";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(myDB, sql.toUtf8().constData(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare query alarm records failed:" << sqlite3_errmsg(myDB);
        return records;
    }
    if (filterStatus)
    {
        sqlite3_bind_text(stmt, 1, status.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AlarmDbRecord record;
        record.id = sqlite3_column_int(stmt, 0);
        record.time = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        record.level = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        record.source = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        record.type = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        record.status = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
        records.push_back(record);
    }

    sqlite3_finalize(stmt);
    return records;
}

bool QMySqlite::updateAlarmStatus(int id, const QString &status)
{
    initAlarmRecordTable();

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "update alarm_records set status = ? where id = ?;";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare update alarm status failed:" << sqlite3_errmsg(myDB);
        return false;
    }

    sqlite3_bind_text(stmt, 1, status.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, id);
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

bool QMySqlite::pendingAlarmExists(const QString &source, const QString &type)
{
    initAlarmRecordTable();

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "select 1 from alarm_records where source = ? and type = ? and status = ? limit 1;";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare pending alarm exists failed:" << sqlite3_errmsg(myDB);
        return false;
    }

    sqlite3_bind_text(stmt, 1, source.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, type.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    const QString pendingStatus = QString::fromUtf8(u8"待处理");
    sqlite3_bind_text(stmt, 3, pendingStatus.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    bool exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return exists;
}

bool QMySqlite::initUserTable()
{
    const QString sql =
            "create table if not exists users ("
            "id integer primary key autoincrement,"
            "username text not null unique,"
            "password_hash text not null,"
            "salt text not null,"
            "display_name text not null,"
            "created_at text not null,"
            "last_login_at text"
            ");";
    char **result = nullptr;
    int row = 0;
    int column = 0;
    int ret = doSql(sql, result, row, column);
    if (result) sqlite3_free_table(result);
    return ret == 0;
}

bool QMySqlite::userExists(const QString &username)
{
    initUserTable();

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "select 1 from users where username = ? limit 1;";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare user exists failed:" << sqlite3_errmsg(myDB);
        return false;
    }

    const QString trimmedUsername = username.trimmed();
    sqlite3_bind_text(stmt, 1, trimmedUsername.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    bool exists = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);
    return exists;
}

bool QMySqlite::registerUser(const QString &username, const QString &password, QString *errorMessage)
{
    initUserTable();

    const QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty())
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"请输入用户名");
        return false;
    }
    if (trimmedUsername.length() < 3)
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"用户名至少 3 个字符");
        return false;
    }
    if (password.length() < 6)
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"密码至少 6 位");
        return false;
    }
    if (userExists(trimmedUsername))
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"用户名已存在");
        return false;
    }

    const QString salt = createSalt();
    const QString hash = passwordHash(password, salt);
    const QString createdAt = nowText();

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "insert into users(username, password_hash, salt, display_name, created_at) values(?, ?, ?, ?, ?);";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare register user failed:" << sqlite3_errmsg(myDB);
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"数据库准备注册失败");
        return false;
    }

    sqlite3_bind_text(stmt, 1, trimmedUsername.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, hash.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, salt.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, trimmedUsername.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, createdAt.toUtf8().constData(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    if (!ok)
    {
        qDebug() << "register user failed:" << sqlite3_errmsg(myDB);
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"注册失败，请稍后重试");
    }
    sqlite3_finalize(stmt);
    return ok;
}

bool QMySqlite::validateUser(const QString &username, const QString &password, UserRecord *user, QString *errorMessage)
{
    initUserTable();

    const QString trimmedUsername = username.trimmed();
    if (trimmedUsername.isEmpty() || password.isEmpty())
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"请输入用户名和密码");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "select id, username, password_hash, salt, display_name, created_at, last_login_at from users where username = ? limit 1;";
    if (sqlite3_prepare_v2(myDB, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        qDebug() << "prepare validate user failed:" << sqlite3_errmsg(myDB);
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"数据库读取用户失败");
        return false;
    }

    sqlite3_bind_text(stmt, 1, trimmedUsername.toUtf8().constData(), -1, SQLITE_TRANSIENT);
    bool ok = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const QString storedHash = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        const QString salt = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        ok = passwordHash(password, salt) == storedHash;
        if (ok && user)
        {
            user->id = sqlite3_column_int(stmt, 0);
            user->username = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            user->displayName = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
            user->createdAt = QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
            const unsigned char *lastLogin = sqlite3_column_text(stmt, 6);
            user->lastLoginAt = lastLogin ? QString::fromUtf8(reinterpret_cast<const char*>(lastLogin)) : QString();
        }
    }
    sqlite3_finalize(stmt);

    if (!ok)
    {
        if (errorMessage) *errorMessage = QString::fromUtf8(u8"用户名或密码错误");
        return false;
    }

    sqlite3_stmt *updateStmt = nullptr;
    const char *updateSql = "update users set last_login_at = ? where username = ?;";
    if (sqlite3_prepare_v2(myDB, updateSql, -1, &updateStmt, nullptr) == SQLITE_OK)
    {
        const QString lastLoginAt = nowText();
        sqlite3_bind_text(updateStmt, 1, lastLoginAt.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(updateStmt, 2, trimmedUsername.toUtf8().constData(), -1, SQLITE_TRANSIENT);
        sqlite3_step(updateStmt);
        sqlite3_finalize(updateStmt);
        if (user) user->lastLoginAt = lastLoginAt;
    }

    return true;
}

void QMySqlite::closeDB()
{
    sqlite3_close(this->myDB);

}

QMySqlite::QMySqlite()
{

}

QMySqlite::QMySqlite(QString fileName)
{
    //鎵撳紑鏁版嵁搴?
    int res = sqlite3_open(fileName.toUtf8(),&myDB);
    if(res!=SQLITE_OK)
    {
        qDebug() << "open error!";
    }else qDebug() << "open OK!";


}
