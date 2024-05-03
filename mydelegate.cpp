#include "mydelegate.h"
#include <QComboBox>
#include <QEvent>
#include <QMouseEvent>
#include <QColorDialog>
#include <QApplication>


MyDelegate::MyDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


QWidget *MyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    return QStyledItemDelegate::createEditor (parent, option, index);

}

void MyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    return QStyledItemDelegate::setEditorData (editor, index);
}

void MyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    return QStyledItemDelegate::setModelData (editor, model, index);

}


bool MyDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{

   return QStyledItemDelegate::editorEvent(event, model, option, index);

 }




void MyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    /*QStyleOptionViewItem op(option);


            op.font.setBold(true);
            op.palette.setColor(QPalette::Normal, QPalette::Background, Qt::black);
            op.palette.setColor(QPalette::Normal, QPalette::Foreground, Qt::white);*/

        //QStyledItemDelegate::paint(painter, op, index);
}
