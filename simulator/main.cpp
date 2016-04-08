#include <QApplication>
#include <QObject>
#include "simulator.h"
#include <iostream>

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
//    Memory *memory = new Memory(1024, 1);
//    Cache *cache = new Cache(8, 4, 2, 10, ReplacePolicy::LRU, WritePolicy::WRITEBACK, memory);

    MemSys *memsys = new MemSys;
    MemSysConfig config;
    config.cacheLevel = 1;
    config.cacheOn = false;
    config.memCycle = 10;
    config.memSize = 1024;
    config.cacheSettings = std::vector<CacheSettings>();
    CacheSettings cs;
    cs.cycle = 5;
    cs.indexsize = 8;
    cs.linesize = 4;
    cs.ways = 2;
    cs.rpolicy = ReplacePolicy::LRU;
    cs.wpolicy = WritePolicy::WRITEBACK;
    config.cacheSettings.push_back(cs);
    memsys->init(config);
    FPU *fpu = new FPU(10);
    VU* vu = new VU(5);
    CPU *cpu = new CPU(memsys, fpu, vu);
    Simulator w(cpu, memsys);
    QObject::connect(memsys, &MemSys::memNotify, &w, &Simulator::memUpdate);
    QObject::connect(memsys, &MemSys::cacheHitNotify, &w, &Simulator::cacheHitUpdate);
    QObject::connect(memsys, &MemSys::cacheMissNotify, &w, &Simulator::cacheMissUpdate);
    QObject::connect(memsys, &MemSys::cacheLineNotify, &w, &Simulator::cacheLineUpadate);
    memsys->fresh();
    w.show();
    return a.exec();
}
