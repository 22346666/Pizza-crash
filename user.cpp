#include "user.h"

User::User(const QSqlDatabase& db, const QString email)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT * FROM users WHERE UserEmail = '%1'").arg(email));
    query.next();
    name=query.value(1).toString();
    surname=query.value(2).toString();
    this->email=query.value(3).toString();
    password=query.value(4).toString();
    qInfo() << name << surname << email << password;
}

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

bool User::create(const QSqlDatabase &db)
{
    if(email=="") {
        return false;
    }
    if(!check_exist(db)) {
        QSqlQuery query(db);
        query.exec(QString("INSERT INTO users (UserName, UserSurname, UserEmail, UserPassword) values ('%1', '%2', '%3', '%4')").arg(name, surname, email, password));
        qInfo() << query.lastError();
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

QJsonObject User::get()
{
    QJsonObject user;
    user.insert("Name", QJsonValue(name));
    user.insert("Surname", QJsonValue(surname));
    user.insert("Email", QJsonValue(email));
    user.insert("Password", QJsonValue(password));
    return user;
}

QString User::get_name() const
{
    return name;
}

QString User::get_email() const
{
    return email;
}

QString User::get_surname() const
{
    return surname;
}

QString User::get_password() const
{
    return password;
}

QString User::get_gender() const
{
    return gender;
}

QString User::get_comp() const
{
    return comp;
}


