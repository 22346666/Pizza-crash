#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <QHttpServer>
#include <QHttpServerResponse>

#if QT_CONFIG(ssl)
#  include <QSslCertificate>
#  include <QSslKey>
#endif

#include <QObject>
#include <QFile>
#include <QString>
#include <QHostInfo>
#include "database.h"
#include "user.h"

class Http_server : public QObject
{
    Q_OBJECT
public:
    explicit Http_server(QObject *parent = nullptr);
    ~Http_server();

    void listen();
    void listen_ssl();
    QHostAddress get_ip() const;
    void set_ip(const QHostAddress &newIp);
    QByteArray read_file(const QString& path) const;

private:
    static inline QString host(const QHttpServerRequest &request);
    QHttpServerResponse get_page(const QHttpServerRequest &request, const QString &page_name);
    QHttpServerResponse redirect(const QHttpServerRequest &request, const QString &endpoint);

    QHttpServer http_server;
    Database db;
    QHostAddress ip;
    QString domain_name = "https://pizzacrash.servebeer.com";
    const quint16 port = 80;
    const quint16 ssl_port = 443;
    QString files_folder_path = "Files/";
signals:

};

#endif // HTTP_SERVER_H
