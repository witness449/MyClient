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

