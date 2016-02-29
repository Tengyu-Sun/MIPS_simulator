#include <QApplication>
#include "simulator.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Cache *cache = new Cache();
    Memory *memory = new Memory(1024, 50);
    MemSys *memsys = new MemSys(nullptr, memory, false);
    Simulator w(memsys);
    w.show();
    return a.exec();
}
