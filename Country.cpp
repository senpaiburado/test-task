#include "Country.h"
#include <QDebug>

Country::Country(const QString &name, const QString &code, const int mcc)
    : QStandardItem()
{
    m_name = name;
    m_mcc = mcc;
    m_code = code;
}

QString Country::name() const
{
    return m_name;
}

int Country::mcc() const
{
    return m_mcc;
}

void Country::appendOperator(const QString &name, const int mcc, const int mnc)
{
    appendRow(new Operator(name, mcc, mnc));
}


QVariant Country::data(int role) const
{
    if (role == Qt::DisplayRole)
        return m_name;
    else if (role == Qt::DecorationRole)
        return m_code;
    else if (role == Qt::UserRole + 1)
        return true;
    else if (role == Qt::UserRole + 2)
        return m_mcc;
    return QVariant();
}

QString Country::code() const
{
    return m_code;
}
