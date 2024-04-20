#include "sender.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QMap<int, QString> TokenParse2(QString& accessToken, QString &login)
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

    //QString room=ui->roomBox->currentText();
    Room room;
    room.name=contactLogin;
    //qDebug()<<contactLogin;
    //jsonObject["room_id"] = QString::number(*RoomNames.find(room));
    int roomId=pMyDB->selectRoomId(room);
    jsonObject["room_id"]=QString::number(roomId);
    qDebug()<<QString::number(roomId);

    QMap<int, QString> rooms=TokenParse2(account.accessToken, account.login);
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
