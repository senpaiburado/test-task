#include "TreeDelegate.h"
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QApplication>

TreeDelegate::TreeDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void TreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    itemOption.icon.addPixmap(QPixmap(":/Icons/" + index.data(Qt::DecorationRole).toString() + ".png"));
    if (index.data(Qt::UserRole + 1).toBool())
        itemOption.font.setBold(true);
    else {
        QStyleOptionButton button;
        button.text = "+";
        button.rect.setSize(QSize(16, 16));
        button.rect.setRect(itemOption.rect.width(), itemOption.rect.height(), 16, 16);
        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    }
    QStyledItemDelegate::paint(painter, itemOption, index);
}
