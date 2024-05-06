#include "clientlogic.h"
#include <QFile>
#include "myrequest.h"
#include <QJsonDocument>
#include "registrator.h"

int ClientLogic::count=0;



ClientLogic::ClientLogic(MyDatabase* pD, QObject *parent) :QObject(parent), pMyDB (pD)
{
    QFile file ("clientConfig.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    netconfig.adr=in.readLine();
    netconfig.port=in.readLine().toInt();
    file.close();

    clientState.setLastEvents(pMyDB);
    clientState.setRooms(pMyDB);
    netconfig.address.setAddress(netconfig.adr);
}


 void ClientLogic::connectSlot()
 {
     if ((socketPut.state()!=QAbstractSocket::ConnectedState)||(socketPut.state()!=QAbstractSocket::ConnectingState)){
         /*QFile certfile("D:\\HttpQt\\MyClient\\MyClient\\MyClient\\ca.crt");
         certfile.open(QIODevice::ReadOnly);
         QSslCertificate cert(&certfile,QSsl::Pem);
         socketPut.setLocalCertificate(cert);
         certfile.close();*/
         socketPut.connectToHostEncrypted(netconfig.adr, netconfig.port);
         QObject::connect(&socketPut, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), &socketPut, static_cast<void (QSslSocket::*)()>(&QSslSocket::ignoreSslErrors));
         //QObject::connect(&socketPut, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), this, &MainWindow::printSslErrors);
         connect(&socketPut,SIGNAL(connected()),this,SLOT(slotConnected()));
         connect(&socketPut,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
         connect(&socketPut,SIGNAL(readyRead()),this,SLOT(readFromServer()));
     }
 }

 void ClientLogic::slotConnected()
 {
     emit emitConnected();
 }

 void ClientLogic::slotDisconnected()
 {
     emit emitDisconnected();
 }

 void ClientLogic::readFromServer()
 {
     if(presponse->getStatus()!=MyResponse::waitForBody)
     {
         presponse=new MyResponse();
     }

     while (socketPut.bytesAvailable() &&
            presponse->getStatus()!=MyResponse::complete &&
            presponse->getStatus()!=MyResponse::abort_size &&
            presponse->getStatus()!=MyResponse::abort_broken)

     {
         presponse->readFromSocket(socketPut);
     }

     QByteArray reqType=presponse->findHeader("request-type");

     if(QString::localeAwareCompare(reqType, "Registration")==0){
         registrator->readResponse(presponse);
         emit emitStatus(registrator->status);
         delete presponse;
         delete registrator;
     }
     else if(QString::localeAwareCompare(reqType, "AuthentificationStage1")==0&&presponse->getBody().size()>0)
     {
         authorizer->readResponse(presponse);
         delete presponse;
     }

     else if(QString::localeAwareCompare(reqType, "AuthentificationStage2")==0&&presponse->getBody().size()>0)
     {
         if (!authorizationgFlag)
         {
             authorizer->readResponse(presponse);
         }

         this->setClientState();


         if ((QString::localeAwareCompare(presponse->returnStatus(), "200")==0)&&(authorizationgFlag==true))
         {
             startSynchronization();
             emit emitStatus("Authorized");
             delete authorizer;
             delete presponse;
         }
         else{
             emit emitStatus ("Unauthorized");
             delete authorizer;
             delete presponse;
         }
     }
     else if (QString::localeAwareCompare(reqType, "Create_room")==0) {
         contactCreator->readResponse(presponse);
         emit emitStatus(contactCreator->status);
         delete contactCreator;
         delete presponse;
     }
     else if (QString::localeAwareCompare(reqType, "Send")==0) {
         sender->readResponse(presponse);
         emit emitStatus(sender->status);
         delete sender;
         delete presponse;
     }
     else if (QString::localeAwareCompare(reqType, "Leave")==0) {
         leaver->readResponse(presponse);
         emit emitStatus(leaver->status);
         delete leaver;
         delete presponse;
     }
     else if (QString::localeAwareCompare(reqType, "Ban")==0) {
         banner->readResponse(presponse);
         emit emitStatus(banner->status);
         delete banner;
         delete presponse;
     }
     else if (QString::localeAwareCompare(reqType, "Unban")==0) {
         unbanner->readResponse(presponse);
         emit emitStatus(unbanner->status);
         delete unbanner;
         delete presponse;
     }


 }

 void ClientLogic:: toRegisterSlot(QString log, QString pass)
 {
     registrator=new Registrator(account, &socketPut);
     account.login=log;
     account.password=pass;
     registrator->sendRequest();

 }

 void ClientLogic::toAuthentificateSlot(QString log, QString pass)
 {
     account.login=log;
     account.password=pass;
     authorizer =new Authorizer(account, &socketPut, pMyDB);
     authorizer->sendRequest();
}


 void ClientLogic::incomingMessageMWSlot(Event message)
 {
     pMyDB->insertMessage(message);
     clientState.setLastEvents(pMyDB);
     emit clientStateChanged(clientState);
     emit emitMessage(message);

 }

 void ClientLogic::incomingRoomMWSlot(Room r, QString s)
 {
     pMyDB->insertRoom(r);
     Contact c;
     c.login=r.name;
     c.idRoom=r.id;
     pMyDB->insertContact(c);
     account.accessToken=s;

     clientState.setToken(account.accessToken);
     clientState.setRooms(pMyDB);
     clientState.setLastEvents(pMyDB);

     emit refreshRooms();
     emit clientStateChanged(clientState);
 }

 void ClientLogic::outcomingRoomMWSlot(Room r, QString s)
 {
     pMyDB->deleteRoom(r);
     account.accessToken=s;

     clientState.setToken(account.accessToken);
     clientState.setRooms(pMyDB);
     clientState.setLastEvents(pMyDB);

     emit refreshRooms();
     emit clientStateChanged(clientState);
 }

 void ClientLogic::toSendSlot(QString contLog, QString txt)
 {
     if (contLog!=""){
     sender = new Sender(account, &socketPut, pMyDB);
     sender->contactLogin=contLog;
     sender->txt=txt;
     sender->sendRequest();
     }
     else
     {
         emit emitStatus("Incorrect contact");
     }
 }

 void ClientLogic::toFindSLOT(QString contactLogin)
 {
     if (contactLogin!=""){
     contactCreator=new ContactCreator(account, &socketPut, pMyDB),
     contactCreator->contactLogin=contactLogin;
     contactCreator->sendRequest();
     }
     else
     {
         emit emitStatus("Incorrect contact");
     }
 }

 void ClientLogic::toLeaveSlot(QString contactLogin)
 {
     if (contactLogin!=""){
     leaver=new Leaver (account, &socketPut, pMyDB);
     leaver->contactLogin=contactLogin;
     leaver->sendRequest();
     }
     else
     {
         emit emitStatus("Incorrect contact");
     }
 }

 void ClientLogic::toBanSLOT(QString contactLogin)
 {
     if (contactLogin!=""){
     banner=new Banner (account, &socketPut, pMyDB);
     banner->contactLogin=contactLogin;
     banner->sendRequest();
     }
     else
     {
         emit emitStatus("Incorrect contact");
     }

 }

 void ClientLogic::startSynchronization()
 {
     int lastId=0;

     thread = new SyncThread(clientState, account.accessToken, account.login, this, lastId);
     connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

     thread->start();
     QObject::connect(thread, SIGNAL(syncConnected()), this, SLOT(syncConnected()));
     QObject::connect(thread, SIGNAL(syncDisconnected()), this, SLOT(syncDisconnected()));
     QObject::connect(thread, SIGNAL(incomingMessageEventSync(Event)), this, SLOT(incomingMessageMWSlot(Event)));
     QObject::connect(thread, SIGNAL(incomingRoomSync(Room, QString)), this, SLOT(incomingRoomMWSlot(Room, QString)));
     QObject::connect(this, SIGNAL(clientStateChanged(ClientState)), thread, SLOT(clientStateChangedSLOT(ClientState)));
     QObject::connect(thread, SIGNAL(outcomingRoomSync(Room, QString)), this, SLOT(outcomingRoomMWSlot(Room, QString)));

 }

 void ClientLogic::disConnectSlot()
 {
     socketPut.disconnectFromHost();
 }

 void ClientLogic::toUnBanSLOT(QString contactLogin)
 {
     if (contactLogin!=""){
     unbanner=new Unbanner (account, &socketPut, pMyDB);
     unbanner->contactLogin=contactLogin;
     unbanner->sendRequest();
     }
     else
     {
         emit emitStatus("Incorrect contact");
     }

 }

 void ClientLogic::slotLogout()
 {
     account.login="";
     account.password="";
     account.accessToken="";
     authorizationgFlag=false;
     thread->quit();
 }

 void const ClientLogic::setClientState()
 {
     clientState.setToken(account.accessToken);
     clientState.setRooms(pMyDB);
     clientState.setLastEvents(pMyDB);
     emit refreshRooms();
     authorizationgFlag=true;
     if (account.accessToken=="")
         authorizationgFlag=false;
 }





