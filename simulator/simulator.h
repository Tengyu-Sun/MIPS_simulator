#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QMainWindow>
#include <QtWidgets>
#include "memsys.h"

class Simulator : public QMainWindow {
    Q_OBJECT

public:
    Simulator(MemSys *memsys, QWidget *parent = 0);
    ~Simulator();

private slots:
    void memOpen();
    void memSave();
    void memLoad();
    void memStore();
    void clkReset();
    void cacheOnOFF();

private:
    QMenu *memMenu;
    QAction *openAct;
    QAction *saveAct;

    QLabel *clkLb;
    uint64_t clk;
    QPushButton *clkResetPB;

    QLineEdit *addLE;
    QLineEdit *valLE;
    QPushButton *loadPB;
    QPushButton *storePB;
    QPushButton *cacheOnPB;
    QLabel *hitLb;
    QLabel *missLb;

    QTableWidget *cacheTW;
    QTableWidget *memTW;

    MemSys *_memsys;

};

#endif // SIMULATOR_H
