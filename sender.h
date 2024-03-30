#ifndef SENDER_H
#define SENDER_H
#include "commonprotocol.h"

class Sender:public CommonProtocol
{
    Account& account;
public:
    Sender(Account& account, QSslSocket* ps);
    void sendRequest();
    void readResponse(MyResponse&);
};

#endif // SENDER_H
