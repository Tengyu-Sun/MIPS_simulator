#include "simulator.h"


Simulator::Simulator(char *mem, QWidget *parent) : QMainWindow(parent) {
    memsys = mem;

    QGroupBox *cpuGroup = new QGroupBox(tr("CPU"));

    QGroupBox *memGroup = new QGroupBox(tr("Memory"));
    addLE = new QLineEdit;
    valLE = new QLineEdit;
    loadPB = new QPushButton(tr("Load"));
    storePB = new QPushButton(tr("Store"));
    memTW = new QTableWidget(5,2);

    QGridLayout *memLayout = new QGridLayout;
    memLayout->addWidget(addLE, 0, 0);
    memLayout->addWidget(loadPB, 0, 1);
    memLayout->addWidget(valLE, 0, 2);
    memLayout->addWidget(storePB, 0, 3);
    memLayout->addWidget(memTW, 1, 2, 5, 2);
    memGroup->setLayout(memLayout);

    QWidget *cw = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cpuGroup, 0, 0);
    layout->addWidget(memGroup, 0, 1);
    cw->setLayout(layout);

    setCentralWidget(cw);

}

Simulator::~Simulator()
{

}

