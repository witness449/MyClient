#include "clientlogic.h"
#include <QFile>
#include "myrequest.h"
#include <QJsonDocument>

int ClientLogic::count=0;

ClientLogic::ClientLogic(MyDatabase* pD, QObject *parent) :
    QObject(parent)
{

    QFile file ("clientConfig.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    adr=in.readLine();
    port=in.readLine().toInt();

    file.close();

    pMyDB=pD;

    clientState.SetLastEvents(pMyDB);
    clientState.SetRooms(pMyDB);

    address.setAddress(adr);

}


 void ClientLogic::ConnectSlot()
 {
     if ((socketPut.state()!=QAbstractSocket::ConnectedState)||(socketPut.state()!=QAbstractSocket::ConnectingState)){ //НЕ ПРОВЕРЯЕТ!!!
         /*QFile certfile("D:\\HttpQt\\MyClient\\MyClient\\MyClient\\ca.crt");
         certfile.open(QIODevice::ReadOnly);
         QSslCertificate cert(&certfile,QSsl::Pem);
         socketPut.setLocalCertificate(cert);
         certfile.close();*/
         socketPut.connectToHostEncrypted(adr, port);
         QObject::connect(&socketPut, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), &socketPut, static_cast<void (QSslSocket::*)()>(&QSslSocket::ignoreSslErrors));
         //QObject::connect(&socketPut, static_cast<void (QSslSocket::*)(const QList<QSslError> &)>(&QSslSocket::sslErrors), this, &MainWindow::printSslErrors);
         connect(&socketPut,SIGNAL(connected()),this,SLOT(slotConnected()));
         connect(&socketPut,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
         connect(&socketPut,SIGNAL(readyRead()),this,SLOT(readFromServer()));
     }
 }

 void ClientLogic::slotConnected()
 {

 }

 void ClientLogic::slotDisconnected()
 {

 }

 void ClientLogic::readFromServer()
 {
     if(presponse.getStatus()!=MyResponse::waitForBody)
     {
         presponse=*new MyResponse();
     }

     while (socketPut.bytesAvailable() &&
            presponse.getStatus()!=MyResponse::complete &&
            presponse.getStatus()!=MyResponse::abort_size &&
            presponse.getStatus()!=MyResponse::abort_broken)

     {
         presponse.readFromSocket(socketPut);
     }

     QByteArray reqType=presponse.findHeader("request-type");

     if(QString::localeAwareCompare(reqType, "AuthentificationStage1")==0&&presponse.getBody().size()>0)
     {
         QByteArray bodyData(presponse.getBody());
         QJsonDocument doc=QJsonDocument::fromJson(bodyData);
         QJsonObject buffer=doc.object();

         MyRequest authRequest;
         authRequest.setMethod("POST");
         authRequest.setVersion("HTTP/1.1");
         authRequest.setPath("/auth");

         authRequest.appendHeader("Content-Type", "application/json");
         QJsonObject jsonObject;
         jsonObject["identifier"] = login;
         jsonObject["password"] = password;
         jsonObject["session"]=buffer["session"];
         QJsonDocument document=QJsonDocument(jsonObject);
         QByteArray authData = document.toJson();

         authRequest.write(authData, true, &socketPut);
     }


     if(QString::localeAwareCompare(reqType, "AuthentificationStage2")==0&&presponse.getBody().size()>0)
     {
         if (!authorizationgFlag)
         {
             QByteArray bodyData(presponse.getBody());
             QJsonDocument doc=QJsonDocument::fromJson(bodyData);
             QJsonObject buffer=doc.object();

             authorizationToken=buffer["Authorization_token"].toString();
             QJsonArray RoomsArr= buffer["Rooms"].toArray();

             int i=1;
             //while(buffer[QString::number(i)+"Room"].toString()!=""){
                 //Rooms.append(buffer[QString::number(i)+"Room"].toString()); //Логин RoomsArr
                 //RoomNames.insert(buffer[QString::number(i)+"Room"].toString().split(" ")[1], buffer[QString::number(i)+"Room"].toString().split(" ")[0].toInt()); //Логиен RoomID
                 //i++;
             for (auto el:RoomsArr)
             {
                 //Rooms.append(el.toObject()["id"].toString());
                 //RoomNames.insert(el.toObject()["login"].toString(), el.toObject()["id"].toString().toInt());
                 Room room;
                 room.Id=el.toObject()["id"].toString().toInt();
                 room.Name=el.toObject()["login"].toString();
                 room.IsActive=1;

                 Contact c;
                 c.Login=el.toObject()["login"].toString();
                 c.IdRoom=el.toObject()["id"].toString().toInt();


                 pMyDB->insertRoom(room);
                 pMyDB->insertContact(c);
                 pMyDB->printTable();

                 //ui->roomBox->addItem(el.toObject()["login"].toString());
             }

             /*for(auto& el:RoomNames)
             {
                 ui->roomBox->addItem(RoomNames.key(el));
             }*/

             clientState.SetToken(authorizationToken);
             clientState.SetRooms(pMyDB);
             clientState.SetLastEvents(pMyDB);

             emit AuthPass();

             authorizationgFlag=true;
             if (authorizationToken=="")
                 authorizationgFlag=false;
         }

         if ((QString::localeAwareCompare(presponse.returnStatus(), "200")==0)&&(authorizationgFlag==true)){
             //ui->statusLabel->setText("Authorized");

             if (count!=0)
             {
                 thread->exit();
             }
             count++;

             //pMyDB->dropTable();
             //pMyDB->createTable();
             int lastId=pMyDB->selectMessageId()+1;
             QString arg="arg";
             QString authToken=authorizationToken.mid(0, 16);
             authToken+="_"+arg;

             thread = new SyncThread(clientState, authToken, login, this, lastId);
             connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

             thread->start();
             QObject::connect(thread, SIGNAL(syncConnected()), this, SLOT(syncConnected()));
             QObject::connect(thread, SIGNAL(syncDisconnected()), this, SLOT(syncDisconnected()));


             QObject::connect(thread, SIGNAL(incomingMessageEventSync(Event)), this, SLOT(incomingMessageMWSlot(Event)));
             QObject::connect(thread, SIGNAL(incomingRoomSync(Room, QString)), this, SLOT(incomingRoomMWSlot(Room, QString)));
             QObject::connect(this, SIGNAL(clientStateChanged(ClientState)), thread, SLOT(clientStateChangedSLOT(ClientState)));


             QObject::connect(this, SIGNAL (stopSync()), thread, SLOT(stopSyncSlot(tokenChangedMWSlot(QString))));





         }

         else{
            // ui->statusLabel->setText("Unauthorized");
         }
     }
     else if(QString::localeAwareCompare(reqType, "Registration")==0){
         /*if (QString::localeAwareCompare(presponse.returnStatus(), "200")==0){
             ui->statusLabel->setText("Registered");
         }
         else{
             ui->statusLabel->setText("Unregistered");
         }*/
     }
 }

 void ClientLogic:: ToRegisterSlot(QString log, QString pass)
 {
     login=log;
     password=pass;

     MyRequest regRequest;
     regRequest.setMethod("POST");
     regRequest.setVersion("HTTP/1.1");
     regRequest.setPath("/reg");

     regRequest.appendHeader("Content-Type", "application/json");
     QJsonObject jsonObject;
     jsonObject["Login"] = login;
     jsonObject["Password"] = password;
     QJsonDocument document=QJsonDocument(jsonObject);
     QByteArray regData = document.toJson();

     regRequest.write(regData, true, &socketPut);
 }

 void ClientLogic::ToAuthentificateSlot(QString log, QString pass)
 {
     login=log;
     password=pass;

     MyRequest authRequest;
     authRequest.setMethod("GET");
     authRequest.setVersion("HTTP/1.1");
     authRequest.setPath("/auth");

     authRequest.write(QByteArray(), true, &socketPut);


 }


 void ClientLogic::incomingMessageMWSlot(Event message)
 {
     pMyDB->insertMessage(message.Content, message.Id, message.IdRoom);

     clientState.SetLastEvents(pMyDB);

     /*if(contactLogin!=""){
     QList<QString> textList=pMyDB->takeMessages(contactLogin);
     //ui->chatBrowser->clear();

     //for(QString & x:textList)
         //ui->chatBrowser->append(x);
     }*/

     emit clientStateChanged(clientState);



     /*QList<QString> textList=pMyDB->takeMessages();
     ui->chatBrowser->clear();

     for(QString & x:textList)
         ui->chatBrowser->append(x);*/
 }

 void ClientLogic::incomingRoomMWSlot(Room r, QString s)
 {
     pMyDB->insertRoom(r);
     Contact c;
     c.Login=r.Name;
     c.IdRoom=r.Id;
     pMyDB->insertContact(c);
     authorizationToken=s;

     clientState.SetToken(authorizationToken);
     clientState.SetRooms(pMyDB);
     clientState.SetLastEvents(pMyDB);

     emit AuthPass();


     emit clientStateChanged(clientState);


 }

 void ClientLogic::ToSendSlot(QString contLog, QString txt)
 {
     MyRequest sendRequest;
     sendRequest.setMethod("POST");
     sendRequest.setVersion("HTTP/1.1");
     sendRequest.setPath("/send");

     QString text=txt;

     sendRequest.appendHeader("Content-Type", "application/json");
     QJsonObject jsonObject;
     jsonObject["message"] = text;

     //QString room=ui->roomBox->currentText();
     QString room=contLog;
     //qDebug()<<contactLogin;
     //jsonObject["room_id"] = QString::number(*RoomNames.find(room));
     int roomId=pMyDB->selectRoomId(room);
     jsonObject["room_id"]=QString::number(roomId);
     qDebug()<<QString::number(roomId);

     jsonObject["login"]=login;

     QJsonDocument document=QJsonDocument(jsonObject);
     QByteArray sendData = document.toJson();

     sendRequest.write(sendData, true, &socketPut);
 }



