#ifndef LEAVER_H
#define LEAVER_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

//Класс, обеспечивающий взаимодействие с сервером по протоколу выхода из чата
class Leaver:public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    Leaver(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);
    //идентифктор контакта с которым ведется переписка в чате
    QString contactLogin;
    QString status;
};

#endif // LEAVER_H
