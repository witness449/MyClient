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


    void insertMessage(Event e); //Внести сообщение в таблицу Events
    void insertRoom(Room r); //Внести чат в таблицу Rooms
    void insertContact(Contact c); //Внести контакт в таблицу Contacts
    int selectRoomId(Room r); //Получить идентифкатор чата
    QString selectRoomName(int roomId); //Получить имя чата (вообще имя контакта) по идентифкатору чата
    void deleteRoom(Room r); //Удалить чат


    QList<QString> const takeMessages(QString); //Получить все сообщения по имени чата
    QMap<int, bool> const selectRooms(); //Получить словарь: ключ идентифкатор чата, значение признак активности
    QMap<int, int> const selectTopMessages(); //получить словарь: ключ идентифкатор чата, значение - идентификатор последнего сообщения в чате

    //Получить указатель на базу данных
    QSqlDatabase getDBPointer()
    {
        return myDB;
    }
   };

#endif // MYDATABASE_H
