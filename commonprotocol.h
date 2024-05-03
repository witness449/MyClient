#ifndef COMMONPROTOCOL_H
#define COMMONPROTOCOL_H

#include "myrequest.h"
#include "myresponse.h"
#include <QSslSocket>
#include "account.h"

class CommonProtocol
{
protected:
    MyRequest request;
    MyResponse* response;
    QSslSocket* socket;

public:
    virtual void sendRequest()=0;
    virtual void readResponse(MyResponse*)=0;

};

#endif // COMMONPROTOCOL_H
