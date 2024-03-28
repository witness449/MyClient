#ifndef CLIENTLOGIC_H
#define CLIENTLOGIC_H

#include <QObject>
#include <QHostAddress>
#include "mydatabase.h"
#include "clientstate.h"
#include <QSslSocket>
#include "myresponse.h"
#include "syncthread.h"

class ClientLogic : public QObject
{

    QString adr;
    qint16 port;
    QHostAddress address;
    MyDatabase* pMyDB;
    ClientState clientState;
    QSslSocket socketPut;

    MyResponse& presponse= *new MyResponse();

    QString login;
    QString password;
    QString authorizationToken;

    bool authorizationgFlag=false;

    SyncThread* thread;
    static int count;


    Q_OBJECT
public:
    explicit ClientLogic(MyDatabase* pD, QObject *parent = 0);

signals:
    void AuthPass();
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
    void ToSendSlot(QString, QString);


};

#endif // CLIENTLOGIC_H
