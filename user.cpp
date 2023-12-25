#include "user.h"

User::User(const QSqlDatabase& db, const QString email)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT * FROM users WHERE UserEmail = '%1'").arg(email));
    if(!query.next()){
        return;
    }
    name=query.value(1).toString();
    surname=query.value(2).toString();
    this->email=query.value(3).toString();
    password=query.value(4).toString();
    qInfo() << name << surname << this->email << password;
}

User::User(const QList<QPair<QString, QString>> &form)
{
    for(auto &i : form) {
        if(i.first=="username") {
            name = decode_plus(i.second);
        } else if (i.first=="surname") {
            surname = decode_plus(i.second);
        } else if (i.first=="email") {
            email = decode_plus(i.second);
        } else if (i.first=="password") {
            password = decode_plus(i.second);
        } else if (i.first=="gender") {
            gender = decode_plus(i.second);
        } else if (i.first=="comp") {
            comp = decode_plus(i.second);
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
        qInfo() << query.lastError().text();
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

QJsonObject User::get_json()
{
    QJsonObject user;
    user.insert("Name", QJsonValue(name));
    user.insert("Surname", QJsonValue(surname));
    user.insert("Email", QJsonValue(email));
    user.insert("Password", QJsonValue(password));
    return user;
}

void User::update(const QSqlDatabase &db, const QJsonObject &json)
{
    QSqlQuery query(db);
    query.exec(QString("UPDATE users SET UserPassword = '%1' WHERE UserEmail = '%2';").arg(json.value("Password").toString(), json.value("Email").toString()));
}

void User::create_order(const QSqlDatabase &db, const QJsonObject &json)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT UserID FROM users WHERE UserEmail = '%1'").arg(json.value("Email").toString()));
    query.next();
    QString id = query.value(0).toString();
    qDebug() << id;
    query.exec(QString("INSERT INTO orders (OrderDate, OrderAddress, OrderSumm, UserID) values ('%1', '%2', '%3', '%4')").arg(json.value("Date").toString(), json.value("Address").toString(), json.value("Summ").toString(), id));
    qDebug() << query.lastError().text();
}

QJsonObject User::get_order_json(const QSqlDatabase& db, const QString &email)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT UserID FROM users WHERE UserEmail = '%1'").arg(email));
    query.next();
    QString id;
    if(query.isValid()) {
        id = query.value(0).toString();
    }
    QJsonObject json;
    if(!id.isEmpty()) {
        query.exec(QString("SELECT OrderDate, OrderAddress, OrderSumm FROM orders WHERE UserID = '%1' ORDER BY OrderID desc").arg(id));
        QJsonArray orders;
        for(size_t i {0}; query.next() && i < 15; ++i) {
            QJsonObject order;
            order.insert("Date", query.value(0).toString());
            order.insert("Address", query.value(1).toString());
            order.insert("Summ", query.value(2).toString());
            orders.push_back(order);
        }
        json.insert("Orders", orders);
    } else {
        json.insert("Orders", id);
    }
    return std::move(json);
}

QString User::decode_plus(QString value)
{
    for(size_t i {0}; i < value.size(); ++i){
        if(value[i]=='+') {
            value.replace(i, 1, ' ');
        }
    }
    qDebug() << value;
    return std::move(value);
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


