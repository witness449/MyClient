#ifndef REGISTRATOR_H
#define REGISTRATOR_H
#include "commonprotocol.h"


class Registrator:public CommonProtocol
{
    Account& account;
public:
    Registrator(Account& acc, QSslSocket* ps);
    void sendRequest();
    void readResponse(MyResponse&);
 };

#endif // REGISTRATOR_H
