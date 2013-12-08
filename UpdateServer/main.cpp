#include "UpdateServerWindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpdateServerWindow w;
    w.show();

    return a.exec();
}
