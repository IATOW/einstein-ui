#include "mainwindow.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QFile ss(qApp->applicationDirPath() + "/main.txt");
    //ss.open(QIODevice::ReadOnly);
    //QString styles = ss.readAll();

    MainWindow w;
    //w.setStyleSheet(styles);
    w.show();

    return a.exec();
}
