#include "EcoCensus.h"
#include <QApplication>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EcoCensus w;
    w.show();

    return a.exec();
}
