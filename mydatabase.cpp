#include "mydatabase.h"
#include <QList>

MyDatabase::MyDatabase()
{
    this->createConnection();
    this->createTable();
}

MyDatabase::~MyDatabase()
{
    this->dropTable();
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

    QSqlQuery query3(myDB);
    QString create3 ="CREATE TABLE Rooms (Id INTEGER PRIMARY KEY, Name  VARCHAR(50) NOT NULL, Alias VARCHAR(50), IsActive BIT NOT NULL)";
    qDebug()<<"Create table status: "<<query3.exec(create3);
    qDebug()<<myDB.tables();

    QSqlQuery query(myDB);
    QString create ="CREATE TABLE Contacts (Id INTEGER IDENTITY (1,1) PRIMARY KEY, Login  VARCHAR, IdRoom INTEGER, FOREIGN KEY (IdRoom) REFERENCES Rooms (Id))";
    qDebug()<<"Create table status: "<<query.exec(create);
    qDebug()<<myDB.tables();

    QSqlQuery query2(myDB);
    QString create2 ="CREATE TABLE Events (Id INTEGER PRIMARY KEY, IdRoom INTEGER NOT NULL, Content NVARCHAR NOT NULL, TimeStamp DATETIME, FOREIGN KEY (IdRoom) REFERENCES Rooms (Id))";
    qDebug()<<"Create table status: "<<query2.exec(create2);
    qDebug()<<myDB.tables();

    }
    else
        qDebug()<<"db is not valid";
};


void MyDatabase:: dropTable(){
    if(myDB.isValid()){
    QSqlQuery query(myDB);
    QString drop="DROP TABLE Contacts";
    query.exec(drop);
    qDebug()<<myDB.tables()<<"Should be only Events";

    QSqlQuery query2(myDB);
    QString drop2="DROP TABLE Events";
    query2.exec(drop2);
    qDebug()<<myDB.tables()<<"Should be no tables";


    QSqlQuery query3(myDB);
    QString drop3="DROP TABLE Rooms";
    query3.exec(drop3);
    qDebug()<<myDB.tables()<<"Should be no tables";}
    else
        qDebug()<<"db is not valid";
}

void MyDatabase::printTable(){//РАСПЕЧАТАТЬ
    if (myDB.isValid()){
    QSqlQuery query(myDB);
    QString select="SELECT * FROM Contacts";
    qDebug()<<"Select query status: "<<query.exec(select);
    QSqlRecord rec =query.record();

    int id;
    QString login;
    QString password;

    while (query.next()){
        qDebug()<<"Room id: "<<query.value(rec.indexOf("IdRoom")).toInt()
        <<"login: "<<query.value(rec.indexOf("Login")).toString();
        }

    QSqlQuery query2(myDB);
    QString select2="SELECT * FROM Rooms";
    qDebug()<<"Select query status: "<<query2.exec(select2);
    QSqlRecord rec2 =query2.record();

    QString text;

    while (query2.next()){
        qDebug()<<"id: "<<query2.value(rec2.indexOf("Id")).toInt()
        <<"text: "<<query2.value(rec2.indexOf("Name")).toString();
        }

    QSqlQuery query3(myDB);
    QString select3="SELECT * FROM Events";
    qDebug()<<"Select query status: "<<query3.exec(select3);
    QSqlRecord rec3 =query3.record();

    while (query3.next()){
        qDebug()<<"Id: "<<query3.value(rec3.indexOf("Id")).toInt()
                   <<"Room id: "<<query3.value(rec3.indexOf("IdRoom")).toInt()
        <<"login: "<<query3.value(rec3.indexOf("Content")).toString();
        }


    }
    else
        qDebug()<<"db is not valid";
}



QList<QString> const MyDatabase::takeMessages(QString login){
    QSqlQuery query2(myDB);
    QString select2="SELECT * FROM Events AS e INNER JOIN Rooms AS r ON e.IdRoom=r.Id "
            "INNER JOIN Contacts c ON r.Id=c.IdRoom "
            "WHERE c.Login='"+login+"' ORDER BY e.Id";
    qDebug()<<"Select query status: "<<query2.exec(select2);
    QSqlRecord rec2 =query2.record();

    QList<QString> textList;

    while (query2.next()){
        textList.append(query2.value(rec2.indexOf("Content")).toString());
        }
    return textList;
}

void MyDatabase::insertMessage(Event e)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString IdStr=QString::number(e.id);
        QString IdRoomStr=QString::number(e.idRoom);
        QString insert ="INSERT INTO Events(Id, IdRoom, Content) VALUES ("+IdStr+", "+IdRoomStr+", '"+e.content+"')";
        bool res=query.exec(insert);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
        //this->printTable();
    }
}

QMap<int, bool> const MyDatabase::selectRooms()
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString selectRooms ="select Id, IsActive from Rooms";
        bool res=query.exec(selectRooms);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();

        QSqlRecord rec =query.record();

        QMap<int, bool>result;

        while (query.next()){
            result.insert(query.value(rec.indexOf("Id")).toInt(), query.value(rec.indexOf("IsActive")).toBool() );
            }

        return result;


    }
}

void MyDatabase::insertRoom(Room r)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insertRoom ="INSERT INTO Rooms (Id, Name, IsActive) VALUES ("+QString::number(r.id)+", '"+r.name+"', "+QString::number(r.isActive)+")";
        bool res=query.exec(insertRoom);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
    }
}

void MyDatabase::insertContact(Contact c)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insertContact ="INSERT INTO Contacts (Login, IdRoom) VALUES ('"+c.login+"', "+QString::number(c.idRoom)+")";
        bool res=query.exec(insertContact);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
    }
}

QMap<int, int> const MyDatabase::selectTopMessages()
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString selectIds ="SELECT MAX(Id) As Id, IdRoom FROM Events GROUP BY IdRoom";
        bool res=query.exec(selectIds);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();

        QSqlRecord rec =query.record();

        QMap<int, int>result;

        while (query.next()){
            result.insert(query.value(rec.indexOf("IdRoom")).toInt(), query.value(rec.indexOf("Id")).toInt() );
            }

        return result;


    }

}

int MyDatabase::selectRoomId(Room r)
{
    int result;
    QSqlQuery query(myDB);
    QString selectRooms ="select Id from Rooms where Name='"+r.name+"'";
    bool res=query.exec(selectRooms);
    QSqlRecord rec =query.record();

    while (query.next()){
        result = query.value(rec.indexOf("Id")).toInt();
        }

    return result;

}

QString MyDatabase::selectRoomName(int roomId)
{
    QSqlQuery query(myDB);
    QString result="";
    QString selectName ="select Name from Rooms where Id='"+QString::number(roomId)+"'";
    query.exec(selectName);
    QSqlRecord rec =query.record();
    while (query.next()){
        result = query.value(rec.indexOf("Name")).toString();
        }

    return result;
}

void MyDatabase::deleteRoom(Room r)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insertRoom ="DELETE FROM Rooms WHERE Id="+QString::number(r.id);
        bool res=query.exec(insertRoom);

        QSqlQuery query2(myDB);
        QString deleteContact ="DELETE FROM Contacts WHERE IdRoom="+QString::number(r.id);
        res=query2.exec(deleteContact);

        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
    }
}


