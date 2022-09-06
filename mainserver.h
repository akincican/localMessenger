#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include <QTcpServer>
#include "localclient.h"
class mainServer : public QObject
{
    Q_OBJECT
public:
    explicit mainServer(QObject *parent = nullptr);
    QTcpServer *server;
    void start();
signals:
public slots:
    void newConnection(QString username, localClient *client);
};

#endif // MAINSERVER_H
