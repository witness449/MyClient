#ifndef SYNC_H
#define SYNC_H

#include <QThread>
#include <QTcpSocket>
#include <QhostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include "clientstate.h"
#include "event.h"

//Поток в котором реализована синхронизация сообщений текущего чата и соответствующих данных на сервере
class SyncThread:public QThread
{
    Q_OBJECT
public:
    //В конструктор передается токен авторизации и id последнего загруженного сообщения в текущем чате
    explicit SyncThread(ClientState &clientState, QString authToken, QString login,  QObject *parent = 0, int lastId=0);
    void run();

private slots:
     void incomingMessageEventSlot(Event);
     void workerConnectedSlot();
     void workerDisconnectedSlot();
     void clientStateChangedSLOT(ClientState);
     void incomingRoomSlot(Room, QString);


signals:
     void incomingMessageEventSync(Event);
     void syncConnected();
     void syncDisconnected();
     void clientStateChangedFromSync(ClientState);


     void incomingRoomSync(Room, QString);


private:
    //QTcpSocket* socketSync;
    int lastId;
    QString authorizationToken;
    QString login;
    ClientState clientState;
};

#endif // SYNC_H
