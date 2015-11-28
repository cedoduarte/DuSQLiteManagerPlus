#include "MainWindow.h"
#include "Application.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setApplicationSettings(a);
    MainWindow w;
    w.show();
    return a.exec();
}
