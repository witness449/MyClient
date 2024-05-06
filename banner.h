#ifndef BANNER_H
#define BANNER_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

//Класс обеспечивающий взаимодействие по протоколу внесения в черный список
class Banner:public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    Banner(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);
    QString contactLogin;
    QString status;
};
#endif // BANNER_H
