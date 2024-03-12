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
    QObject::connect(pw, SIGNAL(incomingMessage(QString)), this, SLOT(incomingMessageSlot(QString)));
    QObject::connect(pw, SIGNAL(workerConnected()), this, SLOT (workerConnectedSlot()));
    QObject::connect(pw, SIGNAL(workerDisonnected()), this, SLOT (workerDisconnectedSlot()));
    exec();
}

void SyncThread::incomingMessageSlot(QString message)
{
    QString messageStr=message;
    emit incomingMessageSync(messageStr);
}

void SyncThread::workerConnectedSlot()
{
    emit syncConnected();
}

void SyncThread::workerDisconnectedSlot()
{
    emit syncDisconnected();
}

