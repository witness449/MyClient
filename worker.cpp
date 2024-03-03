#include "worker.h"
#include <QHostAddress>
#include "myrequest.h"
#include "myresponse.h"
#include <QJsonDocument>
#include <thread>
#include <QFile>

Worker::Worker(QString authToken, QString log, QObject *parent, int last_Id)
{
    QFile file ("clientConfig.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    adr=in.readLine();
    port=in.readLine().toInt();

    file.close();

    adress.setAddress(adr);

    authorizationToken=authToken;
    lastId=last_Id;
    login=log;

    connect(this, SIGNAL(startWorker()), this, SLOT(startWorkerSlot()));
    connect(this, SIGNAL(startSync()), this, SLOT(startSyncSlot()));
    connect (this, SIGNAL (ToConnect()), this, SLOT(connectSlot()));

    emit startWorker();

    //readTimer.setSingleShot(true);
    presponse=new MyResponse();
}

void Worker::startWorkerSlot()
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

void Worker::connectSlot()
{
    socketSync->connectToHostEncrypted(adr, port);
    //socketSync->connectToHost(adress, port);
}

void Worker::slotSyncConnected(){
    emit workerConnected();
    emit startSync();
}

void Worker:: startSyncSlot()
{
    presponse->clear();
    MyRequest syncRequest;
    syncRequest.setMethod("PUT");
    syncRequest.setVersion("HTTP/1.1");
    syncRequest.setPath("/sync");
    QByteArray last_Id=QString::number(lastId).toUtf8();
    syncRequest.appendHeader("Id", last_Id);
    QByteArray authToken=authorizationToken.toUtf8();
    syncRequest.appendHeader("Auth_token", authToken);
    syncRequest.appendHeader("Login", login.toUtf8());

    QByteArray ba=0;

    syncRequest.write(ba, true, socketSync);
    lastId++;
}

void Worker::readFromServer()
{
     //if(presponse->getStatus()!=MyResponse::waitForBody){
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
    if(status.toInt()==500)
    {
        return;
    }

    QByteArray bodyData(presponse->getBody());
    QJsonDocument doc=QJsonDocument::fromJson(bodyData);
    buffer=doc.object();
    bytebuffer=bodyData;
    QString message=buffer["one"].toString()+"1";
    if (message!="1"){
        emit incomingMessage(message);
    }


    if(presponse->getStatus()!=MyResponse::waitForBody){
        std::chrono::milliseconds ms(30);
        std::this_thread::sleep_for(ms);
        emit startSync();
    }
  }

void Worker::slotSyncDisconnected(){
    emit workerDisconnected();
}
