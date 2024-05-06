#include "banner.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "util.h"

Banner::Banner(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    socket=ps;
    pMyDB=pDB;
}


void Banner::sendRequest()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/ban");

    Room r;
    r.name=contactLogin;
    int roomId=pMyDB->selectRoomId(r);

    QMap<int, QString> rooms=TokenParse(account.accessToken, account.login);
    QString roomToken="";
    auto it=rooms.find(roomId);
    if (it!=rooms.end()) roomToken=*it;

    QJsonObject jsonObject;
    jsonObject["user_id"]=contactLogin;
    jsonObject["admin_id"] = account.login;
    jsonObject["room_id"]=QString::number(roomId);
    jsonObject["access_token"]=roomToken;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, socket);

}

void Banner::readResponse(MyResponse* pr)
{
    status=QString(pr->returnStatus());
}
