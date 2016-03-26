#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>
#include "memsys.h"
#include "cpu.h"

class Simulator : public QMainWindow {
    Q_OBJECT
public:
    Simulator(CPU* cpu, MemSys *memsys, QWidget *parent = 0);
    ~Simulator();
public slots:
    void memUpdate(uint8_t *data, uint32_t add, int len);
    void cacheUpadate(Cacheline* data, int idx, int hit, int miss);

private slots:
    void memOpen();
    void memImport();
    void memSave();
    void memLoad();
    void memStore();
    void clkReset();
    void cacheOnOFF();
    void cpuRun();
    void cpuStep();

private:
    QMenu *memMenu;
    QAction *openAct;
    QAction *importAct;
    QAction *saveAct;

    QLabel *clkLb;
    QPushButton *clkResetPB;
    QPushButton *runPB;
    QPushButton *stepPB;

    QLineEdit *addLE;
    QLineEdit *valLE;
    QPushButton *loadPB;
    QPushButton *storePB;
    QPushButton *cacheOnPB;
    QLabel *hitLb;
    QLabel *missLb;

    QLabel *memView[5];
    int memData[5];
    QGroupBox *ccGroup;
    QLabel *cacheView[5];
    int cacheData[5];

    MemSys *_memsys;
    CPU *_cpu;
};

#endif // SIMULATOR_H
