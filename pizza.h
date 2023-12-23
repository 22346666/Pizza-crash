#ifndef PIZZA_H
#define PIZZA_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class Pizza
{
public:
    QJsonObject get_json(const QSqlDatabase& db);
};

#endif // PIZZA_H
