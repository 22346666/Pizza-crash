#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
    qInfo() << "Opening database";
    db.setHostName("AN/MSSQLSERVER01");
    db.setDatabaseName("SQL server");
    db.setUserName("AN\21");
    db.setPassword("11");
    if(!db.open()) {
        qInfo() << db.lastError().text();
        return;
    }
    qInfo() << "Database is working";



}

void Database::test_query()
{
    QSqlQuery query(db);
    if(!query.exec("SELECT * FROM users")) {
        qWarning() << query.lastError();
    }else {
        while(query.next()) {
            quint64 key = query.value(0).toInt();
            QString name =  query.value(1).toString();
            QString password =  query.value(2).toString();
            qInfo() << key << name << password;
        }
    }
}

Database::~Database()
{
    db.close();
    qInfo() << "Closing connection with DB";
}


