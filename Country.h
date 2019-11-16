#ifndef COUNTRY_H
#define COUNTRY_H

#include <QStandardItem>
class Operator;
#include "Operator.h"
#include <QList>

class Country : public QStandardItem
{
//    Q_OBJECT
public:
    Country(const QString &name, const QString &code, const int mcc);

    QString name() const;
    QString code() const;
    int mcc() const;
    QVariant data(int role) const;
    void appendOperator(const QString &name, const int mcc, const int mnc);

private:
    QString m_name;
    QString m_code;
    int m_mcc;
};

#endif // COUNTRY_H
