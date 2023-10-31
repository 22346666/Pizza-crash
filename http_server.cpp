#include "http_server.h"

Http_server::Http_server(QObject *parent)
    : QObject{parent}
{
    ip.setAddress("192.168.1.105");
    http_server.route("/", [this]() {
        QHttpServerResponse response(read_HTML(html_folder_path+"laba_3.html"), QHttpServerResponse::StatusCode::Ok);
        return std::move(response);
    });


    http_server.afterRequest([](QHttpServerResponse &&responce) {
        responce.setHeader("Server", "Pizza cite server");
        return std::move(responce);
    });
}

void Http_server::listen()
{
    if(http_server.listen(ip, port) == 0) {
        qWarning() << "Couldn't managed to listen without SSL to this ip adress:" << ip << "and port:" << port;
        return;
    }
    qInfo().noquote().nospace() << "Running on http://" << ip.toString() << ":" << port << "/";
}

void Http_server::listen_ssl()
{
#if QT_CONFIG(ssl)
    const auto sslCertificateChain = QSslCertificate::fromPath(QStringLiteral(":/assets/server_certificate.crt"));
    if (sslCertificateChain.empty()) {
        qWarning() << "Couldn't retrieve SSL certificate from file.";
        return;
    }
    QFile private_key_file(QStringLiteral(":/assets/server.key"));
    if (!private_key_file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file for reading:" << private_key_file.errorString();
        return;
    }
    http_server.sslSetup(sslCertificateChain.front(), QSslKey(&private_key_file, QSsl::Rsa));
    private_key_file.close();

    http_server.listen(ip, ssl_port);
    if (!ssl_port) {
        qWarning() << "Server failed to listen on a port";
        return;
    }
    qInfo().noquote().nospace() << "Running with SSL on https://" << ip.toString() << ":" << ssl_port << "/";
#else
    qWarning() << "Couldn't managed to run SSL correctly";
#endif
}

QString Http_server::host(const QHttpServerRequest &request)
{
    return QString::fromLatin1(request.value("Host"));
}

QHostAddress Http_server::get_ip() const
{
    return ip;
}

void Http_server::set_ip(const QHostAddress &newIp)
{
    ip = newIp;
}

QByteArray Http_server::read_HTML(const QString &path) const
{
    QFile file(path);
    QByteArray html;
    if(file.open(QIODevice::ReadOnly)) {
        html+=file.readAll();
    }else {
        qWarning() << file.errorString();
    }
    return html;
}
