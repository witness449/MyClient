#ifndef REGISTRATOR_H
#define REGISTRATOR_H
#include "commonprotocol.h"

//Класс обеспевичающий взаимодействие по протоколу регистрации
class Registrator:public CommonProtocol
{
    Account& account;
public:
    Registrator(Account& acc, QSslSocket* ps);
    void sendRequest();
    void readResponse(MyResponse*);
    QString status;
 };

#endif // REGISTRATOR_H
