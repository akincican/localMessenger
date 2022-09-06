#include "infotakerthread.h"
#include "qdebug.h"
#include "qsqldatabase.h"
#include "qthread.h"
infoTakerThread::infoTakerThread()
{


}



void infoTakerThread::run()
{
    QMutex mutex;
    QByteArray currentInfo;
    while(true){

    mutex.lock();
    QByteArray compressedUserInfo = db->userInfoTaker();
    mutex.unlock();
    qDebug()<< "runn";
    if (currentInfo != compressedUserInfo){
        currentInfo = compressedUserInfo;

    emit Result(compressedUserInfo);}
    QThread::sleep(5);

    }
}
