#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include <QObject>
#include <QHostAddress>
#include "mydatabase.h"
#include "clientstate.h"
#include <QSslSocket>
#include "myresponse.h"
#include "syncthread.h"
#include "account.h"
#include "registrator.h"
#include "authorizer.h"
#include "contactcreator.h"
#include "sender.h"
#include "leaver.h"
#include "banner.h"
#include "unbanner.h"

class ClientLogic : public QObject
{

    QString adr;
    qint16 port;
    QHostAddress address;
    MyDatabase* pMyDB;
    ClientState clientState;
    QSslSocket socketPut;
    MyResponse& presponse= *new MyResponse();

    Account account;
    bool authorizationgFlag=false;
    SyncThread* thread;
    static int count;
    Registrator* registrator;
    Authorizer* authorizer;
    ContactCreator* contactCreator;
    Sender* sender;
    Leaver* leaver;
    Banner* banner;
    Unbanner* unbanner;



    Q_OBJECT
public:
    explicit ClientLogic(MyDatabase* pD, QObject *parent = 0);

signals:
    void refreshRooms();
    void clientStateChanged(ClientState);
    void emitStatus(QString status);

public slots:
    void connectSlot();
    //Слоты для оргазиции подключения и отправки сообщений
    void slotConnected();
    void slotDisconnected();
    void readFromServer();
    void toRegisterSlot(QString, QString);
    void toAuthentificateSlot(QString, QString);
    void incomingMessageMWSlot(Event event);
    void incomingRoomMWSlot(Room, QString);
    void outcomingRoomMWSlot(Room, QString);
    void toSendSlot(QString, QString);
    void toFindSLOT(QString);
    void toLeaveSlot(QString);
    void toBanSLOT(QString);
    void toUnBanSLOT(QString);
    //void printSslErrors(const QList<QSslError> & erList);
    void startSynchronization();


};

#endif // CLIENTLOGIC_H
