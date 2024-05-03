#include "leaver.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QMap<int, QString> TokenParse3(QString& accessToken, QString &login)
{
    QStringList pieces=accessToken.split(" ");
    login=pieces[0];
    QMap<int, QString> roomsTokens;
    for (int i=1; i<pieces.size(); i+=2)
    {
         roomsTokens.insert(pieces[i].toInt(), pieces[i+1]);
    }
    return roomsTokens;
}


Leaver::Leaver(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    socket=ps;
    pMyDB=pDB;
}


void Leaver::sendRequest()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/leave");


    Room r;
    r.name=contactLogin;
    int roomId=pMyDB->selectRoomId(r);

    QMap<int, QString> rooms=TokenParse3(account.accessToken, account.login);
    QString roomToken="";
    auto it=rooms.find(roomId);
    if (it!=rooms.end()) roomToken=*it;

    QJsonObject jsonObject;
    jsonObject["creator_id"] = account.login;
    jsonObject["room_id"]=QString::number(roomId);
    jsonObject["access_token"]=roomToken;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, socket);

}

void Leaver::readResponse(MyResponse* pr)
{
     status=QString(pr->returnStatus());
}
