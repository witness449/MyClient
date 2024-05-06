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
#include "netconfig.h"

//Класс обеспечивающий диспетчиризацию запросов и отевтов от сервера
class ClientLogic : public QObject
{
    //Структура с сетевыми настройками
    NetConfig netconfig;
    //Указатель на класс взаимодействия с БД
    MyDatabase* pMyDB;
    //Объект, хранящий сведения об актуальном состоянии аккаунта
    ClientState clientState;
    //Сокет
    QSslSocket socketPut;
    //Объект со сведениями ответа от сервера
    MyResponse* presponse;
    //Объект со сведениями об аккаунте
    Account account;
    //Флаг, отражающий состояние клиента в сысле авторизации
    bool authorizationgFlag=false;
    //Отдельный поток в котором происходит синхронизация с сервером
    SyncThread* thread;
    static int count;
    //Указатель на объект, обеспечивающий регистрацию
    Registrator* registrator;
    //Указатель на объект обеспечивающий аутентификацию-авторизацию
    Authorizer* authorizer;
    //Указатель на объект, обеспечивающий приглашение нового контакта
    ContactCreator* contactCreator;
    //Указатель на объект, обеспечивающий отправку нового сообщения
    Sender* sender;
    //Указатель на объект, обеспевиаюющий выход из чата
    Leaver* leaver;
    //Указатель на объект, обеспечивающий внесение в черный список
    Banner* banner;
    //Указатель на объект, обеспечивающий удаление из черного списка
    Unbanner* unbanner;
    //Метод установки актуального состояния
    void const setClientState();



    Q_OBJECT
public:
    explicit ClientLogic(MyDatabase* pD, QObject *parent = 0);

signals:
    //Сигнал о том, что необходимо обновить состояние комнаты
    void refreshRooms();
    //Сигнал о том, что произошло изменение состояния
    void clientStateChanged(ClientState);
    //Сигнал, пересылающий HTTP статус ответа сервера
    void emitStatus(QString status);
    //Сигнал, пересылающий сообщение
    void emitMessage(Event message);
    //Сигнал о том,ч то установлено соединение с сервером
    void emitConnected();
    //Сигнал о том, что произошло отключение от сервера
    void emitDisconnected();

public slots:
    void connectSlot();
    //Слоты для оргазиции подключения и отправки сообщений
    void slotConnected();
    void slotDisconnected();
    //Слот анализа ответа от сервера
    void readFromServer();
    //Слот, вызываемый при получении команлы на регистрацю
    void toRegisterSlot(QString, QString);
    //Слот, вызываемый при получении команды на аутентифкацию
    void toAuthentificateSlot(QString, QString);
    //Слот вызываемый при получении нового сообщения
    void incomingMessageMWSlot(Event event);
    //Слоты вызываемый при изменении состояния комнаты (например, что новый контак добавлен или удален)
    void incomingRoomMWSlot(Room, QString);
    void outcomingRoomMWSlot(Room, QString);
    //Слот, вызываемый при получении команды на отправку сообщения
    void toSendSlot(QString, QString);
    //Слот, вызываемый при получении команды на поиск нвого контакта, чтобы его внести в список
    void toFindSLOT(QString);
    //Слот, вызываемый при получении сигнала на выход из чата
    void toLeaveSlot(QString);
    //слот, вызываемый при получении сигнала о внесении контакта в черный список
    void toBanSLOT(QString);
    //слот, вызываемый при получении сигнала об удалении контакта из черного списка
    void toUnBanSLOT(QString);
    //void printSslErrors(const QList<QSslError> & erList);
    //Слот, вызываемый для запуска синхронизирующего потока
    void startSynchronization();
    //Слот, вызываемый при получении сигнала об отключении от сервера
    void disConnectSlot();
    //Слот, вызываемый при получении команды о выходе из учетной записи
    void slotLogout();
};



#endif // CLIENTLOGIC_H
