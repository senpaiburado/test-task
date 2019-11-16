#ifndef TREEDELEGATE_H
#define TREEDELEGATE_H

#include <QStyledItemDelegate>

class TreeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TreeDelegate(QObject *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // TREEDELEGATE_H
