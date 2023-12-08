#ifndef COOKIE_H
#define COOKIE_H
#include <QNetworkCookie>

class Cookie
{
public:
    Cookie(const QByteArray name = QByteArray(), const QByteArray value = QByteArray(), const QByteArray domain = QByteArray());
    QByteArray get_raw_cookie();
private:
    QNetworkCookie cookie;
};

#endif // COOKIE_H
