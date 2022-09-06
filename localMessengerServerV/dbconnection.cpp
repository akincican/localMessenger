#include "dbconnection.h"
#include "qsqlrecord.h"

dbConnection::dbConnection()
{
    db.setDatabaseName(QDir::currentPath()+"/messengerServer.db");
    db.open();
    query = new QSqlQuery(db);
    query->exec("CREATE TABLE userInfo (status	INTEGER, username	TEXT, encPassword	BLOB, IP	TEXT,port	INTEGER);");
    query->clear();

    db.close();

}

QByteArray dbConnection::quint16toByteArray(quint16 givenNumber)
{
    QByteArray ba_quint16;
     ba_quint16.resize(2);
     memcpy(ba_quint16.data(), &givenNumber,2);

     return ba_quint16;

}

void dbConnection::reset()
{
    db.open();
    query->prepare("UPDATE userinfo set status = 0 ");
    query->exec();
    query->clear();
    db.close();

}

bool dbConnection::signIn(QString username, QString password, QString ipadress)
{
    db.open();
    qDebug()<<username;

    query->exec("SELECT username FROM userInfo WHERE username = '"+username+"';");

    if(query->first()){
        return false;
        qDebug()<<"false";
    }else{
        qDebug()<<"true";
    query->prepare("INSERT INTO userInfo(status, username, encPassword, IP, port) VALUES (:status, :username, :encPassword, :IP, :port)");
    query->bindValue(0, 0);
    query->bindValue(1, username);
    query->bindValue(2, password);
    query->bindValue(3, ipadress);
    query->bindValue(4, 1000);
    query->exec();
    return true;
    }

    query->clear();
    db.close();
}

void dbConnection::selector()
{
    db.open();

    query->exec("SELECT username, IP,port, status FROM userinfo");
    db.close();
    query->clear();

}

bool dbConnection::login(bool isLogin,QString username, QString password, QString ipadress)
{
    bool islogedIn = false;
//    if(isLogin){
        qDebug()<<"line58";
        bool isMatch = false;
        db.open();
        query->prepare("SELECT encPassword FROM userInfo WHERE username = '"+username+"'"+"and encPassword = '"+password+"'");
        query->exec();
        qDebug()<<"line59: "<<query->first();
        if(query->first() ){
            isMatch= true;

            qDebug()<<"login";
            query->prepare("UPDATE userInfo SET status = 1, IP = '" + ipadress +  "', port = '1000' WHERE username = '"+username+"'" );
            query->exec();

        }else{
            qDebug()<<"incorrect password";
        }



        if(isMatch){
            //        if(isFirstLogin){
            //            infoSenderTimer->start(10000);
            //            isFirstLogin = false;
            //        }
            qDebug()<<"line68";
            islogedIn= true;
        }else{
            islogedIn= false;
        }


 /*   }else{
        query->prepare("INSERT INTO userInfo(status, username, encPassword, IP, port) VALUES (:status, :username, :encPassword, :IP, :port)");
        query->bindValue(0, 1);
        query->bindValue(1, username);
        query->bindValue(2, password);
        query->bindValue(3, ipadress);
        query->bindValue(4, 1000);
        query->exec();
        query->clear();
    }*/
    query->clear();
    db.close();
    return islogedIn;
}

void dbConnection::setStatus(int status, QString username)
{
    db.open();

    if(status == 1){

        query->exec("UPDATE userInfo SET status = 1 WHERE username = '"+username+"'" );

    }else if(status == 0){

        query->exec("UPDATE userInfo SET status = 0 WHERE username = '"+username+"'" );

    }

    db.close();
    query->clear();
}

QByteArray dbConnection::userInfoTaker()
{
    db.open();
    QByteArray usersInfo;

        query->exec("SELECT username, IP,port, status FROM userinfo");
        quint16 i = 0;
            while(query->next())
            {

                for (int j = 0; j < query->record().count(); ++j) {
                    if(j==0){
    //                    qDebug()<<"94: "<<infoQuery.value(j).toByteArray();
                        quint16 temp = (query->value(j).toByteArray()).size();
    //                    qDebug()<<"temp: "<<temp;
                        usersInfo.append(quint16toByteArray(temp));


                    }

    //                qDebug()<<"j:" <<j;
                    if(j==1){
    //                    qDebug()<<"106: "<<infoQuery.value(j).toByteArray();
                        quint16 temp = (query->value(j).toByteArray()).size();
                        QByteArray temparray = quint16toByteArray(temp);
    //                    qDebug()<<"temp: "<<temp;
                        usersInfo.append(temparray);
                    }
                    if(j == 3){
    //                    qDebug()<<"status short: "<<infoQuery.value(j).toByteArray().toShort();
                        QByteArray tempByteArray = quint16toByteArray(query->value(j).toByteArray().toShort());
                        usersInfo.append(tempByteArray);
                    }else{
                        usersInfo.append(query->value(j).toByteArray());
    //                qDebug()<<usersInfo;
                    }
                }
                i++;
            }
        QByteArray numberofRow = quint16toByteArray(i);
        usersInfo= numberofRow+usersInfo;
        QByteArray compressedInfo = qCompress(usersInfo);

        return compressedInfo;
        db.close();
        query->clear();
}
