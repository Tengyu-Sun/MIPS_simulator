#include "simulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    char mem[10];
    Simulator w(mem);
    w.show();
    return a.exec();
}
