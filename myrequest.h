#ifndef MYREQUEST_H
#define MYREQUEST_H

#include <QByteArray>
#include <QMap>
#include <QMultiMap>
#include <QHostAddress>
#include <QTcpSocket>

//Даннй класс основан на библиотеке QtWebApp
//Используется для организации запроса к серверу
class MyRequest
{
public:
    MyRequest();
    //Установить http путь
    void setPath(QByteArray);
    //Установить http версию
    void setVersion(QByteArray);
    //Установить http метод
    void setMethod(QByteArray);
    //Добавить заголовок
    void appendHeader(QByteArray, QByteArray);
    //Сформировать запрос в виде массива байтов
    QByteArray formRequest();
    //Отправить запрос
    void write(QByteArray, bool, QTcpSocket*);
    bool writeToSocket(QByteArray, QTcpSocket*);
    void writeHeaders(QTcpSocket*);

private:
    QString method;
    QString path;
    QString version;

    QMultiMap<QByteArray, QByteArray> headers;
    QByteArray bodydata;
    QMap <QByteArray, QByteArray> cookies;
    QHostAddress peer_address;

    bool sentHeaders;
};

#endif // MYREQUEST_H
