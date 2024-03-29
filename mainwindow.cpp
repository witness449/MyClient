#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myrequest.h"
#include "myresponse.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

int MainWindow::count=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
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

    QList<QString> textList=pMyDB->takeMessages();
    for(QString & x:textList)
    ui->chatBrowser->append(x);
}

MainWindow::~MainWindow()
{
    pMyDB->dropTable();
    delete ui;
}

void MainWindow::on_connButton_clicked()
{
    if ((socketPut.state()!=QAbstractSocket::ConnectedState)||(socketPut.state()!=QAbstractSocket::ConnectingState)){ //НЕ ПРОВЕРЯЕТ!!!
        socketPut.connectToHost(address, port);
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

    login=ui->loginEdit->text();
    password=ui->passwordEdit->text();

    authRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["Login"] = login;
    jsonObject["Password"] = password;
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray authData = document.toJson();

    authRequest.write(authData, true, &socketPut);
}

void MainWindow::readFromServer()
{
    MyResponse& presponse=*new MyResponse();

    while (socketPut.bytesAvailable() &&
           presponse.getStatus()!=MyResponse::complete &&
           presponse.getStatus()!=MyResponse::abort_size &&
           presponse.getStatus()!=MyResponse::abort_broken)

    {
        presponse.readFromSocket(socketPut);
    }

    QByteArray reqType=presponse.findHeader("request-type");
    if(QString::localeAwareCompare(reqType, "Authorization")==0)
    {
        if (!authorizationFlag)
        {
            QByteArray bodyData(presponse.getBody());
            QJsonDocument doc=QJsonDocument::fromJson(bodyData);
            QJsonObject buffer=doc.object();

            authorizationToken=buffer["Authorization_token"].toString();

            int i=1;
            while(buffer[QString::number(i)+"Room"].toString()!=""){
                Rooms.append(buffer[QString::number(i)+"Room"].toString());
                i++;
            }

            for(auto& el:Rooms)
            {
                ui->roomBox->addItem(el);
            }

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
    sendRequest.setMethod("PUT");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/send");

    QString text=ui->messageEdit->text();

    sendRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["message"] = text;

    QString roomId=ui->roomBox->currentText();
    jsonObject["room_id"] = roomId;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, &socketPut);
}

void MainWindow::incomingMessageMWSlot(QString message)
{
    pMyDB->insertMessage(message);
    QList<QString> textList=pMyDB->takeMessages();
    ui->chatBrowser->clear();

    for(QString & x:textList)
        ui->chatBrowser->append(x);
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

void MainWindow::on_roomBox_activated(const QString &arg1)
{
    if (count!=0)
    {
        thread->exit();
    }
    count++;

    pMyDB->dropTable();
    pMyDB->createTable();
    int lastId=pMyDB->selectMessageId()+1;
    QString arg=arg1;
    QString authToken=authorizationToken.mid(0, 16);
    authToken+="_"+arg;

    thread = new SyncThread(authToken, this, lastId);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
    QObject::connect(thread, SIGNAL(syncConnected()), this, SLOT(syncConnected()));
    QObject::connect(thread, SIGNAL(syncDisconnected()), this, SLOT(syncDisconnected()));
    QObject::connect(thread, SIGNAL(incomingMessageSync(QString)), this, SLOT(incomingMessageMWSlot(QString)));
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
