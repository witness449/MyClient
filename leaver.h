#ifndef LEAVER_H
#define LEAVER_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

class Leaver:public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    Leaver(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse&);
    QString contactLogin;
    QString status;
};

#endif // LEAVER_H
