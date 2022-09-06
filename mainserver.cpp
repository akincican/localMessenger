#include "mainserver.h"

mainServer::mainServer(QObject *parent)
    : QObject{parent}
{
    server = new QTcpServer;
}

void mainServer::start()
{
    qDebug()<<"start mainserver";
    server->listen(QHostAddress::Any, 1000);
}

void mainServer::newConnection(QString username, localClient *client)
{
    qDebug()<<"nc"<<server->isListening()<<"socket"<<client->newSocket;
    if(server->waitForNewConnection(5000)){

    qDebug()<<"server- newConnection"<<server->hasPendingConnections();
    client->newSocket = server->nextPendingConnection();
    qDebug()<<client->newSocket->isReadable();
    qDebug()<< username <<": connectedto:"<<client->newSocket;}
    else{
        qDebug()<<"no Request";
    }
//    if(socket->isReadable()){
//        emit connectionEstablished(QString username);
//    }
//    disconnect(client,&localClient::newConnectionRequest,this,&localServer::newConnection);

    connect(client->newSocket, &QTcpSocket::readyRead,client,&localClient::newreadyread);
}
