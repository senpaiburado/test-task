#include "AddEditDialog.h"
#include "ui_AddEditDialog.h"
#include "Country.h"
#include "SaveWorker.h"

#include <QIntValidator>
#include <QDebug>
#include <QThread>

AddEditDialog::AddEditDialog(QStandardItemModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddEditDialog)
{
    ui->setupUi(this);
    m_model = model;
    m_worker = new SaveWorker;

    ui->mccEdit->setValidator(new QIntValidator(ui->mccEdit));
    ui->mncEdit->setValidator(new QIntValidator(ui->mncEdit));

    connect(ui->mccEdit, &QLineEdit::textChanged, this, [&] {
        searchCountryIcon();
        searchOperatorIcon();
    });

    connect(ui->mncEdit, &QLineEdit::textChanged, this, [&] {
        searchOperatorIcon();
    });

    connect(ui->cancelButton, &QPushButton::released, this, [&] {
        delete m_worker;
        close();
    });

    connect(ui->saveButton, &QPushButton::released, this, [&] {
        auto thread = new QThread;

        m_worker->setData(ui->nameEdit->text(), ui->mccEdit->text().toInt(), ui->mncEdit->text().toInt(), m_name.isEmpty() ? SaveWorker::Action::SAVE : SaveWorker::Action::UPDATE);
        m_worker->moveToThread(thread);
        connect(thread, &QThread::started, m_worker, &SaveWorker::process);
        connect(m_worker, &SaveWorker::finished, thread, &QThread::quit);
        connect(m_worker, &SaveWorker::finished, m_worker, &SaveWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
        close();

    });
    ui->saveButton->setEnabled(true);
}

AddEditDialog::~AddEditDialog()
{
    delete ui;
}

void AddEditDialog::setData(const QString &name, const int mcc, const int mnc)
{
    m_name = name;
    m_mcc = mcc;
    m_mnc = mnc;

    ui->nameEdit->setText(m_name);
    ui->mccEdit->setText(QString::number(m_mcc));
    ui->mncEdit->setText(QString::number(m_mnc));

    ui->mccEdit->setEnabled(false);
    ui->mncEdit->setEnabled(false);
}

SaveWorker *AddEditDialog::worker()
{
    return m_worker;
}

void AddEditDialog::searchOperatorIcon()
{
    auto path = QString(":/Icons/%1_%2.png").arg(ui->mccEdit->text()).arg(ui->mncEdit->text());
    QPixmap icon(path);
    if (icon.isNull())
        ui->operatorIcon->setPixmap(QPixmap(":/Icons/question.png"));
    else
        ui->operatorIcon->setPixmap(icon);
}

void AddEditDialog::searchCountryIcon()
{
    for (int i = 0; i < m_model->rowCount(); i++) {
        auto index = m_model->index(i, 0);
        auto mcc = index.data(Qt::UserRole + 2).toInt();
        if (mcc == ui->mccEdit->text().toInt()) {
            auto path = QString(":/Icons/%1.png").arg(index.data(Qt::DecorationRole).toString());
            QPixmap icon(path);
            if (icon.isNull())
                ui->countryIcon->setPixmap(QPixmap(":/Icons/question.png"));
            else
                ui->countryIcon->setPixmap(icon);
            break;
        }
    }
}
