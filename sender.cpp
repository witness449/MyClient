#include "sender.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "util.h"

Sender::Sender(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    pMyDB=pDB;
    socket=ps;


}

void Sender::sendRequest()
{
    MyRequest sendRequest;
    sendRequest.setMethod("PUT");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/send");

    QString text=txt;

    sendRequest.appendHeader("Content-Type", "application/json");
    QJsonObject jsonObject;
    jsonObject["body"] = text;

    Room room;
    room.name=contactLogin;
    int roomId=pMyDB->selectRoomId(room);
    jsonObject["room_id"]=QString::number(roomId);
    qDebug()<<QString::number(roomId);

    QMap<int, QString> rooms=TokenParse(account.accessToken, account.login);
    QString roomToken="";
    auto it=rooms.find(roomId);
    if (it!=rooms.end()) roomToken=*it;

    jsonObject["sender_id"]=account.login;
    jsonObject["access_token"]=roomToken;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, socket);
}

void Sender::readResponse(MyResponse* pr)
{
    status=QString(pr->returnStatus());
}
