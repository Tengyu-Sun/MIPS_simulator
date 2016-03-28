#include <QApplication>
#include <QObject>
#include "simulator.h"
#include <iostream>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    Memory *memory = new Memory(1024, 5);
    Cache *cache = new Cache(8, 4, 2, 10, memory);

    MemSys *memsys = new MemSys(cache, memory, false);
    FPU *fpu = new FPU(10);
    VU* vu = new VU(5);
    CPU *cpu = new CPU(memsys, fpu, vu);
    Simulator w(cpu, memsys);
    QObject::connect(memory, &Memory::update, &w, &Simulator::memUpdate);
    QObject::connect(cache, &Cache::updateHit, &w, &Simulator::cacheHitUpdate);
    QObject::connect(cache, &Cache::updateMiss, &w, &Simulator::cacheMissUpdate);
    QObject::connect(cache, &Cache::updateCacheline, &w, &Simulator::cacheUpadate);
    w.show();
    return a.exec();
}
