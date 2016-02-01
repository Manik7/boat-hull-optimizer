#include "hullplotter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HullPlotter w;
    w.show();

    return a.exec();
}
