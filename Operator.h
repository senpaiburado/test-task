#ifndef OPERATOR_H
#define OPERATOR_H

#include <QString>
#include <QVariant>

class Country;
#include "Country.h"

class Operator : public QStandardItem
{
public:
    Operator(const QString &name, const int mcc, const int mnc);

    QString name() const;
    int mcc() const;
    int mnc() const;

    void setName(const QString &name);

    virtual QVariant data(int role) const override;
    virtual void setData(const QVariant &value, int role = Qt::UserRole + 1) override;

private:
    QString m_name;
    int m_mcc;
    int m_mnc;
};

#endif // OPERATOR_H
