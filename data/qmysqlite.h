#ifndef QMYSQLITE_H
#define QMYSQLITE_H

#include<QString>
#include"sqlite3.h"
class QMySqlite
{
public:
    static QMySqlite* getInstance(QString fileName);
    //执行语句 0成功 -1失败
    int doSql(QString strsql,char** &presult,int &nrow,int &ncolumn);
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
