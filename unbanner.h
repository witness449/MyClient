#ifndef UNBANNER_H
#define UNBANNER_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

class Unbanner:public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    Unbanner(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);
    QString contactLogin;
    QString status;
};

#endif // UNBANNER_H
