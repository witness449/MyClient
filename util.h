#ifndef UTIL_H
#define UTIL_H

#include<QByteArray>
#include<QTcpSocket>

//Утилита записи в сокет объекта типа QByteArray
bool WriteToSocketAny(QByteArray data, QTcpSocket* socket)
{
    int remaining=data.size();
    char* ptr=data.data();
    while (socket->isOpen() && remaining>0)
    {
        // If the output buffer has become large, then wait until it has been sent.
        if (socket->bytesToWrite()>16384)
        {
            socket->waitForBytesWritten(-1);
        }

        qint64 written=socket->write(ptr,remaining);
        if (written==-1)
        {
          return false;
        }
        ptr+=written;
        remaining-=written;
    }
    return true;
}

#endif // UTIL_H
