#ifndef EVENT_H
#define EVENT_H


#include <QString>

//Структура содержащая сведения о  сообщении
struct Event
{
    //Идентифкатор чата
    int idRoom;
    //Идентифкатор сообщения
    int id;
    //Текст сообщения
    QString content;

};

#endif // EVENT_H
