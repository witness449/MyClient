#ifndef COMMONPROTOCOL_H
#define COMMONPROTOCOL_H

#include "myrequest.h"
#include "myresponse.h"
#include <QSslSocket>
#include "account.h"

//Абстрактный класс (фактически интерфейс) взаимодействия с сервером
class CommonProtocol
{
protected:
    //запрос на сервер
    MyRequest request;
    //ответ от сервера
    MyResponse* response;
    //Сокет
    QSslSocket* socket;

public:
    //Метод отправки запроса
    virtual void sendRequest()=0;
    //Метод получения ответа
    virtual void readResponse(MyResponse*)=0;

};

#endif // COMMONPROTOCOL_H
