#ifndef MYRESPONSE_H
#define MYRESPONSE_H

#include <QTcpSocket>
#include <QbyteArray>
#include <QHostAddress>


//Даннй класс основан на библиотеке QtWebApp
//Используется для анализа ответа от серверу

class MyResponse
{
public:
    enum ResponseStatus {waitForRequest, waitForHeader, waitForBody, complete, abort_size, abort_broken};
    MyResponse();
    //Прочесть ответ
    void readResponse(QTcpSocket& socket);
    void readFromSocket(QTcpSocket& socket);
    void readHeader(QTcpSocket&socket);
    void readBody(QTcpSocket& socket);
    //Получить http статус ответа
    QByteArray returnStatus();
    //получить тело ответа
    QByteArray getBody();

    //Получить и установить статус ответа (см Enum )
    ResponseStatus getStatus();
    void setStatus(ResponseStatus);

    //Получить значение заголовка
    QByteArray const findHeader (const QByteArray& header);

    void clear();
private:
    ResponseStatus status;
    int maxSize;
    int currentSize;
    QByteArray lineBuffer;
    QByteArray currentHeader;
    QByteArray boundary;
    int expectedBodySize;
    int maxMultiPartSize;
    QByteArray bodyData;

    QByteArray responseStatus;
    QByteArray method;
    QByteArray path;
    QByteArray version;
    QHostAddress peerAddress;

    QMultiMap<QByteArray, QByteArray> headers;
};
#endif // MYRESPONSE_H
