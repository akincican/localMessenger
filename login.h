#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QTcpSocket>
#include "mainwindow.h"
namespace Ui {
class login;
}
class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();
   QString serverIP;
   int serverPort;
   QByteArray username;
   QString path;
private slots:
    void on_loginButton_clicked();

    void socketReadData();

    void on_signinButton_clicked();

    void disconnected();

private:
    Ui::login *ui;
    QTcpSocket *tempSocket;
    MainWindow *w;
};

#endif // LOGIN_H
