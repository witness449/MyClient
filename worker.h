#ifndef WORKER_H
#define WORKER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QHostAddress>
#include <QTimer>
#include <QSslSocket>
#include "myresponse.h"

//Потоковая функция объекта типа SyncThread
//Обеспечивает цикл синхронизации текущего чата с данными на сервере при помощи long-lived GET запроса
class Worker:public QObject
{
    Q_OBJECT
public:
    explicit Worker(QString authToken, QString login,  QObject *parent = 0, int last_Id=0);
private slots:
    void slotSyncDisconnected();
    void readFromServer();
    void startSyncSlot();
    void startWorkerSlot();
    void connectSlot();

    void slotSyncConnected();

signals:
    void startSync();
    void startWorker();
    void incomingMessage(QString);
    void ToConnect();
    void workerConnected();
    void workerDisconnected();

private:
    QSslSocket* socketSync;
    QJsonObject buffer;
    QByteArray bytebuffer;
    int lastId;
    QString authorizationToken;

    qint16 port;
    QString adr;
    QHostAddress adress;
    QString login;

    MyResponse* presponse;
};

#endif // WORKER_H
