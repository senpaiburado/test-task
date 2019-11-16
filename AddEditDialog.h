#ifndef ADDEDITDIALOG_H
#define ADDEDITDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "SaveWorker.h"

namespace Ui {
class AddEditDialog;
}

class AddEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddEditDialog(QStandardItemModel *model, QWidget *parent = nullptr);
    ~AddEditDialog();

    void setData(const QString &name, const int mcc, const int mnc);
    SaveWorker *worker();

private:
    Ui::AddEditDialog *ui;
    QStandardItemModel *m_model;
    SaveWorker *m_worker;

    QString m_name;
    int m_mcc;
    int m_mnc;

    void searchOperatorIcon();
    void searchCountryIcon();
};

#endif // ADDEDITDIALOG_H
