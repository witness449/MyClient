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

//Класс пользовательского интерфейса
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MyDatabase* pMDB, QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Слоты элементов управления
    void on_connButton_clicked(); //Действия для соединения с сервером
    void on_disconButton_clicked(); //Действия для отключения от сервера
    void on_regButton_clicked(); //Действи по кнопке "Регистрация"
    void on_authButton_clicked(); //Действия по кнопке "Авторизация"
    void on_sendButton_clicked(); //Действия по кнопке "Отправка сообщения"
    void on_find_clicked(); //Действия по кнопке Find
    void on_actionExit_triggered(); //Действия по пункту меню Exit
    void on_banButton_clicked(); //Действия по кноке Ban
    void on_unButton_clicked(); //Действия по кнопке UnBan
    void on_LeaveChatButton_clicked(); //Действия по кнопке Leave
    void on_logoutButton_clicked(); //Действия по кнопке Logout

    //Слоты сигнализирующие об установлении и потере связи с сервером
    void slotConnected();
    void slotDisconnected();

    //Действия по активации выбранного контакта из списка контактов
    void on_tableView_activated(const QModelIndex &index);
    //Изменение доступности определенных действий при прохождении аутентификации
    void authPassSlot();
    //Слот утсановки статуса в стаутсной строке
    void setStatus(QString);
    //Отображение полуечнного сообщеняи в окне текущего чата
    void receiveMessage(Event);


signals:

    //Сигналы передаваемые объекту ClientLogic
    void toConnect();
    void toRegister(QString, QString);
    void toAuthentificate(QString, QString);
    void toSend(QString, QString);
    void toFind(QString);
    void toLeave(QString);
    void toBan(QString);
    void toUnBan(QString);
    void toDisconnect();
    void toLogout();

private:

    Ui::MainWindow *ui;

    MyDatabase* pMyDB; //Указатель на базу данных

    //Указатель на модель чатов
    QSqlTableModel* rooms;
    //Идентифкатор активного контакта (того контакта с которым пользователь ведет переписку)
    QString contactLogin="";

};

#endif // MAINWINDOW_H
