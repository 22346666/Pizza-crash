#include <QCoreApplication>
#include "http_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Http_server server;
    server.listen_ssl();
    return app.exec();
}
