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
#include <QDir>
#include <QString>
#include <QHostInfo>
#include "database.h"
#include <QtTest/QTest>
#include <QUrl>


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
    bool has_id_cookie(const QHttpServerRequest& request);

private:
    static inline QString host(const QHttpServerRequest &request);
    QHttpServerResponse send_file(const QHttpServerRequest &request, const QString &page_name, const QString& files_folder_path, const QByteArray type = "text\\html");
    QHttpServerResponse send_json(const QHttpServerRequest &request, const QJsonObject &json);
    QHttpServerResponse redirect(const QHttpServerRequest &request, const QString &endpoint);
    QHttpServerResponse cookie_redirect(const QHttpServerRequest &request, const QString &endpoint, QByteArray email_value);

    QHttpServer http_server;
    Database db;
    QHostAddress ip;
    QString http_domain_name = "https://pizzacrash.servebeer.com";
    QString domain_name = "pizzacrash.servebeer.com";
    const quint16 port = 80;
    const quint16 ssl_port = 443;
    QString html_path = ":/files/html/";
    QString scripts_path = ":/files/js/";
    QString css_path = ":/files/css/";
    QHttpServerRouter* router;

    void route_pages();
    void route_css();
    void route_scripts();
signals:

};

#endif // HTTP_SERVER_H
