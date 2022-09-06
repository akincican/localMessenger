#include <QCoreApplication>
#include "localserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    localServer server;
    server.startServer();
    return a.exec();
}
