#include "mainwindow.h"
#include <QApplication>
#include "mydatabase.h"
#include "clientlogic.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyDatabase mDB;
    ClientLogic cl(&mDB);
    MainWindow w(&mDB);
    QObject::connect(&w, SIGNAL (ToConnect()), &cl, SLOT(ConnectSlot()));
    QObject::connect(&w, SIGNAL (ToRegister(QString, QString)), &cl, SLOT(ToRegisterSlot(QString, QString)));
    QObject::connect(&w, SIGNAL (ToAuthentificate(QString, QString)), &cl, SLOT (ToAuthentificateSlot(QString, QString)));
    QObject::connect(&cl, SIGNAL (AuthPass()), &w, SLOT (AuthPassSlot()));
    QObject::connect(&w, SIGNAL(ToSend(QString, QString)), &cl, SLOT(ToSendSlot(QString, QString)));

    w.show();

    return a.exec();
}
