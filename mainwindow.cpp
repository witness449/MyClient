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
#include <QStandardItem>
#include "mydelegate.h"



MainWindow::MainWindow(MyDatabase* pMDB, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->connecIndic->setPixmap(QPixmap("noconnect.png"));
    ui->connecSyncIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("");

    pMyDB=pMDB;
    pMyDB->printTable();

    //QList<QString> textList=pMyDB->takeMessages();
    /*for(QString & x:textList)
    ui->chatBrowser->append(x);*/

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
    pMyDB->dropTable();//???
    delete ui;
}

void MainWindow::on_connButton_clicked()
{
    emit toConnect();

   }

void MainWindow::on_disconButton_clicked()
{
    //socketPut.close();
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
    QString login=ui->loginEdit->text();
    QString password=ui->passwordEdit->text();
    emit toRegister(login, password);
}

void MainWindow::on_authButton_clicked()
{
    emit toAuthentificate(ui->loginEdit->text(), ui->passwordEdit->text());

}


void MainWindow::on_sendButton_clicked()
{
    QString text=ui->messageEdit->text();
    emit toSend(contactLogin, text);
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


void MainWindow::on_find_clicked()
{
    emit toFind(ui->findUserEdit->text());
}



void MainWindow::on_actionExit_2_triggered()
{
    //pMyDB->dropTable();
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

void MainWindow::authPassSlot()
{
    contactLogin="";

    //ui->tableView->setAlternatingRowColors(true);
    //QPalette bgpal;
    //bgpal.setColor (QPalette::Background, QColor (255, 0 , 0, 255));
    //ui->tableView->setPalette(bgpal);
    //MyDelegate* delegate;
    //ui->tableView->setItemDelegate(delegate);
    rooms->select();

}

void MainWindow::on_banButton_clicked()
{
    emit toBan(ui->findUserEdit->text());
}

/*void MainWindow::incomingRoomMWSlot(Room r, QString s)
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
    rooms->select();


    emit clientStateChanged(clientState);

}*/



/*void MainWindow::incomingMessageMWSlot(Event message)
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
        ui->chatBrowser->append(x);
}*/




void MainWindow::on_unButton_clicked()
{
    emit toUnBan(ui->findUserEdit->text());
}

void MainWindow::setStatus(QString str)
{
    ui->statusLabel->setText(str);
}

void MainWindow::on_LeaveChatButton_clicked()
{
    emit toLeave(contactLogin);
}

void MainWindow::receiveMessage(QString message)
{
    ui->chatBrowser->append(message);
}
