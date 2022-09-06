#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "localclient.h"
#include "dbconnection.h"
#include "infotakerthread.h"

class localServer : public QTcpServer
{
public:
    explicit localServer(QObject *parent = nullptr);
    void startServer();                                               //Starts  login server listening and set up infotakerthread
    dbConnection db;                                                  //one dbconnection object for all program
    QList<localClient*> clientList;                                     //list for client object
    infoTakerThread *infotaker;                                         //infotaker thread object
    QTcpSocket* socketfinder(QString username);                         //returns to pointer of socket of given user
public slots:
    void newConnection(QString username, localClient *client);          //when newconnectionrequset signal emit this function works and make signal-slot connetion of new socket
    void readyRead();                                                   //read all incoming message from all clients and forward the reciever
    void disconnectedClient();                                  //works when a client disconnected or terminated from newserver and change the status on db

    // QTcpServer interface
protected:
signals:
    virtual void incomingConnection(qintptr handle);                    //virtual function come from qtcpserver inheritince
private:
    QTcpServer *newServer;                                          //mainserver for message communication


};

#endif // LOCALSERVER_H
