#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QThread>
#include <Qdir>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer();
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->pairLabel->hide();

    connect(timer,&QTimer::timeout,this,&MainWindow::handShaking );
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    file.close();
    delete ui;

}
quint16  MainWindow::byteArraytoquint16(QByteArray inputData){

    quint16  output;
    memcpy(&output, inputData.data(), 2);

    return output;

}
void MainWindow::socketReadData()
{
    ui->messageScreen->setAlignment(Qt::AlignLeft);

    QByteArray allData = socket->readAll();
    qDebug()<<allData;
    if(allData.first(7) == "message"){
        qDebug()<<"incoming message";
        QByteArray senderUsername = allData.mid(7,20).trimmed();
        QByteArray message = allData.mid(27);
        qDebug()<<"line41 file isopen: "<<file.isOpen()<<" senderusername: "<<senderUsername<<"currentpair: "<< currentPair;
        if(currentPair == senderUsername){

            qDebug()<<file.isOpen();
            file.write(senderUsername+": "+message+"\n");
            ui->messageScreen->append(senderUsername+": "+message+"\n");
        }else{
            QFile file2(path+myusername+"/"+senderUsername+".txt");
            file2.open(QIODevice::Append);
            file2.write(senderUsername+": "+message+"\n");
            file2.close();
        }
        qDebug()<<"message from:"<<senderUsername<<"\n"<<message;
    }else{

        ui->tableWidget->setRowCount(0);
        qDebug()<<"reading..";
        QByteArray compressedUserInfo = allData;
        QByteArray userInfo = qUncompress(compressedUserInfo);
        quint16 numberofuser = byteArraytoquint16(userInfo.first(2));
        qDebug()<<"numberofusers: "<<numberofuser;
        int cursor = 2;
        qDebug()<<userInfo;

        for (int i = 0; i < numberofuser; ++i) {

            quint16 sizeofUsername = byteArraytoquint16( userInfo.mid(cursor,2) );
            qDebug()<< "soun: "<<sizeofUsername;
            cursor= cursor+2;
            QString username = userInfo.mid(cursor, sizeofUsername);
            cursor = cursor+ sizeofUsername;
            qDebug() <<"username: " <<username;
            quint16 sizeofip = byteArraytoquint16( userInfo.mid(cursor,2) );
            qDebug()<< "soun: "<<sizeofip;
            cursor= cursor+2;
            QHostAddress ip =  QHostAddress(userInfo.mid(cursor, sizeofip));
            cursor = cursor+sizeofip;
            qDebug()<<"ip: "<<ip;

            int port = userInfo.mid(cursor,4).toInt();
            cursor= cursor+4;
            qDebug()<<port;

            bool status = byteArraytoquint16(userInfo.mid(cursor, 2)) == 1;
            qDebug()<<"statusint: "<<byteArraytoquint16(userInfo.mid(cursor, 2));
            cursor=cursor+2;

            qDebug()<<status;
            user tempuser;
            tempuser.username =username;
            tempuser.ip = ip;
            tempuser.port = port;
            tempuser.status = status;
            userArray[i] = tempuser;

            qDebug()<<userArray;

        }
        QString status;
        qDebug()<<"number of users: "<<numberofuser;
        for (int i = 0; i <numberofuser ; ++i) {
            ui->tableWidget->insertRow ( ui->tableWidget->rowCount() );

            ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,0,new QTableWidgetItem(userArray[i].username));
            if(userArray[i].status){
                status = "online";
                QTableWidgetItem statusItem(status);

            }

            ui->tableWidget->setItem   ( ui->tableWidget->rowCount()-1,1,new QTableWidgetItem(status));
            status = "offline";

        }

    }

    //"\x04\x00\t\x00""akincican::ffff:127.0.0.110001\x05\x00""burak::ffff:127.0.0.110001\x05\x00""alpay::ffff:127.0.0.110001\x03\x00""ali::ffff:10.240.4.8510001"
}


void MainWindow::on_sendButton_clicked()
{
    ui->textEdit->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->messageScreen->setAlignment(Qt::AlignRight);
    QByteArray username;
    QByteArray pair = currentPair.toUtf8();
    qDebug()<<"ip: "<<serverIP<<" port: "<<serverPort;
    QByteArray sendMessage;
    sendMessage = ui->textEdit->toPlainText().toUtf8();
    QString messageShow = myusername+": " +sendMessage;
    file.write(messageShow.toUtf8()+"\n");


    ui->messageScreen->append(messageShow+"\n");
    ui->textEdit->clear();

    int i = currentPair.size();

    username = pair;
    username.resize(20);
    for(i; i<20; i++){
        username[i]= ' ' ;
    }
    sendMessage = username+sendMessage;
    qDebug()<<sendMessage;
    qDebug()<<socket->isWritable();
    socket->write(sendMessage);
    socket->flush();
    ui->textEdit->setEnabled(true);
    ui->sendButton->setEnabled(true);
}

void MainWindow::handShaking()
{

    timer->stop();
    qDebug()<<"ip: "<<serverIP<<" port: "<<serverPort;
    socket = new QTcpSocket();
    socket->connectToHost(serverIP, serverPort);
    if(!socket->waitForConnected(30000)){
        ui->statusbar->showMessage("server connection problem!");
        return;
    }else{
        ui->statusbar->showMessage("connected");
    }


    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::disconnected);
    QObject::connect(socket, &QTcpSocket::destroyed, this, &MainWindow::disconnected);
    QDir dir(path+myusername);
    if (!dir.exists()){
        dir.mkpath(".");
    }
    QDir::setCurrent(path+myusername);

}

void MainWindow::disconnected()
{
    ui->statusbar->showMessage("server connection problem!");
    ui->sendButton->setEnabled(false);
    ui->textEdit->setEnabled(false);
}


void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    ui->textEdit->setEnabled(false);
    ui->sendButton->setEnabled(false);
    QString username = userArray[row].username;
    if(currentPair != username){
        if(currentPair != "NULL"){
            file.close();

        }
        ui->pairLabel->setText(username);
        ui->pairLabel->show();
        currentPair = username;
        file.setFileName(path+myusername+"/"+username+".txt");
        file.open(QIODevice::ReadOnly);

        QByteArray messageHistory = file.readAll();
        ui->messageScreen->setText(messageHistory);
        file.close();
        file.open(QIODevice::Append);
        if(userArray[row].status == 1){
            ui->textEdit->setEnabled(true);
            ui->sendButton->setEnabled(true);
        }
    }


}

