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
    worker.cpp \
    syncthread.cpp \
    clientstate.cpp \
    clientlogic.cpp

HEADERS  += mainwindow.h \
    util.h \
    myrequest.h \
    mydatabase.h \
    myresponse.h \
    worker.h \
    syncthread.h \
    room.h \
    contact.h \
    clientstate.h \
    event.h \
    clientlogic.h

FORMS    += mainwindow.ui

CONFIG +=C++11


win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibcrypto

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include

win32:CONFIG(release, debug|release): LIBS +=-LD:/tools/OpenSSL/vc-win32/lib/ -llibssl

INCLUDEPATH += D:/tools/OpenSSL/vc-win32/include
DEPENDPATH += D:/tools/OpenSSL/vc-win32/include

