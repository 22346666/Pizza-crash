#include "cookie.h"

Cookie::Cookie(const QByteArray name, const QByteArray value, const QByteArray domain)
{
    cookie.setName(name);
    cookie.setValue(value);
    cookie.setSecure(true);
    cookie.setDomain(domain);
    cookie.setPath("/");
}

QByteArray Cookie::get_raw_cookie()
{
    return cookie.toRawForm();
}
