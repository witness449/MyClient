#ifndef SYNC_H
#define SYNC_H

#include <QThread>
#include <QTcpSocket>
#include <QhostAddress>
#include <QJsonObject>
#include <QJsonDocument>

//Поток в котором реализована синхронизация сообщений текущего чата и соответствующих данных на сервере
class SyncThread:public QThread
{
    Q_OBJECT
public:
    //В конструктор передается токен авторизации и id последнего загруженного сообщения в текущем чате
    explicit SyncThread(QString authToken,  QObject *parent = 0, int lastId=0);
    void run();

private slots:
     void incomingMessageSlot(QString);
     void workerConnectedSlot();
     void workerDisconnectedSlot();

signals:
     void incomingMessageSync(QString);
     void syncConnected();
     void syncDisconnected();

private:
    //QTcpSocket* socketSync;
    int lastId;
    QString authorizationToken;
};

#endif // SYNC_H
