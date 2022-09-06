#ifndef DBCONNECTION_H
#define DBCONNECTION_H
#include <QSql>
#include <QSqlQuery>
#include <QDir>

class dbConnection                                                                    //one db connection object for all db operation
{
public:
    dbConnection();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    QByteArray quint16toByteArray(quint16 givenNumber);                                          //returns 2byte bytearray from quint16

    void reset();                                                                                //set 0 to all status

    QSqlQuery *query;

    bool signIn(QString username, QString password, QString ipadress);                //sign in process set new user row

    void selector();                                                                             //SELECT username, IP,port, status FROM userinfo

    bool login(bool isLogin,QString username, QString password, QString ipadress);              //login process return a true if username and password match db

    void setStatus(int status, QString username);                                           // set the status 0 or 1 given username

    QByteArray userInfoTaker();                                             //take the all db info and prepare for streaming


};

#endif // DBCONNECTION_H
