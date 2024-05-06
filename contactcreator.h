#ifndef CONTACTCREATOR_H
#define CONTACTCREATOR_H

#include <QString>
#include <QSSlSocket>
#include "myresponse.h"
#include "commonprotocol.h"
#include "mydatabase.h"

//Класс обеспечивающий взаимодействие с сервером по протоколу создания нового контакта
class ContactCreator : public CommonProtocol
{
    MyDatabase* pMyDB;
    Account& account;
public:
    //Идентфикатор контакта
    QString contactLogin;
    QString status;
    ContactCreator(Account& account, QSslSocket* ps, MyDatabase* pMyDB);
    void sendRequest();
    void readResponse(MyResponse*);

};

#endif // CONTACTCREATOR_H
