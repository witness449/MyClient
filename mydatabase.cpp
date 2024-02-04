#include "mydatabase.h"
#include <QList>

MyDatabase::MyDatabase()
{
}

void MyDatabase::createConnection(){
    myDB=QSqlDatabase::addDatabase("QSQLITE", "MyServerDB connection");
    if(myDB.isValid())
    {
        myDB.setDatabaseName("MyServerDB");
        if(myDB.open())
            qDebug()<<"Database is connected";
        else
            qDebug()<<myDB.lastError();
     }
    else
        qDebug()<<myDB.lastError();
}

void MyDatabase::closeConnection(){
    myDB.close();
    qDebug()<<"Database is closed";
}

void MyDatabase::createTable(){
    if(myDB.isValid()){
    QSqlQuery query(myDB);
    QString create ="CREATE TABLE Clients (id INTEGER  PRIMARY KEY, login  VARCHAR)";
    qDebug()<<"Create table status: "<<query.exec(create);
    qDebug()<<myDB.tables();

    QSqlQuery query2(myDB);
    QString create2 ="CREATE TABLE TestMessagess (id INTEGER  PRIMARY KEY, text  VARCHAR)";
    qDebug()<<"Create table status: "<<query2.exec(create2);
    qDebug()<<myDB.tables();}
    else
        qDebug()<<"db is not valid";
}



void MyDatabase:: dropTable(){
    if(myDB.isValid()){
    QSqlQuery query(myDB);
    QString drop="DROP TABLE Clients";
    query.exec(drop);
    qDebug()<<myDB.tables()<<"Should be only TestMessagess table";

    QSqlQuery query2(myDB);
    QString drop2="DROP TABLE TestMessagess";
    query2.exec(drop2);
    qDebug()<<myDB.tables()<<"Should be no tables";}
    else
        qDebug()<<"db is not valid";
}

void MyDatabase::printTable(){
    if (myDB.isValid()){
    QSqlQuery query(myDB);
    QString select="SELECT * FROM Clients";
    qDebug()<<"Select query status: "<<query.exec(select);
    QSqlRecord rec =query.record();

    int id;
    QString login;
    QString password;

    while (query.next()){
        qDebug()<<"id: "<<query.value(rec.indexOf("id")).toInt()
        <<"login: "<<query.value(rec.indexOf("login")).toString();
        }

    QSqlQuery query2(myDB);
    QString select2="SELECT * FROM TestMessagess";
    qDebug()<<"Select query status: "<<query2.exec(select2);
    QSqlRecord rec2 =query2.record();

    QString text;

    while (query2.next()){
        qDebug()<<"id: "<<query2.value(rec2.indexOf("id")).toInt()
        <<"text: "<<query2.value(rec2.indexOf("text")).toString();
        }
    }
    else
        qDebug()<<"db is not valid";
}

void MyDatabase::insertClient(QString login){
    for(int i=0;i<1; i++){
        if (myDB.isValid()){
            QSqlQuery query(myDB);
            QString insert ="INSERT INTO Clients(login) VALUES ('"+login+"')";
            bool res=query.exec(insert);
            qDebug()<<"Insert query status: "<<res;
            if (!res) qDebug()<<query.lastError();
        }
    }
}

QString MyDatabase::selectClient(QString user)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString selectUser="SELECT * FROM Clients WHERE login='"+user+"'";
        qDebug()<<"Select query status: "<<query.exec(selectUser);
        query.next();
        QSqlRecord rec =query.record();
        QString password=query.value(rec.indexOf("password")).toString();
        qDebug()<<"password: "<<password;
        return password;
    }
}

/*void MyDatabase::insertTestMessages()
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO TestMessagess(text) VALUES ('test message')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        this->printTable();
    }
}*/

QList<QString> MyDatabase::takeMessages(){
    QSqlQuery query2(myDB);
    QString select2="SELECT * FROM TestMessagess";
    qDebug()<<"Select query status: "<<query2.exec(select2);
    QSqlRecord rec2 =query2.record();

    QList<QString> textList;

    while (query2.next()){
        textList.append(query2.value(rec2.indexOf("text")).toString());
        }
    return textList;
}

int MyDatabase::selectMessageId()
{
    QSqlQuery query2(myDB);
    QString select2="SELECT * FROM TestMessagess";
    qDebug()<<"Select query status: "<<query2.exec(select2);
    QSqlRecord rec2 =query2.record();

    int lastId=0;

    while (query2.next()){
        lastId=query2.value(rec2.indexOf("id")).toInt();
        }
    return lastId;
}

void MyDatabase::insertMessage(QString message)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insert ="INSERT INTO TestMessagess(text) VALUES ('"+message+"')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        //this->printTable();
    }
}
