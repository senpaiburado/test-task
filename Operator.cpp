#include "Operator.h"
#include <QDebug>
#include <QList>

Operator::Operator(const QString &name, const int mcc, const int mnc)
    : QStandardItem()
{
    m_name = name;
    m_mcc = mcc;
    m_mnc = mnc;
}

QString Operator::name() const
{
    return m_name;
}

int Operator::mcc() const
{
    return m_mcc;
}

int Operator::mnc() const
{
    return m_mnc;
}

void Operator::setName(const QString &name)
{
    m_name = name;
}

QVariant Operator::data(int role) const
{
    if (role == Qt::DisplayRole)
        return QString("%1 (%2, %3)").arg(m_name).arg(m_mcc).arg(m_mnc);
    else if (role == Qt::DecorationRole)
        return QString("%1_%2").arg(m_mcc).arg(m_mnc);
    else if (role == Qt::UserRole + 2)
        return m_name;
    else if (role == Qt::UserRole + 3)
        return m_mcc;
    else if (role == Qt::UserRole + 4)
        return m_mnc;
    return QVariant();
}

void Operator::setData(const QVariant &value, int role)
{
    if (role == Qt::EditRole)
        m_name = value.toString();
}

