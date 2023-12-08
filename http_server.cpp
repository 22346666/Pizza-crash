#include "http_server.h"
#include "cookie.h"
#include "user.h"

Http_server::Http_server(QObject *parent)
    : QObject{parent}
{
    ip.setAddress("192.168.1.105");
    http_server.afterRequest([](QHttpServerResponse &&responce) {
        responce.setHeader("Server", "Pizza cite server");
        return std::move(responce);
    });
    router = http_server.router();
    route_scripts();
    route_css();
    route_pages();
}

void Http_server::route_pages() {
    http_server.route("/", [this](const QHttpServerRequest &request) {
        return send_file(request, "home.html", html_path);
    });

    http_server.route("/laba_3", [this](const QHttpServerRequest &request) {
        return send_file(request, "laba_3.html", html_path);
    });

    http_server.route("/laba_4_2", [this](const QHttpServerRequest &request) {
        return send_file(request, "laba_4_2.html", html_path);
    });

    http_server.route("/laba_5", [this](const QHttpServerRequest &request) {
        return send_file(request, "laba_5.html", html_path);
    });

    http_server.route("/laba_5/sign_in", [this](const QHttpServerRequest &request) {
        return send_file(request, "laba_5_si.html", html_path);
    });

    http_server.route("/laba_5/sign_up", [this](const QHttpServerRequest &request) {
        return send_file(request, "laba_5_su.html", html_path);
    });

    http_server.route("/laba_5/page", [this](const QHttpServerRequest &request) {
        if(has_id_cookie(request)) {
            return send_file(request, "laba_4.html", html_path);
        }
        return QHttpServerResponse(QHttpServerResponse::StatusCode::Unauthorized);
    });

    http_server.route("/laba_5/post/user/sign_up", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(user.create(db.db)) {
            Cookie cookie("email", user.get_email().toLatin1(), http_domain_name.toLatin1());
            QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
            response.addHeader("Set-Cookie", cookie.get_raw_cookie());
            return cookie_redirect(request, "/laba_5/page", user.get_email().toLatin1());
        }
        return redirect(request, "/laba_5/sign_in");
    });

    http_server.route("/laba_5/post/user/sign_in", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(!user.check_exist(db.db)) {
            return redirect(request, "/laba_5/sign_up");
        }
        if(user.password_check(db.db)) {

            return cookie_redirect(request, "/laba_5/page", user.get_email().toLatin1());
        }
        return redirect(request, "/laba_5/sign_in");
    });

    http_server.route("/laba_5/get/user", [this](const QHttpServerRequest &request) {
        User user(db.db, "s@gmail.com");
        return send_json(request, user.get());
    });


}

void Http_server::route_scripts() {

}

void Http_server::route_css() {
    QString endpoint = "/styles/";
    QByteArray type = "text/css";

    http_server.route(endpoint+"laba_4", [this, type](const QHttpServerRequest &request) {
        return send_file(request, "laba_4.css", css_path, type);
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

QHttpServerResponse Http_server::send_file(const QHttpServerRequest &request, const QString &page_name, const QString &files_folder_path, const QByteArray type)
{
    QHttpServerResponse response(read_file(files_folder_path+page_name), QHttpServerResponse::StatusCode::Ok);
    response.setHeader("Content-Type", type);
    return std::move(response);
}

QHttpServerResponse Http_server::send_json(const QHttpServerRequest &request, const QJsonObject &json)
{
    QHttpServerResponse response(json);
    response.setHeader("Content-Type", "application/json");
    return std::move(response);
}

QHttpServerResponse Http_server::redirect(const QHttpServerRequest &request, const QString &endpoint)
{
    QHttpServerResponse response(QHttpServerResponse::StatusCode::Found);
    response.addHeader("Location", (http_domain_name+endpoint).toLatin1());
    return std::move(response);
}

QHttpServerResponse Http_server::cookie_redirect(const QHttpServerRequest &request, const QString &endpoint, QByteArray email_value)
{
    Cookie cookie("email", email_value, domain_name.toLatin1());
    QHttpServerResponse response(QHttpServerResponse::StatusCode::Found);
    qInfo() << "Raw cookie" << cookie.get_raw_cookie();
    response.addHeader("Set-Cookie", cookie.get_raw_cookie());
    response.addHeader("Location", (http_domain_name+endpoint).toLatin1());
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

bool Http_server::has_id_cookie(const QHttpServerRequest &request)
{
    QString request_cookie = request.value("Cookie");
    QStringList cookies = request_cookie.split("; ");
    for(auto& i : cookies) {
        if(i.startsWith("email", Qt::CaseInsensitive)) {
            qInfo() << "There is website cookie in the request";
            return true;
        }
    }
    return false;
}



