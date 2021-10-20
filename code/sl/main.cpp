#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!createConnection())
          return 1;
    MainWindow w;
    w.setFixedSize(1000, 600);
    w.show();
    return a.exec();
}
