#include "syncthread.h"
#include "myresponse.h"
#include "myrequest.h"
#include "synchrologic.h"


SyncThread::SyncThread(ClientState &cState, QString authToken,  QString log, QObject* parent, int last_Id):QThread(parent)
{
    lastId=last_Id;
    authorizationToken=authToken;
    login=log;
    clientState=cState;
}

void SyncThread::run()
{
    SynchroLogic*pw=new SynchroLogic(clientState, authorizationToken, login, this, lastId);
    QObject::connect(pw, SIGNAL(incomingMessageEvent(Event)), this, SLOT(incomingMessageEventSlot(Event)));
    QObject::connect(pw, SIGNAL(synchroLogicConnected()), this, SLOT (synchroLogicConnectedSlot()));
    QObject::connect(pw, SIGNAL(synchroLogicDisonnected()), this, SLOT (synchroLogicDisconnectedSlot()));
    QObject::connect(this, SIGNAL (clientStateChangedFromSync(ClientState)), pw, SLOT (clientStateChangedSynchroLogicSlot(ClientState)));
    QObject::connect(pw, SIGNAL(incomingRoom(Room, QString)), this, SLOT(incomingRoomSlot(Room, QString)));
    QObject::connect(pw, SIGNAL(outcomingRoom(Room, QString)), this, SLOT(outcomingRoomSlot(Room, QString)));


    exec();

}

void SyncThread::incomingMessageEventSlot(Event event)
{
    Event ev=event;
    emit incomingMessageEventSync(ev);
}

void SyncThread::synchroLogicConnectedSlot()
{
    emit syncConnected();
}

void SyncThread::synchroLogicDisconnectedSlot()
{
    emit syncDisconnected();
}

void SyncThread::clientStateChangedSlot(ClientState cs)
{
    clientState=cs;
    emit clientStateChangedFromSync(clientState);
}

void SyncThread::incomingRoomSlot(Room room, QString token)
{
    Room r=room;
    authorizationToken=token;
    emit incomingRoomSync(r, token);
}


 void SyncThread::outcomingRoomSlot(Room room, QString token)
 {
     Room r=room;
     authorizationToken=token;
     emit outcomingRoomSync(r, token);
 }


