#ifndef MYDELEGATE_H
#define MYDELEGATE_H

#include <QStyledItemDelegate>



class MyDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyDelegate(QObject *parent = 0);

    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    // QAbstractItemDelegate interface
public:
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;


    // QAbstractItemDelegate interface

    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MYDELEGATE_H
