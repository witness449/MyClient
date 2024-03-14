#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QSslSocket>
#include <QMap>
#include "myresponse.h"
#include "syncthread.h"
#include "mydatabase.h"
#include "clientstate.h"
#include <QSqlTableModel>
#include <QItemSelection>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Слоты элементов управления
    void on_connButton_clicked(); //Действия для соединения с сервером
    void on_disconButton_clicked(); //Действия для отключения от сервера
    void on_regButton_clicked(); //Действи по кнопке "Регистрация"
    void on_authButton_clicked(); //Действия по кнопке "Авторизация"
    void on_sendButton_clicked(); //Действи япо кнопке "Отправка сообщения"
    void on_roomBox_activated(const QString &arg1); //Действия при активации выбора чата

    //Слоты для оргазиции подключения и отправки сообщений
    void slotConnected();
    void slotDisconnected();
    void readFromServer();


    //Слоты для оргазиции синхронизации с сервером
    void incomingMessageMWSlot(Event event);
    void syncConnected();
    void syncDisconnected();

    void on_find_clicked();

    void printSslErrors(const QList<QSslError> & erList);

    void on_actionExit_2_triggered();

    void on_tableView_activated(const QModelIndex &index);

signals:
    void stopSync();
    void clientStateChanged(ClientState);

private:
    //Переменные для организации соединения
    QString adr;
    qint16 port;
    QHostAddress address;

    Ui::MainWindow *ui;
    QSslSocket socketPut; //Сокет для соединения с сервером и отправки сообщений
    MyDatabase* pMyDB; //Указатель набазу данных
    SyncThread* thread; //Поток для организации синхронизации

    QString login; //Логин пользователя
    QString password; //Пароль пользователя
    QString authorizationToken; //Токен авторизации
    int authorizationFlag=false;

    QList<QString> Rooms; //Список чатов
    QMap <QString, int>RoomNames;

    MyResponse& presponse= *new MyResponse(); //Костыль. Либо изза кривых рук либо кривой библиотеки

    ClientState clientState;
    QSqlTableModel* rooms;

    static int count;
};

#endif // MAINWINDOW_H
