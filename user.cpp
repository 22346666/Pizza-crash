#include "user.h"

User::User(const QList<QPair<QString, QString>> &form)
{
    for(auto &i : form) {
        if(i.first=="username") {
            name = i.second;
        } else if (i.first=="surname") {
            surname = i.second;
        } else if (i.first=="email") {
            email = i.second;
        } else if (i.first=="password") {
            password = i.second;
        } else if (i.first=="gender") {
            gender = i.second;
        } else if (i.first=="comp") {
            comp = i.second;
        }
    }
}

bool User::create_in_db(const QSqlDatabase &db)
{
    if(email=="") {
        return false;
    }
    if(!check_exist(db)) {
        QSqlQuery query(db);
        query.exec(QString("INSERT INTO users (UserName, UserSurname, UserEmail, UserPassword, UserGender, UserComp) values ('%1', '%2', '%3', '%4', '%5', '%6')").arg(name, surname, email, password, gender, comp));
        return true;
    }
    return false;
}

bool User::check_exist(const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT UserId FROM users WHERE UserEmail = '%1'").arg(email));
    if(query.next()) {
        qInfo() << email << " are already in db";
        return true;
    }
    qInfo() << email << "are not in db";
    return false;
}

bool User::password_check(const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT UserPassword FROM users WHERE UserEmail = '%1'").arg(email));
    if(query.next() && query.value(0).toString()==password) {
        qInfo() << "Password confirmed";
        return true;
    }
    qInfo() << "Password is not confirmed";
    return false;
}


