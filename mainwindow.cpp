#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myrequest.h"
#include "myresponse.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QJsonArray>
#include "room.h"
#include "event.h"

Q_DECLARE_METATYPE(Event);
Q_DECLARE_METATYPE(ClientState);

int MainWindow::count=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    qRegisterMetaType<Event>();
    qRegisterMetaType<ClientState>();


    QFile file ("clientConfig.txt");
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);

    adr=in.readLine();
    port=in.readLine().toInt();

    file.close();

    address.setAddress(adr);

    ui->setupUi(this);
    ui->connecIndic->setPixmap(QPixmap("noconnect.png"));
    ui->connecSyncIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("");

    pMyDB=new MyDatabase();
    pMyDB->createConnection();
    pMyDB->createTable();
    //pMyDB->dbInsert("test user");
    //pMyDB->insertTestMessages();
    pMyDB->printTable();

    clientState.SetLastEvents(pMyDB);
    clientState.SetRooms(pMyDB);

    QList<QString> textList=pMyDB->takeMessages();
    for(QString & x:textList)
    ui->chatBrowser->append(x);

    rooms=new QSqlTableModel(this, pMyDB->getDBPointer());
    rooms->setTable("Rooms");

    ui->tableView->setModel(rooms);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 this, SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &))                );

}

MainWindow::~MainWindow()
{
    pMyDB->dropTable();
    delete ui;
}

void MainWindow::on_connButton_clicked()
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

void MainWindow::on_disconButton_clicked()
{
    socketPut.close();
}

void MainWindow::slotConnected(){
    ui->connecIndic->setPixmap(QPixmap("connect.png"));
    ui->statusLabel->setText("Connected");
}

void MainWindow::slotDisconnected(){
    ui->connecIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("Disconnected");
}

void MainWindow::on_regButton_clicked()
{
    MyRequest regRequest;
    regRequest.setMethod("POST");
    regRequest.setVersion("HTTP/1.1");
    regRequest.setPath("/reg");

    login=ui->loginEdit->text();
    password=ui->passwordEdit->text();
    qDebug()<<"Login: "<<login<<" Password: "<<password;

    regRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["Login"] = login;
    jsonObject["Password"] = password;
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray regData = document.toJson();

    regRequest.write(regData, true, &socketPut);
}

void MainWindow::on_authButton_clicked()
{
    authorizationFlag=false;
    this->Rooms.clear();
    ui->roomBox->clear();
    ui->chatBrowser->clear();

    MyRequest authRequest;
    authRequest.setMethod("GET");
    authRequest.setVersion("HTTP/1.1");
    authRequest.setPath("/auth");

    /*login=ui->loginEdit->text();
    password=ui->passwordEdit->text();

    authRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["identifier"] = login;
    jsonObject["password"] = password;
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray authData = document.toJson();*/

    authRequest.write(QByteArray(), true, &socketPut);
}

void MainWindow::readFromServer()
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

        login=ui->loginEdit->text();
        password=ui->passwordEdit->text();

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
        if (!authorizationFlag)
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
                Rooms.append(el.toObject()["id"].toString());
                RoomNames.insert(el.toObject()["login"].toString(), el.toObject()["id"].toString().toInt());
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

                ui->roomBox->addItem(el.toObject()["login"].toString());
            }

            /*for(auto& el:RoomNames)
            {
                ui->roomBox->addItem(RoomNames.key(el));
            }*/

            clientState.SetToken(authorizationToken);
            clientState.SetRooms(pMyDB);
            clientState.SetLastEvents(pMyDB);
            rooms->select();

            authorizationFlag=true;
            if (authorizationToken=="")
                authorizationFlag=false;
        }
        if ((QString::localeAwareCompare(presponse.returnStatus(), "200")==0)&&(authorizationFlag==true)){
            ui->statusLabel->setText("Authorized");
        }
        else{
            ui->statusLabel->setText("Unauthorized");
        }
    }
    else if(QString::localeAwareCompare(reqType, "Registration")==0){
        if (QString::localeAwareCompare(presponse.returnStatus(), "200")==0){
            ui->statusLabel->setText("Registered");
        }
        else{
            ui->statusLabel->setText("Unregistered");
        }
    }
}
void MainWindow::on_sendButton_clicked()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/send");

    QString text=ui->messageEdit->text();

    sendRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["message"] = text;

    QString room=ui->roomBox->currentText();
    jsonObject["room_id"] = QString::number(*RoomNames.find(room));

    jsonObject["login"]=login;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, &socketPut);
}

void MainWindow::incomingMessageMWSlot(Event message)
{
    pMyDB->insertMessage(message.Content, message.Id, message.IdRoom);

    clientState.SetLastEvents(pMyDB);

    if(contactLogin!=""){
    QList<QString> textList=pMyDB->takeMessages(contactLogin);
    ui->chatBrowser->clear();

    for(QString & x:textList)
        ui->chatBrowser->append(x);}

    emit clientStateChanged(clientState);



    /*QList<QString> textList=pMyDB->takeMessages();
    ui->chatBrowser->clear();

    for(QString & x:textList)
        ui->chatBrowser->append(x);*/
}

void MainWindow::syncConnected()
{
    ui->connecSyncIndic->setPixmap(QPixmap("connect.png"));
    ui->statusLabel->setText("Sync");
}

void MainWindow::syncDisconnected()
{
    ui->connecSyncIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("Nosync");
}

/*void MainWindow::on_pushButton_clicked()
{
    emit stopSync();
}*/

void MainWindow::on_roomBox_activated(const QString &arg1) //arg - Логин, а нужно Id комнаты, в токен д передаваться не arg а токен
{
    if (count!=0)
    {
        thread->exit();
    }
    count++;

    //pMyDB->dropTable();
    //pMyDB->createTable();
    int lastId=pMyDB->selectMessageId()+1;
    QString arg=QString::number(*RoomNames.find(arg1));
    QString authToken=authorizationToken.mid(0, 16);
    authToken+="_"+arg;

    thread = new SyncThread(clientState, authToken, login, this, lastId);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
    QObject::connect(thread, SIGNAL(syncConnected()), this, SLOT(syncConnected()));
    QObject::connect(thread, SIGNAL(syncDisconnected()), this, SLOT(syncDisconnected()));
    QObject::connect(thread, SIGNAL(incomingMessageEventSync(Event)), this, SLOT(incomingMessageMWSlot(Event)));
    QObject::connect(this, SIGNAL(clientStateChanged(ClientState)), thread, SLOT(clientStateChangedSLOT(ClientState)));
    QObject::connect(this, SIGNAL (stopSync()), thread, SLOT(stopSyncSlot()));
}

void MainWindow::on_find_clicked()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/create_room");

    QJsonObject jsonObject;
    jsonObject["creatorLogin"] = login;
    jsonObject["clientLogin"]=ui->findUserEdit->text();

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, &socketPut);
}

void MainWindow::printSslErrors(const QList<QSslError> & erList)
{
   for (auto el: erList)
   {
       qDebug()<<el;
   }
}

void MainWindow::on_actionExit_2_triggered()
{
    pMyDB->dropTable();
    this->close();
}


void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    contactLogin=ui->tableView->model()->data(index).toString();

    QList<QString> textList=pMyDB->takeMessages(contactLogin);
    ui->chatBrowser->clear();

    for(QString & x:textList)
        ui->chatBrowser->append(x);

    pMyDB->printTable();


}
