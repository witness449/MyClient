#include "clientstate.h"

ClientState::ClientState()
{
}


QString const ClientState::getToken()
{
    return accessToken;
}

QMap<int, bool> const ClientState::getActiveRooms()
{
    return rooms;
}
QMap<int, int> const ClientState::getLastEvents()
{
    return events;
}


void ClientState::setToken(QString token)
{
    accessToken=token;
}

void ClientState::setRooms(MyDatabase *pMDB)
{
    rooms=pMDB->selectRooms();
}

void ClientState::setLastEvents(MyDatabase *pMDB)
{
    events=pMDB->selectTopMessages();
}

ClientState& ClientState::operator=(const ClientState& cs)
{
    accessToken=cs.accessToken;
    rooms=cs.rooms;
    events=cs.events;

    return *this;

}

QJsonObject ClientState::toJson()
{
    QJsonObject objectRes;
    objectRes["AccessToken"]=accessToken;
    QJsonArray arRooms;
    QJsonArray arEvents;

    for (auto i=rooms.begin(); i!=rooms.end(); i++)
    {
        QJsonObject object;
        object["Id"]=i.key();
        object["IsActive"]=i.value();
        arRooms.append(object);
    }

    for (auto i=events.begin(); i!=events.end(); i++)
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



