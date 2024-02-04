#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSQLError>
#include <QSqlRecord>

//Класс реализующий работу с базой данных клиентского приложения
class MyDatabase
{
private:
    QSqlDatabase myDB;
public:
    MyDatabase();
    void createConnection(); //Установить соединение с базой данных
    void closeConnection(); //Закрыть соединение с базой данных
    void createTable(); //Создать таблицы
    void dropTable(); //Удалить таблицы
    void printTable(); //Вывести таблицы в консоль дебаггера
    void insertClient(QString); //Внести клиента в контакты
    QString selectClient (QString); //Выбрать клиента
    int selectMessageId(); //Получение идентификатора последнего сообщения в текущем чате
     //void insertTestMessages();
    QList<QString> takeMessages();//Получить сообщения текущего чата
    void insertMessage(QString); //Внести сообщение в текущий чат
   };

#endif // MYDATABASE_H
