#ifndef CLIENTSTATE_H
#define CLIENTSTATE_H
#include <QString>
#include <QMap>
#include "mydatabase.h"
#include <QJsonObject>
#include <QJsonArray>

class ClientState
{
private:
    QString accessToken;
    QMap <int, bool> rooms;
    QMap<int, int> events;

public:
    ClientState();
    QString const getToken();
    QMap<int, bool> const getActiveRooms();
    QMap<int, int> const getLastEvents();

    void setToken(QString token);
    void setRooms(MyDatabase* pMDB);
    void setLastEvents(MyDatabase* pMDB);

    ClientState& operator=(const ClientState& cs);
    QJsonObject toJson();

};

#endif // CLIENTSTATE_H
