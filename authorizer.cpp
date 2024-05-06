#include "authorizer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Authorizer::Authorizer(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    socket=ps;
    pMyDB=pDB;
}

void Authorizer::sendRequest()
{
    MyRequest authRequest;
    authRequest.setMethod("GET");
    authRequest.setVersion("HTTP/1.1");
    authRequest.setPath("/auth");
    authRequest.write(QByteArray(), true, socket);

}

void Authorizer::readResponse(MyResponse* presponse)
{
    QString reqType=QString(presponse->findHeader("request-type"));
    int type=reqType=="AuthentificationStage1"?0:1;

    switch (type)
    {
        case 0:{
        QByteArray bodyData(presponse->getBody());
        QJsonDocument doc=QJsonDocument::fromJson(bodyData);
        QJsonObject buffer=doc.object();

        MyRequest authRequest;
        authRequest.setMethod("POST");
        authRequest.setVersion("HTTP/1.1");
        authRequest.setPath("/auth");

        authRequest.appendHeader("Content-Type", "application/json");
        QJsonObject jsonObject;
        jsonObject["identifier"] = account.login;
        jsonObject["password"] = account.password;
        jsonObject["session"]=buffer["session"];
        QJsonDocument document=QJsonDocument(jsonObject);
        QByteArray authData = document.toJson();
        authRequest.write(authData, true, socket);
        }
        break;

        case  1:{
        QByteArray bodyData(presponse->getBody());
        QJsonDocument doc=QJsonDocument::fromJson(bodyData);
        QJsonObject buffer=doc.object();

        account.accessToken=buffer["access_token"].toString();
        QJsonArray RoomsArr= buffer["rooms"].toArray();

        int i=1;

        for (auto el:RoomsArr)
        {
            Room room;
            room.id=el.toObject()["id"].toString().toInt();
            room.name=el.toObject()["login"].toString();
            room.isActive=1;

            Contact c;
            c.login=el.toObject()["login"].toString();
            c.idRoom=el.toObject()["id"].toString().toInt();

            pMyDB->insertRoom(room);
            pMyDB->insertContact(c);
            pMyDB->printTable();
        }
        }
        break;
    }
}
