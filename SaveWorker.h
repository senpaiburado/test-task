#ifndef SAVEWORKER_H
#define SAVEWORKER_H

#include <QObject>

class SaveWorker : public QObject
{
    Q_OBJECT
public:
    explicit SaveWorker(QObject *parent = nullptr);

    enum class Action { SAVE = 0, UPDATE };

    void setData(const QString &name, const int mcc, const int mnc, Action action);

private:
    QString m_name;
    int m_mcc;
    int m_mnc;
    Action m_action;

public slots:
    void process();
signals:
    void finished();
    void tableUpdated();
    void rowUpdated(const QString &name, const int mcc, int mnc);
};

#endif // SAVEWORKER_H
