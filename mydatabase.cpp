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

void MyDatabase::printTable(){
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

QMap<int, bool> MyDatabase::selectRooms()
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
        QString insertRoom ="INSERT INTO Rooms (Id, Name, IsActive) VALUES ("+QString::number(r.Id)+", '"+r.Name+"', "+QString::number(r.IsActive)+")";
        bool res=query.exec(insertRoom);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
    }
}

void MyDatabase::insertContact(Contact c)
{
    if (myDB.isValid()){
        QSqlQuery query(myDB);
        QString insertContact ="INSERT INTO Contacts (Login, IdRoom) VALUES ('"+c.Login+"', "+QString::number(c.IdRoom)+")";
        bool res=query.exec(insertContact);
        qDebug()<<"Insert query status: "<<res;
        if (!res) qDebug()<<query.lastError();
    }
}

QMap<int, int> MyDatabase::selectTopMessages()
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
