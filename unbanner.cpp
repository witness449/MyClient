#include "unbanner.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QMap<int, QString> TokenParse5(QString& accessToken, QString &login)
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


Unbanner::Unbanner(Account& ac, QSslSocket* ps, MyDatabase* pDB):account (ac)
{
    socket=ps;
    pMyDB=pDB;
}


void Unbanner::sendRequest()
{
    MyRequest sendRequest;
    sendRequest.setMethod("POST");
    sendRequest.setVersion("HTTP/1.1");
    sendRequest.setPath("/unban");

    QJsonObject jsonObject;
    jsonObject["admin_id"] = account.login;
    jsonObject["user_id"]=contactLogin;

    QJsonDocument document=QJsonDocument(jsonObject);
    QByteArray sendData = document.toJson();

    sendRequest.write(sendData, true, socket);

}

void Unbanner::readResponse(MyResponse* pr)
{
    status=QString(pr->returnStatus());
}
