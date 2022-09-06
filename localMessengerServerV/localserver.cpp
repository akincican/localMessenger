 #include "localserver.h"

localServer::localServer(QObject *parent)
    : QTcpServer{parent}
{
    qDebug()<< "server constructor";
    db.reset();
    qDebug()<< "db reseted";
    newServer = new QTcpServer;
    newServer->listen(QHostAddress::Any, 1000);

    QThreadPool::globalInstance()->setMaxThreadCount(15);

}
void localServer::startServer()
{
    infotaker = new infoTakerThread;
    infotaker->db = &db;
    infotaker->setAutoDelete(true);

    if(listen(QHostAddress::Any,8765)){
        qDebug() <<"started";

    }else{
        qDebug() <<"not started!";
    }
}

QTcpSocket *localServer::socketfinder(QString username)
{
    QTcpSocket * socket;
    for(int i=0; i< clientList.length(); i++){
        if(clientList[i]->username == username){
            socket = clientList[i]->newSocket;
            break;
        }
    }
    return socket;
}

void localServer::newConnection(QString username, localClient *client)
{
//    qDebug()<<"nc"<<newServer->isListening()<<"socket"<<client->newSocket;
//    qDebug()<<"server: "<<hasPendingConnections();
    if(newServer->waitForNewConnection(5000)){

    qDebug()<<"server- newConnection"<<newServer->hasPendingConnections();
    client->newSocket = newServer->nextPendingConnection();
    qDebug()<<client->newSocket->isReadable();
    qDebug()<< username <<": connectedto:"<<client->newSocket;}
    else{
        qDebug()<<"no Request";
    }
    connect(infotaker,SIGNAL(Result(QByteArray)),client,SLOT(TaskResult(QByteArray)),Qt::QueuedConnection);

    connect(client->newSocket,&QTcpSocket::readyRead,this,&localServer::readyRead);
    connect(client->newSocket,&QTcpSocket::destroyed,this,&localServer::disconnectedClient);
    connect(client->newSocket,&QTcpSocket::disconnected,this,&localServer::disconnectedClient);

    if(clientList.length() == 1){
        QThreadPool::globalInstance()->start(infotaker);
    }
////    if(socket->isReadable()){
////        emit connectionEstablished(QString username);
////    }

}

void localServer::readyRead()
{
  QTcpSocket* currentSocket= reinterpret_cast <QTcpSocket*>(sender());
  QByteArray username;
  QByteArray username20;

  QByteArray rawMessage = currentSocket->readAll();
  for (int i = 0; i < clientList.length() ; ++i) {
      if(clientList[i]->newSocket == currentSocket){
          username = clientList[i]->username.toUtf8();
          break;
      }
  }
  username20 = username;
  username20.resize(20);
   for(int i = username.length();i<20;i++){
       username20[i] = ' ';
   }

  QString recieverUsername = rawMessage.first(20).trimmed();
  QByteArray message = rawMessage.remove(0,20);
  message = "message"+username20+message;
  QTcpSocket *recieverSocket = socketfinder(recieverUsername);
  recieverSocket->write(message);
  qDebug()<<"from: "<<username<<"to: "<<recieverUsername<< "\n"<<message<<currentSocket;

}

void localServer::disconnectedClient()
{  QString username;
    if(clientList.length() == 1){

        username = clientList[0]->username;
        clientList.clear();
        db.setStatus(0,username);
    }else{
    QTcpSocket* currentSocket= reinterpret_cast <QTcpSocket*>(sender());
    for (int i = 0; i < clientList.length() ; ++i) {
        if(clientList[i]->newSocket == currentSocket){
            username = clientList[i]->username;
            db.setStatus(0,username);
            clientList.removeAt(i);
            clientList.squeeze();
            break;
        }
    }
    }


    qDebug()<<username<<" disconnected";
    qDebug()<<"userlist"<<clientList;
}

void localServer::incomingConnection(qintptr handle)
{


    qDebug()<<"task start"<<"newserverptr:"<<newServer;

    //time consumer

    localClient *client = new localClient(this);
    clientList.append(client);
    client->setSocket(handle);
    client->db = &db;

    connect(client,&localClient::newConnectionRequest,this,&localServer::newConnection);
    emit
}
