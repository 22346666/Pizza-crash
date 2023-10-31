#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{

}

void Database::connect_db()
{
    qInfo() << "Opening database";
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("AN/MSSQLSERVER01");
    db.setDatabaseName("SQL server");
    db.setUserName("AN\21");
    db.setPassword("11");
    if(!db.open()) {
        qInfo() << db.lastError().text();
        return;
    }
    qInfo() << "Database is working";
    db.close();
}


