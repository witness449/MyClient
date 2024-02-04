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
    syncthread.cpp

HEADERS  += mainwindow.h \
    util.h \
    myrequest.h \
    mydatabase.h \
    myresponse.h \
    worker.h \
    syncthread.h

FORMS    += mainwindow.ui

CONFIG +=C++11


