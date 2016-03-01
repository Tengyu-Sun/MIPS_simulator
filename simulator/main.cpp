#include <QApplication>
#include "simulator.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Memory *memory = new Memory(1024, 50);
    Cache *cache = new Cache(8, 4, 2, 5, memory);

    MemSys *memsys = new MemSys(cache, memory, true);
    Simulator w(memsys);
    w.show();
    return a.exec();
}
