#include "MainWindow.h"

#include <QApplication>

#include "ReadDatabaseThread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
