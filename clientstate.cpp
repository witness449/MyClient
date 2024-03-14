#include "clientstate.h"

ClientState::ClientState()
{
}


QString ClientState::GetToken()
{
    return AccessToken;
}

QMap<int, bool> ClientState::GetActiveRooms()
{
    return Rooms;
}
QMap<int, int> ClientState::GetLastEvents()
{
    return Events;
}


void ClientState::SetToken(QString token)
{
    AccessToken=token;
}

void ClientState::SetRooms(MyDatabase *pMDB)
{
    Rooms=pMDB->selectRooms();
}

void ClientState::SetLastEvents(MyDatabase *pMDB)
{
    Events=pMDB->selectTopMessages();
}

ClientState& ClientState::operator=(const ClientState& cs)
{
    AccessToken=cs.AccessToken;
    Rooms=cs.Rooms;
    Events=cs.Events;

    return *this;

}

QJsonObject ClientState::ToJson()
{
    QJsonObject objectRes;
    objectRes["AccessToken"]=AccessToken;
    QJsonArray arRooms;
    QJsonArray arEvents;

    for (auto i=Rooms.begin(); i!=Rooms.end(); i++)
    {
        QJsonObject object;
        object["Id"]=i.key();
        object["IsActive"]=i.value();
        arRooms.append(object);
    }

    for (auto i=Events.begin(); i!=Events.end(); i++)
    {
        QJsonObject object;
        object["IdRoom"]=i.key();
        object["IdEvent"]=i.value();
        arEvents.append(object);
    }

    objectRes["RoomsState"]=arRooms;
    objectRes["EventsState"]=arEvents;

    return objectRes;
}



