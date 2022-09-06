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
        ui->statusLabel->hide();
        ui->loginButton->hide();
        ui->signinButton->hide();
        ui->usernameLabel->hide();
        ui->passwordLabel->hide();
        ui->passwordLineEdit->hide();
        ui->usernameLineEdit->hide();
    tempSocket = new QTcpSocket();
    tempSocket->abort();

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
    ui->ipAdressLine->setEnabled(true);
    ui->loginButton->hide();
    ui->signinButton->hide();
    ui->usernameLabel->hide();
    ui->passwordLabel->hide();
    ui->passwordLineEdit->hide();
    ui->usernameLineEdit->hide();
    ui->connectButton->show();
}




void login::on_connectButton_clicked()
{
    ui->connectButton->setEnabled(false);
    if(!(ui->statusLabel->text() == "connected")){

    serverIP = ui->ipAdressLine->text();

    tempSocket->connectToHost(serverIP,8765);
    if(!tempSocket->waitForConnected(3000)){

       ui->statusLabel->setText("server connection problem!");

    }else{
        ui->statusLabel->setText("connected");
        ui->ipAdressLine->setEnabled(false);
        ui->connectButton->hide();
        ui->statusLabel->show();
        ui->loginButton->show();
        ui->signinButton->show();
        ui->usernameLabel->show();
        ui->passwordLabel->show();
        ui->passwordLineEdit->show();
        ui->usernameLineEdit->show();
    }
    ui->statusLabel->show();

}
        ui->connectButton->setEnabled(true);
}

