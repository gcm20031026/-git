#include "qmysqlite.h"
#include<QDebug>

QMySqlite* QMySqlite::mySqlite = nullptr;
QMySqlite *QMySqlite::getInstance(QString fileName)
{
    //是否被创建过
    if(nullptr!=QMySqlite::mySqlite)
    {
        return QMySqlite::mySqlite;
    }else{
        //调用
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

void QMySqlite::closeDB()
{
    sqlite3_close(this->myDB);

}

QMySqlite::QMySqlite()
{

}

QMySqlite::QMySqlite(QString fileName)
{
    //打开数据库
    int res = sqlite3_open(fileName.toUtf8(),&myDB);
    if(res!=SQLITE_OK)
    {
        qDebug() << "open error!";
    }else qDebug() << "open OK!";


}
