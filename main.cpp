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
    QObject::connect(&w, SIGNAL (toConnect()), &cl, SLOT(connectSlot()));
    QObject::connect(&w, SIGNAL (toRegister(QString, QString)), &cl, SLOT(toRegisterSlot(QString, QString)));
    QObject::connect(&w, SIGNAL (toAuthentificate(QString, QString)), &cl, SLOT (toAuthentificateSlot(QString, QString)));
    QObject::connect(&cl, SIGNAL (refreshRooms()), &w, SLOT (authPassSlot()));
    QObject::connect(&w, SIGNAL(toSend(QString, QString)), &cl, SLOT(toSendSlot(QString, QString)));
    QObject::connect(&w, SIGNAL(toFind(QString)), &cl, SLOT(toFindSLOT(QString)));
    QObject::connect(&w, SIGNAL(toLeave(QString)), &cl, SLOT(toLeaveSlot(QString)));
    QObject::connect(&w, SIGNAL(toBan(QString)), &cl, SLOT(toBanSLOT(QString)));
    QObject::connect(&w, SIGNAL(toUnBan(QString)), &cl, SLOT(toUnBanSLOT(QString)));
    QObject::connect(&cl, SIGNAL(emitStatus(QString)), &w, SLOT(setStatus(QString)));
    QObject::connect(&cl, SIGNAL (emitMessage(Event)), &w, SLOT(receiveMessage(Event)));
    QObject::connect(&cl, SIGNAL (emitConnected()), &w, SLOT(slotConnected()));
    QObject::connect(&w, SIGNAL (toDisconnect()), &cl, SLOT(disConnectSlot()));
    QObject::connect(&cl, SIGNAL (emitDisconnected()), &w, SLOT(slotDisconnected()));
    QObject::connect(&w, SIGNAL(toLogout()), &cl, SLOT(slotLogout()));

    w.show();

    return a.exec();
}
