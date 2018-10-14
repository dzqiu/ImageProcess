#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include "Python.h"
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    w.show();
    return a.exec();
}
