#ifndef SQLQUERYTHREAD_H
#define SQLQUERYTHREAD_H

#include <QThread>
#include "Country.h"

class ReadDatabaseThread : public QThread
{
    Q_OBJECT
public:
    explicit ReadDatabaseThread(const QString &dbName = "./system.db", QObject *parent = nullptr);

    void run() override final;

private:
    QString m_dbName;

signals:
    void oneCountryRead(Country *country);
};

#endif // SQLQUERYTHREAD_H
