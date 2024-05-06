#include "synchrologic.h"
#include <QHostAddress>
#include "myrequest.h"
#include "myresponse.h"
#include <QJsonDocument>
#include <thread>
#include <QFile>
#include "event.h"
#include "sender.h"
#include "util.h"




SynchroLogic::SynchroLogic(ClientState cState,  QString authToken, QString log, QObject *parent, int last_Id)
{
    QFile file ("clientConfig.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    netconfig.adr=in.readLine();
    netconfig.port=in.readLine().toInt();

    file.close();

    netconfig.address.setAddress(netconfig.adr);

    login=log;
    clientState=cState;
    QString token=cState.getToken();
    QMap<int, QString> rooms=TokenParse(token, login);

    connect(this, SIGNAL(startSynchroLogic()), this, SLOT(startSynchroLogicSlot()));
    connect(this, SIGNAL(startSync()), this, SLOT(startSyncSlot()));
    connect (this, SIGNAL (ToConnect()), this, SLOT(connectSlot()));

    emit startSynchroLogic();

    presponse=new MyResponse();
}

void SynchroLogic::startSynchroLogicSlot()
{
    /*socketSync=new QTcpSocket();
    if (socketSync->state()!=QAbstractSocket::ConnectedState){
        connect(socketSync,SIGNAL(connected()),this,SLOT(slotSyncConnected()));
        connect(socketSync,SIGNAL(disconnected()),this,SLOT(slotSyncDisconnected()));
        connect(socketSync,SIGNAL(readyRead()),this,SLOT(readFromServer()));*/

    socketSync=new QSslSocket();
    if (socketSync->state()!=QAbstractSocket::ConnectedState){
        QObject::connect(socketSync, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), socketSync, static_cast<void (QSslSocket::*)()>(&QSslSocket::ignoreSslErrors));
        //QObject::connect(&socketPut, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), this, &MainWindow::printSslErrors);
        connect(socketSync,SIGNAL(connected()),this,SLOT(slotSyncConnected()));
        connect(socketSync,SIGNAL(disconnected()),this,SLOT(slotSyncDisconnected()));
        connect(socketSync,SIGNAL(readyRead()),this,SLOT(readFromServer()));

        emit ToConnect();
    }
}

void SynchroLogic::connectSlot()
{
    socketSync->connectToHostEncrypted(netconfig.adr, netconfig.port);
    //socketSync->connectToHost(adress, port);
}

void SynchroLogic::slotSyncConnected(){
    emit synchroLogicConnected();
    emit startSync();
}

void SynchroLogic::clientStateChangedSynchroLogicSlot(ClientState cs)
{
    clientState=cs;
    emit startSync();
}

void SynchroLogic:: startSyncSlot()
{
    presponse->clear();
    MyRequest syncRequest;
    syncRequest.setMethod("PUT");
    syncRequest.setVersion("HTTP/1.1");
    syncRequest.setPath("/sync");
    QByteArray authToken=clientState.getToken().toUtf8();
    syncRequest.appendHeader("access_token", authToken);
    syncRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject=clientState.toJson();
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray ba = document.toJson();
    qDebug()<<ba;
    syncRequest.write(ba, true, socketSync);
}

void SynchroLogic::readFromServer()
{
    QByteArray dataRead;
    QByteArray qbabuffer;

    while (socketSync->bytesAvailable() &&
           presponse->getStatus()!=MyResponse::complete &&
           presponse->getStatus()!=MyResponse::abort_size &&
           presponse->getStatus()!=MyResponse::abort_broken)
    {
        presponse->readFromSocket(*socketSync);
    }

    QByteArray status=presponse->returnStatus();
    if(status.toInt()==500){
        return;
    }


    QByteArray bodyData(presponse->getBody());
    QJsonDocument doc=QJsonDocument::fromJson(bodyData);
    buffer=doc.object();
    bytebuffer=bodyData;

    if (buffer["type"].toInt()==0){
        Event event;
        event.content=buffer["Content"].toString();
        event.id=buffer["Id"].toInt();
        event.idRoom=buffer["IdRoom"].toInt();
        qDebug()<<QString::number(buffer["NewId"].toInt());
        if (event.content!=""){
            emit incomingMessageEvent(event);
        }
    }
    else if (buffer["type"]==1)
    {
        if (buffer["idRoom"].toInt()){
            Room r;
            QString token;
            r.id=buffer["idRoom"].toInt();
            r.isActive=true;
            r.name=buffer["RoomName"].toString();
            token=buffer["access_token"].toString();

            emit incomingRoom(r, token);
        }
        else if (buffer["banRoom"].toInt())
        {
            Room r;
            QString token;
            r.id=buffer["banRoom"].toInt();
            r.isActive=false;
            token=buffer["access_token"].toString();

            emit outcomingRoom(r, token);
        }
     }

    if(presponse->getStatus()!=MyResponse::waitForBody){
        std::chrono::milliseconds ms(30);
        std::this_thread::sleep_for(ms);
    }
  }

void SynchroLogic::slotSyncDisconnected(){
    emit synchroLogicDisconnected();
}
