#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QThread>
login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{

    serverIP = "127.0.0.1";

    ui->setupUi(this);
    tempSocket = new QTcpSocket();
    tempSocket->abort();
    tempSocket->connectToHost(serverIP,8765);
    if(!tempSocket->waitForConnected(30000)){
       ui->statusLabel->show();
       ui->statusLabel->setText("server connection problem!");
        return;
    }else{
        ui->statusLabel->setText("connected");
    }


    QObject::connect(tempSocket, &QTcpSocket::readyRead, this, &login::socketReadData);
    QObject::connect(tempSocket, &QTcpSocket::disconnected, this, &login::disconnected);
    QObject::connect(tempSocket, &QTcpSocket::destroyed, this, &login::disconnected);

}

login::~login()
{
    delete ui;
}

void login::on_loginButton_clicked()
{
    username;

    QByteArray rawUsername = ui->usernameLineEdit->text().toLatin1();
    int i = rawUsername.size();

    username= (rawUsername);
        username.resize(20);
    for(i; i<20; i++){
        username[i]= ' ' ;
    }
    QByteArray password;

    QByteArray rawPassword = ui->passwordLineEdit->text().toLatin1();
    int j = rawPassword.size();
    password= rawPassword;
    password.resize(16);
    for(j; j<16; j++){
        password[j]= ' ' ;
    }
    tempSocket->write("1");
    tempSocket->write(username);
    tempSocket->write(password);
    tempSocket->flush();


}

void login::socketReadData()
{
    qDebug()<<"reading..";
    QByteArray incoming = tempSocket->readAll();
    qDebug()<<incoming;
    int response = incoming.toInt();

    if (response == 0){

        ui->statusLabel->setText("invalid username or password");

    }else if(response == 2){

        ui->statusLabel->setText("Sign in successfuly");
    }else if(response == 3){

        ui->statusLabel->setText("username is not available");

    }else{
        ui->loginButton->setDisabled(true);
        serverPort = response;
        qDebug()<<"login sucessful";
        tempSocket->disconnectFromHost();
        this->hide();


        w = new MainWindow(this);

//        qDebug()<< "login.cpp" <<"ip: "<<serverIP<<" port: "<<serverPort;

        w->serverIP = serverIP;
        w->serverPort = serverPort;
        w->path = path;
        w->myusername = ui->usernameLineEdit->text();
        w->show();

    }

}





void login::on_signinButton_clicked()
{
    QByteArray username;

    QByteArray rawUsername = ui->usernameLineEdit->text().toLatin1();
    int i = rawUsername.size();

    username= (rawUsername);
        username.resize(20);
    for(i; i<20; i++){
        username[i]= ' ' ;
    }
    QByteArray password;

    QByteArray rawPassword = ui->passwordLineEdit->text().toLatin1();
    int j = rawPassword.size();
    password= rawPassword;
    password.resize(16);
    for(j; j<16; j++){
        password[j]= ' ' ;
    }
    tempSocket->write("0");
    tempSocket->write(username);
    tempSocket->write(password);
    tempSocket->flush();

}

void login::disconnected()
{
    ui->statusLabel->setText("server connection problem!");

}

