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
    regRequest.setPath("/register");

    regRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["identifier"] = account.login;
    jsonObject["password"] = account.password;
    jsonObject["type"]="m.login.password";
    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray regData = document.toJson();

    regRequest.write(regData, true, socket);
}

void Registrator::readResponse(MyResponse& pr)
{
    status=QString(pr.returnStatus());
}
