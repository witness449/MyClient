#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSQLError>
#include <QSqlRecord>
#include <QMap>
#include "room.h";
#include "contact.h"
#include "event.h"

//Класс реализующий работу с базой данных клиентского приложения
class MyDatabase
{
private:
    QSqlDatabase myDB;
    void createConnection(); //Установить соединение с базой данных
    void closeConnection(); //Закрыть соединение с базой данных


public:
    void dropTable(); //Удалить таблицы
    void printTable(); //Вывести таблицы в консоль дебаггера
    void createTable(); //Создать таблицы
    MyDatabase();
    ~MyDatabase();


    //void insertClient(QString); //Внести клиента в контакты
     //QString selectClient (QString); //Выбрать клиента

    void insertMessage(Event e); //Внести сообщение в текущий чат
    void insertRoom(Room r);
    void insertContact(Contact c);
    int selectRoomId(Room r);
    QString selectRoomName(int roomId);
    void deleteRoom(Room r);


    //int selectMessageId(); //Получение идентификатора последнего сообщения в текущем чате
    //QList<QString> takeMessages();//Получить сообщения текущего чата
    QList<QString> const takeMessages(QString);
    QMap<int, bool> const selectRooms();
    QMap<int, int> const selectTopMessages();

    QSqlDatabase getDBPointer()
    {
        return myDB;
    }
   };

#endif // MYDATABASE_H
