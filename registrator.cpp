#include "registrator.h"
#include <QJsonObject>
#include <QJsonDocument>

Registrator::Registrator(Account& ac, QSslSocket*  ps):account (ac)
{
    socket=ps;

}

void Registrator::sendRequest()
{
    MyRequest regRequest;
    regRequest.setMethod("POST");
    regRequest.setVersion("HTTP/1.1");
    regRequest.setPath("/reg");

    regRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["Login"] = account.login;
    jsonObject["Password"] = account.password;
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray regData = document.toJson();

    regRequest.write(regData, true, socket);

}

void Registrator::readResponse(MyResponse&)
{
    MyResponse pr;

}
