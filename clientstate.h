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
    QString AccessToken;
    QMap <int, bool> Rooms;
    QMap<int, int> Events;

public:
    ClientState();
    QString GetToken();
    QMap<int, bool> GetActiveRooms();
    QMap<int, int> GetLastEvents();

    void SetToken(QString token);
    void SetRooms(MyDatabase* pMDB);
    void SetLastEvents(MyDatabase* pMDB);

    ClientState& operator=(const ClientState& cs);
    QJsonObject ToJson();

};

#endif // CLIENTSTATE_H
