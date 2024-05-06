#ifndef WORKER_H
#define WORKER_H

#include <QTcpSocket>
#include <QJsonObject>
#include <QHostAddress>
#include <QTimer>
#include <QSslSocket>
#include "myresponse.h"
#include "clientstate.h"
#include "event.h"
#include "netconfig.h"

//Потоковая функция объекта типа SyncThread
//Обеспечивает цикл синхронизации текущего чата с данными на сервере при помощи long poll GET запроса
class SynchroLogic:public QObject
{
    Q_OBJECT
public:
    explicit SynchroLogic(ClientState cState, QString authToken, QString login,  QObject *parent = 0, int last_Id=0);
private slots:
    //Слот вызываемый при отключении синхронизующего потока от сервера
    void slotSyncDisconnected();
    //Слот вызываемый для обработки данных от сервера
    void readFromServer();
    //Слот вызываемый при получении сгнала - отправить запрос на севрер по протоколу синхронизации
    void startSyncSlot();
    //Слот вызываемый при получении сигнала - азпустить цикл синхронизации
    void startSynchroLogicSlot();
    //Слот вызываемый при подключении к серверу
    void connectSlot();
    //Слот вызываемый при отключении от сервера
    void slotSyncConnected();
    //Слот вызываемый при получении измененного состояния клиента
    void clientStateChangedSynchroLogicSlot(ClientState);


signals:
    //Сигнал запустить цикл синхронизации
    void startSynchroLogic();
    //Сигнал запустить виток цикла синхронизации
    void startSync();
    //Сигнал испускаемый при получении от сервера нового сообщения
    void incomingMessageEvent(Event);
    //Сигнал испусскаемый при получении от сервера информации о том, что пользователя добавили в чат
    void incomingRoom(Room, QString);
    //Сигнал испускаемый при получении от сервреа информации о том, что пользователя удалили из чата
    void outcomingRoom(Room, QString);
    //Сигнал - запустить подключение к серверу
    void ToConnect();
    //Сигнал испускаемый при подключении к серверу
    void synchroLogicConnected();
    //Сигнал испускаемый при отключении от сервера
    void synchroLogicDisconnected();




private:
    //Переменные для работы с сетью
    NetConfig netconfig;
    QSslSocket* socketSync;
    QJsonObject buffer;
    QByteArray bytebuffer;
    MyResponse* presponse;


    QString login; //Идентифкатор пользовтаеля
    ClientState clientState;//Объект актуального состояния клиента
};



#endif // WORKER_H
