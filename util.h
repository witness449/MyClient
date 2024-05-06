#ifndef UTIL_H
#define UTIL_H

#include<QByteArray>
#include<QTcpSocket>

//используемые глобальные функции

//Утилита записи в сокет объекта типа QByteArray
bool WriteToSocketAny(QByteArray data, QTcpSocket* socket);
//Парсинг токена авторизации
QMap<int, QString> TokenParse(QString& accessToken, QString &login);



#endif // UTIL_H
