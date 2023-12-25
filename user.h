#ifndef USER_H
#define USER_H
#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class User
{
public:
    User(const QSqlDatabase& db, const QString email);
    User(const QList<QPair<QString, QString>>& form);
    bool create(const QSqlDatabase& db);
    bool check_exist(const QSqlDatabase& db);
    bool password_check(const QSqlDatabase& db);
    QJsonObject get_json();
    static void update(const QSqlDatabase& db, const QJsonObject& json);




    static void create_order(const QSqlDatabase& db, const QJsonObject& json);
    static QJsonObject get_order_json(const QSqlDatabase& db, const QString& email);




    QString decode_plus(QString value);

    QString get_name() const;

    QString get_email() const;

    QString get_surname() const;

    QString get_password() const;

    QString get_gender() const;

    QString get_comp() const;

private:
    QString name;
    QString surname;
    QString email;
    QString password;
    QString gender;
    QString comp;
};

#endif // USER_H
