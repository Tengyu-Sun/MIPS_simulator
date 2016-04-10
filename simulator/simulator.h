#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>
#include "memsys.h"
#include "cpu.h"
#include "configdialog.h"
#include "memoryview.h"
#include "cacheview.h"
#include "cpuview.h"

class Simulator : public QMainWindow {
    Q_OBJECT
public:
    Simulator(CPU* cpu, MemSys *memsys, QWidget *parent = 0);
    ~Simulator();

private slots:
    void memOpen();
    void memImport();
    void memSave();
    void memLoad();
    void memStore();
    void memConfig();
    void cacheOnOFF();
    void memsysInit(MemSysConfig config);
    void memUpdate(uint8_t *data, uint32_t add, int len);
    void cacheLineUpadate(int level, Cacheline** data, int idx, int way);
    void cacheHitUpdate(int level, int hit);
    void cacheMissUpdate(int level, int miss);
    void clkReset();
    void cpuRun();
    void cpuStep();
    void cpuPipeSet(bool p);

private:
    ConfigDialog *configDL;

    CPUView *cpuView;

    QLineEdit *addLE;
    QLineEdit *valLE;
    QPushButton *cacheOnPB;
    QLabel *hitLb;
    QLabel *missLb;

    MemoryView *mv;
    QGridLayout *mvLayout;

    QTabWidget *cacheViewTW;
    QGroupBox *ccGroup;

    MemSys *_memsys;
    CPU *_cpu;
};

#endif // SIMULATOR_H
