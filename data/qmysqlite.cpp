#include "qmysqlite.h"
#include<QDebug>
#include<QStringList>

QMySqlite* QMySqlite::mySqlite = nullptr;
QMySqlite *QMySqlite::getInstance(QString fileName)
{
    //鏄惁琚垱寤鸿繃
    if(nullptr!=QMySqlite::mySqlite)
    {
        return QMySqlite::mySqlite;
    }else{
        //璋冪敤
        QMySqlite::mySqlite = new QMySqlite(fileName);
        return QMySqlite::mySqlite;
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
