#ifndef SENDER_H
#define SENDER_H
#include "commonprotocol.h"
#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

class Sender:public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    Sender(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);
    QString contactLogin;
    QString txt;
    QString status;
};

#endif // SENDER_H
