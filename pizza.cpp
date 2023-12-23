#include "pizza.h"

QJsonObject Pizza::get_json(const QSqlDatabase &db)
{
    QSqlQuery query(db);
    query.exec(QString("SELECT PizzaName, PizzaCost, PizzaImage from pizza"));
    QJsonArray pizza_list;
    while(query.next()){
        QJsonObject pizza;
        pizza.insert("Name",query.value(0).toString());
        pizza.insert("Cost",query.value(1).toString());
        pizza.insert("Image link",query.value(2).toString());
        pizza_list.append(pizza);
    }
    QJsonObject json;
    json.insert("Pizzas", pizza_list);
    return json;
}
