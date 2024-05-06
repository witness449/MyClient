#ifndef AUTHORIZER_H
#define AUTHORIZER_H
#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

//Класс обеспечивающий взаимодействие по протоколу аутентификации-авторизации
class Authorizer: public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;

public:
    Authorizer(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);
};

#endif // AUTHORIZER_H
