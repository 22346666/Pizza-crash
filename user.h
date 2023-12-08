#ifndef USER_H
#define USER_H
#include "database.h"
#include <QString>
#include <QList>

class User
{
public:
    User(const QList<QPair<QString, QString>>& form);
    bool create_in_db(const QSqlDatabase& db);
    bool check_exist(const QSqlDatabase& db);
    bool password_check(const QSqlDatabase& db);
    void get();
    void print();
private:
    QString name;
    QString surname;
    QString email;
    QString password;
    QString gender;
    QString comp;
};

#endif // USER_H
