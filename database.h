#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QSqlRecord>


class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    void test_query();
    ~Database();
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
signals:

};

#endif // DATABASE_H
