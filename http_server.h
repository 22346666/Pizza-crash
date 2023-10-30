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

class Http_server : public QObject
{
    Q_OBJECT
public:
    explicit Http_server(QObject *parent = nullptr);
    void listen();
    void listen_ssl();
    QHostAddress get_ip() const;
    void set_ip(const QHostAddress &newIp);
    QByteArray read_HTML(const QString& path) const;
private:
    static inline QString host(const QHttpServerRequest &request);

    QHttpServer http_server;
    QHostAddress ip;
    const quint16 port = 80;
    const quint16 ssl_port = 443;
    QString html_folder_path = "D:/A tut y menua uchoba/4 Курс/Internet/html/labs/";
signals:

};

#endif // HTTP_SERVER_H
