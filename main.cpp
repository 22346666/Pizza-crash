#include <QHttpServer>
#include <QHttpServerResponse>

#if QT_CONFIG(ssl)
#  include <QSslCertificate>
#  include <QSslKey>
#endif

#include <QCoreApplication>
#include <QFile>
#include <QString>
#include <QHostInfo>

using namespace Qt::StringLiterals;

static inline QString host(const QHttpServerRequest &request)
{
    return QString::fromLatin1(request.value("Host"));
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QHttpServer httpServer;
    httpServer.route("/", []() {
        QHttpServerResponse response("<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title></title></head><body><h1>Hellow workd</h1></body></html>",QHttpServerResponse::StatusCode::Ok);
        return std::move(response);
    });

    httpServer.route("/", []() {
        QHttpServerResponse response("<!DOCTYPE html><html><head><meta charset=\"utf-8\"><title></title></head><body><h1>Hellow workd</h1></body></html>",QHttpServerResponse::StatusCode::Ok);
        return std::move(response);
    });

    //! [Using afterRequest()]
    httpServer.afterRequest([](QHttpServerResponse &&resp) {
        resp.setHeader("Server", "Pizza cite server");
        return std::move(resp);
    });
    //! [Using afterRequest()]
    QHostAddress ip;
    ip.setAddress("192.168.1.105");
    quint16 port = 80;
    quint16 ssl_port = 443;
    httpServer.listen(ip, port);
    if (!port) {
        qWarning() << QCoreApplication::translate("QHttpServerExample",
                                                  "Server failed to listen on a port.");
        return -1;
    }

#if QT_CONFIG(ssl)
    //! [HTTPS Configuration example]
    //!
    const auto sslCertificateChain = QSslCertificate::fromPath(QStringLiteral(":/assets/server_certificate.crt"));
    if (sslCertificateChain.empty()) {
        qWarning() << QCoreApplication::translate("QHttpServerExample",
                                                  "Couldn't retrieve SSL certificate from file.");
        return -1;
    }
    QFile privateKeyFile(QStringLiteral(":/assets/server.key"));
    if (!privateKeyFile.open(QIODevice::ReadOnly)) {
        qWarning() << QCoreApplication::translate("QHttpServerExample",
                                                  "Couldn't open file for reading: %1")
                          .arg(privateKeyFile.errorString());
        return -1;
    }
    httpServer.sslSetup(sslCertificateChain.front(), QSslKey(&privateKeyFile, QSsl::Rsa));
    privateKeyFile.close();

    httpServer.listen(ip, ssl_port);
    if (!ssl_port) {
        qWarning() << QCoreApplication::translate("QHttpServerExample",
                                                  "Server failed to listen on a port.");
        return -1;
    }
    // [HTTPS Configuration example]

    qInfo().noquote()
        << QCoreApplication::translate("QHttpServerExample",
                                       "Running with SSL on http://%1:%2/ and "
                                       "https://%3:%4/ (Press CTRL+C to quit)")
               .arg(ip.toString()).arg(port).arg(ip.toString()).arg(ssl_port);
#else
    qInfo().noquote()
        << QCoreApplication::translate("QHttpServerExample",
                                       "Running on http://127.0.0.1:%1/"
                                       "(Press CTRL+C to quit)").arg(port);
#endif
    return app.exec();
}
