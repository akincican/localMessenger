#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    class user{
    public:
        QString username;
        QHostAddress ip;
        int port;
        bool status;
    };
    user userArray[100];
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString serverIP;
    int serverPort;
    quint16 byteArraytoquint16(QByteArray inputData);                                   //returns quint16 from 2byte bytearray
    QString currentPair = "NULL";
    QFile file;
    QString myusername;
    QString path;

private slots:
    void socketReadData();
    void on_sendButton_clicked();
    void handShaking();
    void disconnected();
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    QTcpSocket * socket;
    QTimer * timer;
};
#endif // MAINWINDOW_H
