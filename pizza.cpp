#include "pizza.h"




QJsonObject Pizza::get_json(const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT * FROM users WHERE UserEmail = '%1'"));
    if(!query.next()){
    }
    return QJsonObject();
}
