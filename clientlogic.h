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
    Registrator registrator;
    Authorizer authorizer;

    Q_OBJECT
public:
    explicit ClientLogic(MyDatabase* pD, QObject *parent = 0);

signals:
    void refreshRooms();
    void clientStateChanged(ClientState);

public slots:
    void ConnectSlot();
    //Слоты для оргазиции подключения и отправки сообщений
    void slotConnected();
    void slotDisconnected();
    void readFromServer();
    void ToRegisterSlot(QString, QString);
    void ToAuthentificateSlot(QString, QString);
    void incomingMessageMWSlot(Event event);
    void incomingRoomMWSlot(Room, QString);
    void outcomingRoomMWSlot(Room, QString);
    void ToSendSlot(QString, QString);
    void ToFindSLOT(QString);
    void ToBanSLOT(QString);
    void ToUnBanSLOT(QString);
    //void printSslErrors(const QList<QSslError> & erList);
    void StartSynchronization();


};

#endif // CLIENTLOGIC_H
