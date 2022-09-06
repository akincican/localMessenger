#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QDir>
int main(int argc, char *argv[])
{
    QString path = QDir::currentPath()+"/messages/";
    QDir dir(path);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    QApplication a(argc, argv);
    login l;
    l.path = path;
    l.show();

    return a.exec();
}
