#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H
#include <QString>
#include <QMap>
#include "mydatabase.h"
#include <QJsonObject>
#include <QJsonArray>

//Класс описывающий состояние учетной записи
class ClientState
{
private:
    //Токен доступа
    QString accessToken;
    //Словарь с информцаией об активных комнатах
    QMap <int, bool> rooms;
    //СЛоварь с синформацией об идентифкаторах последних сообщенйи в чате
    QMap<int, int> events;

public:
    ClientState();
    //Метод получения токена авторизации
    QString const getToken();
    //Метод получение информации об активных чатах
    QMap<int, bool> const getActiveRooms();
    //Метод получения информцаии о последних сообщениях в чатах
    QMap<int, int> const getLastEvents();

    //Метод утсановки токена авториазции
    void setToken(QString token);
    //Метод формирования словаря активных чатов
    void setRooms(MyDatabase* pMDB);
    //Метод формирования словаря последних сообщений в чатах
    void setLastEvents(MyDatabase* pMDB);

    ClientState& operator=(const ClientState& cs);
    //Формирование JSOn объекта отображающего актуальное состояние учетной записи
    QJsonObject toJson();

};

#endif // CLIENTSTATE_H
