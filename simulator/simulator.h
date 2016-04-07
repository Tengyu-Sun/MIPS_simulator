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

class Simulator : public QMainWindow {
    Q_OBJECT
public:
    Simulator(CPU* cpu, MemSys *memsys, QWidget *parent = 0);
    ~Simulator();
public slots:
    void memUpdate(uint8_t *data, uint32_t add, int len);
    void cacheLineUpadate(int level, Cacheline** data, int idx, int way);
    void cacheHitUpdate(int level, int hit);
    void cacheMissUpdate(int level, int miss);

private slots:
    void memOpen();
    void memImport();
    void memSave();
    void memLoad();
    void memStore();
    void memConfig();
    void clkReset();
    void cacheOnOFF();
    void cpuRun();
    void cpuStep();
    void memsysInit(MemSysConfig config);

private:

    ConfigDialog *configDL;

    QLabel *clkLb;
    QPushButton *clkResetPB;
    QPushButton *runPB;
    QPushButton *stepPB;

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
