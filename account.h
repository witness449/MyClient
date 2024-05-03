#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

struct Account
{
    QString login;
    QString password;
    QString accessToken;
};

#endif // ACCOUNT_H
