#include "myrequest.h"
#include <QString>

MyRequest::MyRequest()
{
    sentHeaders=false;
}

void MyRequest::setPath(QByteArray varpath)
{
    path=varpath;
}
void MyRequest::setVersion(QByteArray varver)
{
    version=varver;
}
void MyRequest::setMethod(QByteArray varmeth)
{
    method=varmeth;
}
void MyRequest::appendHeader(QByteArray varkey, QByteArray varval)
{
   headers.insert(varkey, varval);
}

QByteArray MyRequest::formRequest()
{
    QString buffer=method+" "+path+" "+version+" "+"\r\n";
    return buffer.toLocal8Bit();
}

void MyRequest::write(QByteArray data, bool lastPart, QTcpSocket * socket)
{
    if (sentHeaders==false)
    {
        if (lastPart)
        {
          headers.insert("Content-Length",QByteArray::number(data.size()));
        }
        writeHeaders(socket);
    }
    if (data.size()>0)
    {
       writeToSocket(data, socket);
     }
}

bool MyRequest::writeToSocket(QByteArray data, QTcpSocket* socket)
{

    int remaining=data.size();
    char* ptr=data.data();
    while (socket->isOpen() && remaining>0)
    {
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

void MyRequest::writeHeaders(QTcpSocket* socket)
{
    QByteArray buffer;
    buffer.append(method);
    buffer.append(' ');
    buffer.append(path);
    buffer.append(' ');
    buffer.append("HTTP/1.1 ");
    buffer.append("\r\n");
    foreach(QByteArray name, headers.keys())
    {
        buffer.append(name);
        buffer.append(": ");
        buffer.append(headers.value(name));
        buffer.append("\r\n");
    }
    buffer.append("\r\n");
    writeToSocket(buffer, socket);
    socket->flush();
    sentHeaders=true;
}
