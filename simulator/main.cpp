#include <QApplication>
#include "simulator.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Memory *memory = new Memory(1024, 100);
    Cache *cache = new Cache(8, 4, 2, 5, memory);

    MemSys *memsys = new MemSys(cache, memory, true);
    CPU *cpu = new CPU(memsys);
    Simulator w(cpu, memsys);
    w.show();
    return a.exec();
}
