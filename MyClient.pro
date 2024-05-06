#-------------------------------------------------
#
# Project created by QtCreator 2023-10-18T12:11:08
#
#-------------------------------------------------

QT       += core gui
QT+=sql
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myrequest.cpp \
    mydatabase.cpp \
    myresponse.cpp \
    syncthread.cpp \
    clientstate.cpp \
    clientlogic.cpp \
    registrator.cpp \
    authorizer.cpp \
    sender.cpp \
    contactcreator.cpp \
    leaver.cpp \
    banner.cpp \
    unbanner.cpp \
    synchrologic.cpp \
    util.cpp

HEADERS  += mainwindow.h \
    util.h \
    myrequest.h \
    mydatabase.h \
    myresponse.h \
    syncthread.h \
    room.h \
    contact.h \
    clientstate.h \
    event.h \
    clientlogic.h \
    commonprotocol.h \
    account.h \
    registrator.h \
    authorizer.h \
    sender.h \
    contactcreator.h \
    leaver.h \
    banner.h \
    unbanner.h \
    netconfig.h \
    synchrologic.h

FORMS    += mainwindow.ui

CONFIG +=C++11


win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibcrypto

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include

win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibssl

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include

