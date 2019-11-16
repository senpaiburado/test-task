#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ReadDatabaseThread.h"
#include <QDebug>
#include <QFileSystemModel>
#include <QDir>
#include <QTimer>

#include "Country.h"
#include "TreeDelegate.h"
#include "AddEditDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto dbThread = new ReadDatabaseThread("./system.db");
    m_addBt = new QPushButton("+", ui->treeView);

    connect(dbThread, &ReadDatabaseThread::oneCountryRead, this, [&](Country *country) {
        m_model->appendRow(country);
    });
    connect(dbThread, &ReadDatabaseThread::finished, dbThread, &ReadDatabaseThread::deleteLater);
    connect(ui->treeView, &QTreeView::doubleClicked, this, [&](const QModelIndex &index) {
        auto isCountry = index.data(Qt::UserRole + 1).toBool();
        if (isCountry)
            return;
        auto name = index.data(Qt::UserRole + 2).toString();
        auto mcc = index.data(Qt::UserRole + 3).toInt();
        auto mnc = index.data(Qt::UserRole + 4).toInt();

        AddEditDialog dl(m_model);
        dl.setData(name, mcc, mnc);

        connect(dl.worker(), &SaveWorker::tableUpdated, this, [&] {
            auto reloadThread = new ReadDatabaseThread("./system.db");
            m_model->clear();

            connect(reloadThread, &ReadDatabaseThread::oneCountryRead, this, [&](Country *country) {
                m_model->appendRow(country);
            });
            connect(reloadThread, &ReadDatabaseThread::finished, reloadThread, &ReadDatabaseThread::deleteLater);

            reloadThread->start();
        });

        connect(dl.worker(), &SaveWorker::rowUpdated, this, [&](const QString &name, const int mcc, const int mnc) {
            for (int i = 0; i < m_model->rowCount(); i++) {
                auto parent = m_model->index(i, 0);

                for (int i = 0; i < m_model->rowCount(parent); i++) {
                    auto index = m_model->index(i, 0, parent);
                    if (!index.data(Qt::UserRole + 1).toBool()) {
                        if (index.data(Qt::UserRole + 3).toInt() == mcc && index.data(Qt::UserRole + 4).toInt() == mnc) {
                            m_model->setData(index, name);
                            index.data();
                            return;
                        }
                    }
                }
            }
        });

        dl.exec();
    });
    connect(dbThread, &ReadDatabaseThread::finished, dbThread, &ReadDatabaseThread::deleteLater);
    dbThread->start();
    m_model = new QStandardItemModel(ui->treeView);
    ui->treeView->setModel(m_model);
    ui->treeView->setItemDelegate(new TreeDelegate(ui->treeView));
    ui->treeView->header()->hide();
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(m_addBt, &QPushButton::released, this, [&]{
        AddEditDialog dl(m_model);

        connect(dl.worker(), &SaveWorker::tableUpdated, this, [&] {
            auto reloadThread = new ReadDatabaseThread("./system.db");
            m_model->clear();

            connect(reloadThread, &ReadDatabaseThread::oneCountryRead, this, [&](Country *country) {
                m_model->appendRow(country);
            });
            connect(reloadThread, &ReadDatabaseThread::finished, reloadThread, &ReadDatabaseThread::deleteLater);

            reloadThread->start();
        });
        dl.exec();
    });
    m_addBt->setFlat(true);
    m_addBt->setGeometry(this->width() - 30, this->width() - 30, 40, 40);
    m_addBt->move(geometry().width() - 100, geometry().height() - 70);
    m_addBt->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

