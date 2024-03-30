#include "mainwindow.h"
#include <QApplication>
#include "mydatabase.h"
#include "clientlogic.h"

Q_DECLARE_METATYPE(Event)
Q_DECLARE_METATYPE(ClientState)
Q_DECLARE_METATYPE(Room)

int main(int argc, char *argv[])
{
    qRegisterMetaType<Event>();
    qRegisterMetaType<ClientState>();
    qRegisterMetaType<Room>();

    QApplication a(argc, argv);
    MyDatabase mDB;
    ClientLogic cl(&mDB);
    MainWindow w(&mDB);
    QObject::connect(&w, SIGNAL (ToConnect()), &cl, SLOT(ConnectSlot()));
    QObject::connect(&w, SIGNAL (ToRegister(QString, QString)), &cl, SLOT(ToRegisterSlot(QString, QString)));
    QObject::connect(&w, SIGNAL (ToAuthentificate(QString, QString)), &cl, SLOT (ToAuthentificateSlot(QString, QString)));
    QObject::connect(&cl, SIGNAL (refreshRooms()), &w, SLOT (AuthPassSlot()));
    QObject::connect(&w, SIGNAL(ToSend(QString, QString)), &cl, SLOT(ToSendSlot(QString, QString)));
    QObject::connect(&w, SIGNAL(ToFind(QString)), &cl, SLOT(ToFindSLOT(QString)));
    QObject::connect(&w, SIGNAL(ToBan(QString)), &cl, SLOT(ToBanSLOT(QString)));
    QObject::connect(&w, SIGNAL(ToUnBan(QString)), &cl, SLOT(ToUnBanSLOT(QString)));

    w.show();

    return a.exec();
}
