#ifndef QMYSQLITE_H
#define QMYSQLITE_H

#include<QString>
#include<QVector>
#include"sqlite3.h"

struct VideoRecord
{
    int id = 0;
    QString cameraName;
    QString filePath;
    QString saveTime;
    QString format;
};
class QMySqlite
{
public:
    static QMySqlite* getInstance(QString fileName);
    //执行语句 0成功 -1失败
    int doSql(QString strsql,char** &presult,int &nrow,int &ncolumn);
    bool initVideoRecordTable();
    bool insertVideoRecord(const QString &cameraName, const QString &filePath, const QString &saveTime, const QString &format);
    QVector<VideoRecord> queryVideoRecords(const QString &keyword = QString(), const QString &format = QString());
    void closeDB();
private:
    QMySqlite();
    QMySqlite(QString fileName);
    static QMySqlite* mySqlite;
    //数据库文件名
    QString fileName;
    //数据库句柄
    sqlite3 *myDB;
};

#endif // QMYSQLITE_H
