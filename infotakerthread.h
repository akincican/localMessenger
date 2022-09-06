#ifndef INFOTAKERTHREAD_H
#define INFOTAKERTHREAD_H

#include "qsqldatabase.h"
#include <QRunnable>
#include <QObject>
#include <QTimer>
#include "dbconnection.h"
#include <QMutex>
class infoTakerThread :public QObject, public QRunnable          //this thread detect changes at userinfo table and emit alldata
{
    Q_OBJECT
public:
    infoTakerThread();
    QByteArray quint16toByteArray(quint16 givenNumber);            //returns 2byte bytearray from quint16
    bool isitDone=true;
    dbConnection *db;                                               //db object same for all program

signals:
    void Result(QByteArray usersInfo);                              //emit all data when something change
private slots:
    // QRunnable interface
public:
    virtual void run();
private:
};

#endif // INFOTAKERTHREAD_H
