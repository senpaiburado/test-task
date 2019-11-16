#include "ReadDatabaseThread.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

ReadDatabaseThread::ReadDatabaseThread(const QString &dbName, QObject *parent) : QThread(parent), m_dbName(dbName) {}

void ReadDatabaseThread::run()
{
    auto db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_dbName);

    if (!db.open()) {
        qCritical() << "DB is not opened!";
        return;
    }

    auto resultCountries = db.exec("SELECT * FROM countries;");
    if (resultCountries.lastError().type() != QSqlError::NoError) {
        db.close();
        qCritical() << "Error reading database: " << resultCountries.lastError().text();
        return;
    }

    while (resultCountries.next()) {
        auto countryName = resultCountries.value("name").toString();
        auto countryMCC = resultCountries.value("mcc").toInt();
        auto countryCode = resultCountries.value("code").toString();

        auto country = new Country(countryName, countryCode, countryMCC);
        auto resultOperators = db.exec(QString("SELECT * FROM operators WHERE mcc = %1;").arg(countryMCC));

        while (resultOperators.next()) {
            auto operatorName = resultOperators.value("name").toString();
            auto operatorMCC = resultOperators.value("mcc").toInt();
            auto operatorMNC = resultOperators.value("mnc").toInt();

            country->appendOperator(operatorName, operatorMCC, operatorMNC);
        }

        emit oneCountryRead(country);
    }

    db.close();
}
