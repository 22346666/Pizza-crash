#include <QCoreApplication>
#include "http_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Http_server server;
    server.listen();
    server.listen_ssl();
    //server.read_HTML("D:/A tut y menua uchoba/4 Курс/Internet/html/labs/laba_2.html");

    return app.exec();
}
