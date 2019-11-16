#include "SaveWorker.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

SaveWorker::SaveWorker(QObject *parent) : QObject(parent) {}

void SaveWorker::setData(const QString &name, const int mcc, const int mnc, SaveWorker::Action action)
{
    m_name = name;
    m_mcc = mcc;
    m_mnc = mnc;
    m_action = action;
}

void SaveWorker::process()
{
    QString command;
    if (m_action == Action::SAVE)
        command = QString("INSERT INTO operators VALUES(%1, %2, \"%3\");").arg(m_mcc).arg(m_mnc).arg(m_name);
    else if (m_action == Action::UPDATE)
        command = QString("UPDATE operators SET name = \"%1\" WHERE mcc = %2 AND mnc = %3;").arg(m_name).arg(m_mcc).arg(m_mnc);

    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./system.db");

    if (!db.open()) {
        qCritical() << "DB is not opened!";
        return;
    }

    auto result = db.exec(command);
    if (result.lastError().type() == QSqlError::NoError) {
        if (m_action == Action::SAVE)
            emit tableUpdated();
        else
            emit rowUpdated(m_name, m_mcc, m_mnc);
    }
    else
        qWarning() << result.lastError().text();

    db.close();
    emit finished();
}
