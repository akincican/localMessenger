#ifndef LOCALCLIENT_H
#define LOCALCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QThreadPool>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMutex>
#include "dbconnection.h"

class localClient : public QObject
{
    Q_OBJECT
public:
    explicit localClient(QObject *parent = nullptr);
    void setSocket(int Descriptor);                                         //sets a new socket with given descriptor
    void loginProcess(QTcpSocket *socket);                                  //prepare incoming data from client's login screen for db check
    bool isLoggedIn=false;                                              // status of user
    bool isSignedIn = false;
    QString username;                                                   // username of client
    dbConnection *db;                                                   // one db for all program
    int descriptor;                                                     // descriptor ID of the this clients socket
    QTcpSocket *newSocket;                                              //new QTCpsocket object for communicating new server
signals:
    void newConnectionRequest(QString username, localClient * client);  //emit username and object pointer of 'this' object when new connection request

public slots:

    void connected();                                               //just useful for debug
    void disconnected();                            //just useful for debug
    void readyRead();                              //read data from socket and call login process and emit newConnectionRequest signal
    void TaskResult(QByteArray usersInfo);          //when infotakerthread done this void write the info data to client
    void newDisconnected();                        //sets user status to 0 at db and delete 'this' local client object
    void newreadyread();                           //just useful for debug



private:

    QTcpSocket *socket;                         //this socket just use for login proccess
};

#endif // LOCALCLIENT_H
