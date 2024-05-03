#ifndef CONTACTCREATOR_H
#define CONTACTCREATOR_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

class ContactCreator : public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    QString contactLogin;
    QString status;
    ContactCreator(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);

};

#endif // CONTACTCREATOR_H
