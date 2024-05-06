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




MainWindow::MainWindow(MyDatabase* pMDB, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->connecIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("");

    pMyDB=pMDB;
    pMyDB->printTable();

    rooms=new QSqlTableModel(this, pMyDB->getDBPointer());
    rooms->setTable("Rooms");

    ui->tableView->setModel(rooms);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 this, SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &))                );

    ui->authButton->setEnabled(false);
    ui->regButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->banButton->setEnabled(false);
    ui->disconButton->setEnabled(false);
    ui->LeaveChatButton->setEnabled(false);
    ui->banButton->setEnabled(false);
    ui->unButton->setEnabled(false);
    ui->find->setEnabled(false);
    ui->findUserEdit->setEnabled(false);
    ui->loginEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->logoutButton->setEnabled(false);

}

MainWindow::~MainWindow()
{
    pMyDB->dropTable();
    delete ui;
}

void MainWindow::on_connButton_clicked()
{
    emit toConnect();

   }

void MainWindow::on_disconButton_clicked()
{
    emit toDisconnect();
}

void MainWindow::slotConnected(){
    ui->connecIndic->setPixmap(QPixmap("connect.png"));
    ui->statusLabel->setText("Connected");
    ui->authButton->setEnabled(true);
    ui->regButton->setEnabled(true);
    ui->disconButton->setEnabled(true);
    ui->loginEdit->setEnabled(true);
    ui->passwordEdit->setEnabled(true);
}

void MainWindow::slotDisconnected(){
    ui->connecIndic->setPixmap(QPixmap("noconnect.png"));
    ui->statusLabel->setText("Disconnected");
    ui->authButton->setEnabled(false);
    ui->regButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->banButton->setEnabled(false);
    ui->disconButton->setEnabled(false);
    ui->LeaveChatButton->setEnabled(false);
    ui->banButton->setEnabled(false);
    ui->unButton->setEnabled(false);
    ui->find->setEnabled(false);
    ui->findUserEdit->clear();
    ui->findUserEdit->setEnabled(false);
    ui->loginEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->logoutButton->setEnabled(false);
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


void MainWindow::on_find_clicked()
{
    emit toFind(ui->findUserEdit->text());
}


void MainWindow::on_actionExit_triggered()
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

}

void MainWindow::authPassSlot()
{
    contactLogin="";
    rooms->select();

    ui->authButton->setEnabled(false);
    ui->regButton->setEnabled(false);
    ui->sendButton->setEnabled(true);
    ui->banButton->setEnabled(true);
    ui->disconButton->setEnabled(true);
    ui->LeaveChatButton->setEnabled(true);
    ui->banButton->setEnabled(true);
    ui->unButton->setEnabled(true);
    ui->find->setEnabled(true);
    ui->findUserEdit->setEnabled(true);
    ui->loginEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->logoutButton->setEnabled(true);

}

void MainWindow::on_banButton_clicked()
{
    emit toBan(ui->findUserEdit->text());
}


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

void MainWindow::receiveMessage(Event message)
{
    QString roomName=pMyDB->selectRoomName(message.idRoom);
    if(roomName==contactLogin&&roomName!=""){
    ui->chatBrowser->append(message.content);
    }
}

void MainWindow::on_logoutButton_clicked()
{
    pMyDB->dropTable();
    pMyDB->createTable();
    ui->chatBrowser->clear();
    rooms->select();
    ui->loginEdit->clear();
    ui->passwordEdit->clear();

    ui->sendButton->setEnabled(false);
    ui->banButton->setEnabled(false);

    ui->LeaveChatButton->setEnabled(false);
    ui->banButton->setEnabled(false);
    ui->unButton->setEnabled(false);
    ui->find->setEnabled(false);
    ui->findUserEdit->setEnabled(false);
    ui->logoutButton->setEnabled(false);
    ui->authButton->setEnabled(true);
    ui->regButton->setEnabled(true);
    ui->disconButton->setEnabled(true);
    ui->loginEdit->setEnabled(true);
    ui->passwordEdit->setEnabled(true);

    ui->passwordEdit->clear();
    ui->loginEdit->clear();

    emit toLogout();
}
