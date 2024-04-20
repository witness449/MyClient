#include "contactcreator.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

ContactCreator::ContactCreator(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    socket=ps;
    pMyDB=pDB;
}

void ContactCreator::sendRequest()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/create_room");

    QJsonObject jsonObject;
    jsonObject["preset"]="private_chat";
    jsonObject["creator_id"] = account.login;
    jsonObject["invite"]=contactLogin;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, socket);
}

void ContactCreator::readResponse(MyResponse* pr)
{
    status=QString(pr->returnStatus());
}
