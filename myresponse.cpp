#include "myresponse.h"
#include <QHostAddress>

MyResponse::MyResponse()
{
    maxSize=1000000;
    maxMultiPartSize=1000000;
    currentSize=0;
    expectedBodySize=0;
    status=waitForRequest;
}

void MyResponse::readResponse(QTcpSocket& socket)
{

    #ifdef SUPERVERBOSE
        qDebug("HttpRequest: read request");
    #endif
    int toRead=maxSize-currentSize+1; // allow one byte more to be able to detect overflow
    QByteArray dataRead = socket.readLine(toRead);
    currentSize += dataRead.size();
    lineBuffer.append(dataRead);
    if (!lineBuffer.contains("\r\n"))
    {
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: collecting more parts until line break");
        #endif
        return;
    }
    QByteArray newData=lineBuffer.trimmed();
    lineBuffer.clear();
    if (!newData.isEmpty())
    {
        qDebug("HttpRequest: from %s: %s",qPrintable(socket.peerAddress().toString()),newData.data());
        QList<QByteArray> list=newData.split(' ');
        if (list.count()!=3 || !list.at(0).contains("HTTP"))
        {
            qWarning("HttpRequest: received broken HTTP request, invalid first line");
            status=abort_broken;
        }
        else
        {
            //method=list.at(0).trimmed();
            //path=list.at(1);
            //version=list.at(2);
            responseStatus=list.at(1);
            peerAddress = socket.peerAddress();
            status=waitForHeader;
         }
    }
}

void MyResponse::readFromSocket(QTcpSocket& socket)
{
    //Q_ASSERT(status!=complete);
    if (status==waitForRequest)
    {
        readResponse(socket);
    }
    else if (status==waitForHeader)
    {
        readHeader(socket);
    }
    else if (status==waitForBody)
    {
        readBody(socket);
    }
    /*if ((boundary.isEmpty() && currentSize>maxSize) || (!boundary.isEmpty() && currentSize>maxMultiPartSize))
    {
        qWarning("HttpRequest: received too many bytes");
        status=abort_size;
    }
    if (status==complete)
    {
        // Extract and decode request parameters from url and body
        decodeRequestParams();
        // Extract cookies from headers
        extractCookies();
    }*/
    //}
}

QByteArray MyResponse::returnStatus()
{
    return responseStatus;
}

void MyResponse::readHeader(QTcpSocket& socket)
{
    int toRead=maxSize-currentSize+1; // allow one byte more to be able to detect overflow
    QByteArray dataRead = socket.readLine(toRead);
    currentSize += dataRead.size();
    lineBuffer.append(dataRead);
    if (!lineBuffer.contains("\r\n"))
    {
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: collecting more parts until line break");
        #endif
        return;
    }
    QByteArray newData=lineBuffer.trimmed();
    lineBuffer.clear();
    int colon=newData.indexOf(':');
    if (colon>0)
    {
        // Received a line with a colon - a header
        currentHeader=newData.left(colon).toLower();
        QByteArray value=newData.mid(colon+1).trimmed();
        headers.insert(currentHeader,value);
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: received header %s: %s",currentHeader.data(),value.data());
        #endif
    }
    else if (!newData.isEmpty())
    {
        // received another line - belongs to the previous header
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: read additional line of header");
        #endif
        // Received additional line of previous header
        if (headers.contains(currentHeader)) {
            headers.insert(currentHeader,headers.value(currentHeader)+" "+newData);
        }
    }
    else
    {
        // received an empty line - end of headers reached
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: headers completed");
        #endif
        // Empty line received, that means all headers have been received
        // Check for multipart/form-data
        QByteArray contentType=headers.value("content-type");
        if (contentType.startsWith("multipart/form-data"))
        {
            int posi=contentType.indexOf("boundary=");
            if (posi>=0) {
                boundary=contentType.mid(posi+9);
                if  (boundary.startsWith('"') && boundary.endsWith('"'))
                {
                   boundary = boundary.mid(1,boundary.length()-2);
                }
            }
        }
        QByteArray contentLength=headers.value("content-length");
        if (!contentLength.isEmpty())
        {
            expectedBodySize=contentLength.toInt();
        }
        if (expectedBodySize==0)
        {
            #ifdef SUPERVERBOSE
                qDebug("HttpRequest: expect no body");
            #endif
            status=complete;
        }
        else if (boundary.isEmpty() && expectedBodySize+currentSize>maxSize)
        {
            qWarning("HttpRequest: expected body is too large");
            status=abort_size;
        }
        else if (!boundary.isEmpty() && expectedBodySize>maxMultiPartSize)
        {
            qWarning("HttpRequest: expected multipart body is too large");
            status=abort_size;
        }
        else {
            #ifdef SUPERVERBOSE
                qDebug("HttpRequest: expect %i bytes body",expectedBodySize);
            #endif
            status=waitForBody;
        }
    }
}

MyResponse::ResponseStatus MyResponse::getStatus()
{
    return status;
}

void MyResponse::setStatus(ResponseStatus st)
{
    status=st;
}

void MyResponse::readBody(QTcpSocket& socket)
{
    //Q_ASSERT(expectedBodySize!=0);
    //if (boundary.isEmpty())
    //{
        // normal body, no multipart
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: receive body");
        #endif
        int toRead=expectedBodySize-bodyData.size();
        QByteArray newData=socket.read(toRead);
        currentSize+=newData.size();
        bodyData.append(newData);
        if (bodyData.size()>=expectedBodySize)
        {
            status=complete;
        }
    //}
    /*else
    {
        // multipart body, store into temp file
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: receiving multipart body");
        #endif
        // Create an object for the temporary file, if not already present
        if (tempFile == nullptr)
        {
            tempFile = new QTemporaryFile;
        }
        if (!tempFile->isOpen())
        {
            tempFile->open();
        }
        // Transfer data in 64kb blocks
        qint64 fileSize=tempFile->size();
        qint64 toRead=expectedBodySize-fileSize;
        if (toRead>65536)
        {
            toRead=65536;
        }
        fileSize+=tempFile->write(socket->read(toRead));
        if (fileSize>=maxMultiPartSize)
        {
            qWarning("HttpRequest: received too many multipart bytes");
            status=abort_size;
        }
        else if (fileSize>=expectedBodySize)
        {
        #ifdef SUPERVERBOSE
            qDebug("HttpRequest: received whole multipart body");
        #endif
            tempFile->flush();
            if (tempFile->error())
            {
                qCritical("HttpRequest: Error writing temp file for multipart body");
            }
            parseMultiPartFile();
            tempFile->close();
            status=complete;
        }
    }*/
}

QByteArray MyResponse::getBody()
{
    return bodyData;
}

QByteArray const MyResponse:: findHeader (const QByteArray& header)
{
    auto i = headers.find(header);
    if (i!=headers.end())
    {
        return i.value();
    }
    else
    {
        return QByteArray();
    }
}

void MyResponse::clear()
{
    maxSize=1000000;
    maxMultiPartSize=1000000;
    currentSize=0;
    expectedBodySize=0;
    status=waitForRequest;
    this->setStatus(MyResponse::waitForRequest);



    lineBuffer.clear();
    currentHeader.clear();
    boundary.clear();
    bodyData.clear();

    method.clear();
    path.clear();
    version.clear();
    peerAddress.clear();

    headers.clear();



}

