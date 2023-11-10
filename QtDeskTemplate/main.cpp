#include "QtDeskTemplate.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtDeskTemplate w;
    w.show();
    return a.exec();
}
