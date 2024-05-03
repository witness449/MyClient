#ifndef NETCONFIG_H
#define NETCONFIG_H

#include <QHostAddress>

struct NetConfig
{
    QString adr;
    qint16 port;
    QHostAddress address;
};

#endif // NETCONFIG_H
