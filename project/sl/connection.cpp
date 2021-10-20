#include "connection.h"

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("db01");
    db.setUserName("root");
    db.setPassword("hongchangkai123");
    bool ok = db.open();
    if (ok)
    {
        qDebug() << "连接成功" ;
        return 1;
    }
    else
    {
         qDebug() << "连接失败" ;
         return 0;
    }
}
