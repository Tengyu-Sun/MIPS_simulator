#include <QApplication>
#include "simulator.h"
#include <iostream>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    Memory *memory = new Memory(1024, 5);
    Cache *cache = new Cache(8, 4, 2, 5, memory);

    MemSys *memsys = new MemSys(cache, memory, false);
    CPU *cpu = new CPU(memsys);
    Simulator w(cpu, memsys);
    w.show();
    return a.exec();
}
