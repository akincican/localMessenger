#include "localclient.h"

localClient::localClient(QObject *parent)
    : QObject{parent}
{

    QThreadPool::globalInstance()->setMaxThreadCount(15);
}

void localClient::setSocket(int Descriptor)
{
    socket = new QTcpSocket(this);

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    descriptor = Descriptor;
    socket->setSocketDescriptor(Descriptor);
    qDebug() <<"client Connected";
}

void localClient::loginProcess(QTcpSocket *socket)
{
    QByteArray rawData = socket->readAll();
    while(!isLoggedIn){
    bool isLogin;
    QString password;
    QString userName;
    QString ip = socket->peerAddress().toString();

    qDebug()<<"line48"<<rawData;
    isLogin = rawData.first(1).toInt() == 1;
    userName = rawData.mid(1,20);
    userName = userName.trimmed();
    password = rawData.last(16);
    password = password.trimmed();
    qDebug()<<isLogin<<" "<<userName<<" "<<password;
    if(isLogin){
    if(db->login(isLogin,userName,password,ip)){
        isLoggedIn=true;
    }else{
        socket->write("0");
        break;

    }
    this->username = userName;
    }else{
        if(db->signIn(userName,password,ip)){
            socket->write("2");
        }else{
            socket->write("3");
        }
        break;

    }
}
}


void localClient::connected()
{
    qDebug() <<"client Connected event";

}

void localClient::disconnected()
{
   qDebug() <<"client disConnected event";
}

void localClient::readyRead()
{
    qDebug()<<"client readread ";

  if(!isLoggedIn){
      loginProcess(socket);
  }
  if(isLoggedIn){

  socket->write(QByteArray::number(1000));
  socket->flush();
  QHostAddress ip6Address = socket->peerAddress();
  QHostAddress ip4Address(ip6Address.toIPv4Address());

  qDebug()<<"socket peer adress: "<< ip4Address<<"port: "<<1000<< "username: "<<username;

  qDebug()<<"client ptr: "<<this;
    emit newConnectionRequest(username, this);
  }

}

void localClient::TaskResult(QByteArray usersInfo)
{

    newSocket->write(usersInfo);
    qDebug()<<username <<": line 141: "<<usersInfo;




}

void localClient::newDisconnected()
{   /*infotaker->isitDone =false;*/
    qDebug()<<username <<"disconnect";
    db->setStatus(0, username);
    this->deleteLater();
}

void localClient::newreadyread()
{
    qDebug()<<"readyread signal username:"<<username;


}



