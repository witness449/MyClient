#include "syncthread.h"
#include "myresponse.h"
#include "myrequest.h"
#include "worker.h"


SyncThread::SyncThread(ClientState &cState, QString authToken,  QString log, QObject* parent, int last_Id):QThread(parent)
{
    lastId=last_Id;
    authorizationToken=authToken;
    login=log;
    clientState=cState;
}

void SyncThread::run()
{
    Worker*pw=new Worker(clientState, authorizationToken, login, this, lastId);
    QObject::connect(pw, SIGNAL(incomingMessageEvent(Event)), this, SLOT(incomingMessageEventSlot(Event)));
    QObject::connect(pw, SIGNAL(workerConnected()), this, SLOT (workerConnectedSlot()));
    QObject::connect(pw, SIGNAL(workerDisonnected()), this, SLOT (workerDisconnectedSlot()));
    QObject::connect(this, SIGNAL (clientStateChangedFromSync(ClientState)), pw, SLOT (clientStateChangedWorkerSlot(ClientState)));
    exec();
}

void SyncThread::incomingMessageEventSlot(Event event)
{
    Event ev=event;
    emit incomingMessageEventSync(ev);
}

void SyncThread::workerConnectedSlot()
{
    emit syncConnected();
}

void SyncThread::workerDisconnectedSlot()
{
    emit syncDisconnected();
}

void SyncThread::clientStateChangedSLOT(ClientState cs)
{
    clientState=cs;
    emit clientStateChangedFromSync(clientState);
}

