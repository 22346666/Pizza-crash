#include "http_server.h"

Http_server::Http_server(QObject *parent)
    : QObject{parent}
{
    ip.setAddress("192.168.1.105");

    http_server.route("/", [this](const QHttpServerRequest &request) {
        return get_page(request, "home.html");
    });

    http_server.route("/laba_3", [this](const QHttpServerRequest &request) {
        return get_page(request, "laba_3.html");
    });

    http_server.route("/laba_5", [this](const QHttpServerRequest &request) {
        return get_page(request, "laba_5.html");
    });

    http_server.route("/laba_5/sign_in", [this](const QHttpServerRequest &request) {
        return get_page(request, "laba_5_si.html");
    });

    http_server.route("/laba_5/sign_up", [this](const QHttpServerRequest &request) {
        return get_page(request, "laba_5_su.html");
    });

    http_server.route("/laba_5/page", [this](const QHttpServerRequest &request) {
        return get_page(request, "laba_5_page.html");
    });


    http_server.route("/laba_5/post/user", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(user.create_in_db(db.db)) {
            return redirect(request, "/laba_5/page");
        }
        return redirect(request, "/laba_5/sign_in");
    });

    http_server.route("/laba_5/get/user", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(!user.check_exist(db.db)) {
            return redirect(request, "/laba_5/sign_up");
        }
        if(user.password_check(db.db)) {
            return redirect(request, "/laba_5/page");
        }
        return redirect(request, "/laba_5/sign_in");
    });


    http_server.afterRequest([](QHttpServerResponse &&responce) {
        responce.setHeader("Server", "Pizza cite server");
        return std::move(responce);
    });
}

Http_server::~Http_server()
{
    http_server.deleteLater();
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
    const auto sslCertificateChain = QSslCertificate::fromPath(QStringLiteral(":/assets/fullchain.pem"));
    if (sslCertificateChain.empty()) {
        qWarning() << "Couldn't retrieve SSL certificate from file.";
        return;
    }
    QFile private_key_file(QStringLiteral(":/assets/privkey.pem"));
    if (!private_key_file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file for reading:" << private_key_file.errorString();
        return;
    }
    http_server.sslSetup(sslCertificateChain.front(), QSslKey(&private_key_file, QSsl::KeyAlgorithm::Ec));
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

QHttpServerResponse Http_server::get_page(const QHttpServerRequest &request, const QString &page_name)
{
    QHttpServerResponse response(read_file(files_folder_path+page_name), QHttpServerResponse::StatusCode::Ok);
    return std::move(response);
}

QHttpServerResponse Http_server::redirect(const QHttpServerRequest &request, const QString &endpoint)
{
    QHttpServerResponse response(QHttpServerResponse::StatusCode::Found);
    response.addHeader("Location", (domain_name+endpoint).toLatin1());
    return std::move(response);
}

QHostAddress Http_server::get_ip() const
{
    return ip;
}

void Http_server::set_ip(const QHostAddress &newIp)
{
    ip = newIp;
}

QByteArray Http_server::read_file(const QString &path) const
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
