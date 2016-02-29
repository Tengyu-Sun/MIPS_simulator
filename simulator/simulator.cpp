#include "simulator.h"
#include <string>

Simulator::Simulator(QWidget *parent) : QMainWindow(parent) {
    //memsys = mem;
    clk = 0;

    openAct = new QAction(tr("open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(memOpen()));
    saveAct = new QAction(tr("save"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(memSave()));
    memMenu = menuBar()->addMenu(tr("Memory"));
    memMenu->addAction(openAct);
    memMenu->addAction(saveAct);

    QGroupBox *cpuGroup = new QGroupBox(tr("CPU"));
    QLabel *ccLb = new QLabel(tr("clock circle:"));
    clkLb = new QLabel(tr("0"));
    QGridLayout *cpuLayout = new QGridLayout;
    cpuLayout->addWidget(ccLb, 0, 0);
    cpuLayout->addWidget(clkLb, 0, 1);
    cpuGroup->setLayout(cpuLayout);

    QGroupBox *memGroup = new QGroupBox(tr("Memory"));
    addLE = new QLineEdit;
    valLE = new QLineEdit;
    loadPB = new QPushButton(tr("Load"));
    storePB = new QPushButton(tr("Store"));
    memTW = new QTableWidget(5,2);
    cacheTW = new QTableWidget(5,2);

    QGridLayout *memLayout = new QGridLayout;
    memLayout->addWidget(addLE, 0, 0);
    memLayout->addWidget(loadPB, 0, 1);
    memLayout->addWidget(valLE, 0, 2);
    memLayout->addWidget(storePB, 0, 3);
    memLayout->addWidget(cacheTW, 1, 0, 5, 2);
    memLayout->addWidget(memTW, 1, 1, 5, 2);
    memGroup->setLayout(memLayout);

    QWidget *cw = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cpuGroup, 0, 0);
    layout->addWidget(memGroup, 0, 1);
    cw->setLayout(layout);

    setCentralWidget(cw);

}

void Simulator::memOpen() {
    clk++;
    clkLb->setText(std::to_string(clk).c_str());
}

void Simulator::memSave() {
    clk--;
    clkLb->setText(std::to_string(clk).c_str());
}

Simulator::~Simulator() {

}

