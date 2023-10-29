#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <QObject>

class Http_server : public QObject
{
    Q_OBJECT
public:
    explicit Http_server(QObject *parent = nullptr);

signals:

};

#endif // HTTP_SERVER_H
