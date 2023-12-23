#include "http_server.h"
#include "cookie.h"
#include "user.h"
#include "pizza.h"

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
    route_photos();
}

void Http_server::route_pages() {
    http_server.route("/", [this](const QHttpServerRequest &request) {
        return send_file(request, "Home.html", html_path);
    });

    http_server.route("/about", [this](const QHttpServerRequest &request) {
        return send_file(request, "About.html", html_path);
    });

    http_server.route("/order", [this](const QHttpServerRequest &request) {
        if(has_id_cookie(request)) {
            return send_file(request, "Order.html", html_path);
        }
        return QHttpServerResponse(read_file(html_path+"401.html"), QHttpServerResponse::StatusCode::Unauthorized);
    });

    http_server.route("/main_menu", [this](const QHttpServerRequest &request) {
        return send_file(request, "MainMenu.html", html_path);
    });

    http_server.route("/sign_in", [this](const QHttpServerRequest &request) {
        if(has_id_cookie(request)) {
            return redirect(request, "/");
        }
        return send_file(request, "Login.html", html_path);
    });

    http_server.route("/sign_up", [this](const QHttpServerRequest &request) {
        if(has_id_cookie(request)) {
            return redirect(request, "/");
        }
        return send_file(request, "Register.html", html_path);
    });

    http_server.route("/profile", [this](const QHttpServerRequest &request) {
        if(has_id_cookie(request)) {
            return send_file(request, "Profile.html", html_path);
        }
        return QHttpServerResponse(read_file(html_path+"401.html"), QHttpServerResponse::StatusCode::Unauthorized);
    });

    http_server.route("/post/user/sign_up", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(user.create(db.db)) {
            Cookie cookie("email", user.get_email().toLatin1(), http_domain_name.toLatin1());
            QHttpServerResponse response(QHttpServerResponse::StatusCode::Ok);
            response.addHeader("Set-Cookie", cookie.get_raw_cookie());
            return cookie_redirect(request, "/", user.get_email().toLatin1());
        }
        return redirect(request, "/sign_in");
    });

    http_server.route("/post/user/sign_in", [this](const QHttpServerRequest &request) {
        qInfo() << request.body();
        QUrlQuery url(request.body());
        qInfo() << url.queryItems(QUrl::FullyDecoded);
        User user(url.queryItems(QUrl::FullyDecoded));
        if(!user.check_exist(db.db)) {
            return redirect(request, "/sign_up");
        }
        if(user.password_check(db.db)) {

            return cookie_redirect(request, "/", user.get_email().toLatin1());
        }
        return redirect(request, "/sign_in");
    });

    http_server.route("/get/user/<arg>", [this](const QString email, const QHttpServerRequest &request) {
        qDebug() << email;
        User user(db.db, email);
        return send_json(request, user.get_json());
    });

    http_server.route("/patch/user", [this](const QHttpServerRequest &request) {
        QJsonDocument data = QJsonDocument::fromJson(request.body());
        qDebug() << data.object();
        User::update(db.db, data.object());
        return "Ok";
    });

    http_server.route("/get/pizza", [this](const QHttpServerRequest &request) {
        qDebug() << "Someone asked for pizza list";
        return send_json(request, Pizza::get_json(db.db));
    });
}

void Http_server::route_scripts() {
    QString endpoint = "/scripts/";
    QByteArray type = "text/javascript";

    http_server.route(endpoint+"main_menu", [this, type](const QHttpServerRequest &request) {
        return send_file(request, "MainMenu.js", scripts_path, type);
    });

    http_server.route(endpoint+"home", [this, type](const QHttpServerRequest &request) {
        return send_file(request, "Home.js", scripts_path, type);
    });
}

void Http_server::route_photos()
{
    QString endpoint = "/photos/";
    QByteArray png_type = "image/png";
    QByteArray jpg_type = "image/jpg";

    http_server.route(endpoint+"cart", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "Cart.png", photos_path, png_type);
    });

    http_server.route(endpoint+"backgr_shape", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "BackgrShape.png", photos_path, png_type);
    });

    http_server.route(endpoint+"backgr_shape_c", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "BackgrShapeC.png", photos_path, png_type);
    });

    http_server.route(endpoint+"backgr_shape_r", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "BackgrShapeR.png", photos_path, png_type);
    });

    http_server.route(endpoint+"fish", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "fish.png", photos_path, png_type);
    });

    http_server.route(endpoint+"default_pizza", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "DefaultPizza.png", photos_path, png_type);
    });

    http_server.route(endpoint+"facebook", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "Facebook.png", photos_path, png_type);
    });

    http_server.route(endpoint+"instagram", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "Instagram.png", photos_path, png_type);
    });

    http_server.route(endpoint+"phone", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "Phone.png", photos_path, png_type);
    });

    http_server.route(endpoint+"pizza_background", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "PizzaBackgr.png", photos_path, png_type);
    });

    http_server.route(endpoint+"margh", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "margh.png", photos_path, png_type);
    });

    http_server.route(endpoint+"diabolo", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "diabolo.png", photos_path, png_type);
    });

    http_server.route(endpoint+"veggie", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "veggie.png", photos_path, png_type);
    });

    http_server.route(endpoint+"pepperoni", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "pepperoni.png", photos_path, png_type);
    });

    http_server.route(endpoint+"four_cheese", [this, png_type](const QHttpServerRequest &request) {
        return send_file(request, "four_cheese.png", photos_path, png_type);
    });
}

void Http_server::route_css() {
    QString endpoint = "/styles/";
    QByteArray type = "text/css";

    http_server.route(endpoint+"pizza", [this, type](const QHttpServerRequest &request) {
        return send_file(request, "StylesPizza.css", css_path, type);
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



