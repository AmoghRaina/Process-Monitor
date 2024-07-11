#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Process Finder");
    w.setWindowIcon(QIcon(":/resources/rsimg/file.ico"));
    w.show();
    return a.exec();
}
