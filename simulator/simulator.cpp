#include "simulator.h"
#include <string>
#include <iostream>
#include <fstream>

Simulator::Simulator(CPU *cpu, MemSys* memsys, QWidget *parent) : QMainWindow(parent) {
    _cpu = cpu;
    _memsys = memsys;

    openAct = new QAction(tr("open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(memOpen()));
    saveAct = new QAction(tr("save"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(memSave()));
    importAct = new QAction(tr("import"), this);
    connect(importAct, SIGNAL(triggered()), this, SLOT(memImport()));
    memMenu = menuBar()->addMenu(tr("Memory"));
    memMenu->addAction(openAct);
    memMenu->addAction(importAct);
    memMenu->addAction(saveAct);


    QGroupBox *cpuGroup = new QGroupBox(tr("CPU"));
    QLabel *ccLb = new QLabel(tr("clock cycle:"));
    clkLb = new QLabel(tr("0"));
    clkResetPB = new QPushButton(tr("Reset"));
    runPB = new QPushButton(tr("Run"));
    stepPB = new QPushButton(tr("Step"));
    connect(clkResetPB, SIGNAL(clicked()), this, SLOT(clkReset()));
    connect(runPB, SIGNAL(clicked()), this, SLOT(cpuRun()));
    connect(stepPB, SIGNAL(clicked()), this, SLOT(cpuStep()));
    QGridLayout *cpuLayout = new QGridLayout;
    cpuLayout->addWidget(ccLb, 0, 0);
    cpuLayout->addWidget(clkLb, 0, 1);
    cpuLayout->addWidget(clkResetPB, 1, 0);
    cpuLayout->addWidget(runPB, 2, 0);
    cpuLayout->addWidget(stepPB, 2, 1);
    cpuGroup->setLayout(cpuLayout);

    QGroupBox *memGroup = new QGroupBox(tr("Memory System"));
    addLE = new QLineEdit;
    valLE = new QLineEdit;
    loadPB = new QPushButton(tr("Load"));
    connect(loadPB, SIGNAL(clicked()), this, SLOT(memLoad()));
    storePB = new QPushButton(tr("Store"));
    connect(storePB, SIGNAL(clicked()), this, SLOT(memStore()));
    cacheOnPB = new QPushButton(tr("ON"));
    if (!_memsys->_cacheOn){
      cacheOnPB->setText(tr("OFF"));
    }
    connect(cacheOnPB, SIGNAL(clicked()), this, SLOT(cacheOnOFF()));
    QLabel *hLb = new QLabel(tr("hit: "));
    hitLb = new QLabel(tr("0"));
    QLabel *mLb = new QLabel(tr("miss: "));
    missLb = new QLabel(tr("0"));
    memTW = new QTableWidget(5, 2);
    cacheTW = new QTableWidget(5, 2);
    QGroupBox *mmGroup = new QGroupBox(tr("Main Memory"));
    QGridLayout *mmLayout = new QGridLayout;
    for(int i=0; i<5; ++i) {
        std::string lb = std::to_string(i)+":";
        memView[i] = new QLabel(tr("0"));
        mmLayout->addWidget(new QLabel(lb.c_str()), i, 0);
        mmLayout->addWidget(memView[i], i, 1);
    }
    mmGroup->setLayout(mmLayout);

    QGridLayout *memLayout = new QGridLayout;
    memLayout->addWidget(addLE, 0, 0);
    memLayout->addWidget(valLE, 0, 1);
    memLayout->addWidget(loadPB, 0, 2);
    memLayout->addWidget(storePB, 0, 3);
    memLayout->addWidget(cacheOnPB, 1, 0);
    memLayout->addWidget(hLb, 1, 1);
    memLayout->addWidget(hitLb, 1, 2);
    memLayout->addWidget(mLb, 1, 3);
    memLayout->addWidget(missLb, 1, 4);
   // memLayout->addWidget(cacheTW, 2, 1);
    memLayout->addWidget(mmGroup, 2, 2);
    memGroup->setLayout(memLayout);

    QWidget *cw = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cpuGroup, 0, 0);
    layout->addWidget(memGroup, 0, 1);
    cw->setLayout(layout);

    setCentralWidget(cw);

}

void Simulator::memOpen() {
    std::string filename = QFileDialog::getOpenFileName(this,tr("Open Test Case"),
               "/Users/blade/workspace/cs535/MIPS_simulator").toStdString();
    std::fstream input(filename);
    std::string line;
    while(input){
        std::getline(input, line);
        if (line[0] == 'L') {
            int add = std::stoi(line.substr(2));
            uint8_t val = 0;
            do {
               _cpu->clk++;
               clkLb->setText(std::to_string(_cpu->clk).c_str());
               //std::cout<<clk<<" ";
            } while(_memsys->loadByte(add, &val) == 0);
           // std::cout<<std::endl;
            std::cout<<"load "<<add<<": "<<(int)val<<std::endl;
            if(_memsys->_cacheOn) {
                int h = _memsys->getHit();
                int m = _memsys->getMiss();
                std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
                hitLb->setText(std::to_string(h).c_str());
                missLb->setText(std::to_string(m).c_str());
            }
        } else if (line[0] == 'S') {
            int p = 2;
            int nl = line.size();
            while(p<nl && line[p] != ',') {
                ++p;
            }
            int add = std::stoi(line.substr(2, p-2));
            int val = std::stoi(line.substr(p+1));
            do {
              _cpu->clk++;
              clkLb->setText(std::to_string(_cpu->clk).c_str());
              //std::cout<<clk<<" ";
            } while(_memsys->storeByte(add, val) == 0);

            std::cout<<"store "<<add<<" "<<(int)val<<std::endl;
            if(_memsys->_cacheOn) {
                int h = _memsys->getHit();
                int m = _memsys->getMiss();
                std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
                hitLb->setText(std::to_string(h).c_str());
                missLb->setText(std::to_string(m).c_str());
            }
        } else {
            return;
           // std::cout<<"unrecognized command: "<<line<<std::endl;
        }
    }
}

void Simulator::memSave() {
    _memsys->dump("/Users/blade/workspace/cs535/output.txt");
}

void Simulator::memLoad() {
    int add = 0;
    try {
      add = std::stoi(addLE->displayText().toStdString());
    } catch(...) {
      add = 0;
    }
    uint8_t val = 0;
    do {
       _cpu->clk++;
       clkLb->setText(std::to_string(_cpu->clk).c_str());
       std::cout<<_cpu->clk<<std::endl;
    } while(_memsys->loadByte(add, &val) == 0);
    std::cout<<"load "<<add<<": "<<(int)val<<std::endl;
    if(_memsys->_cacheOn) {
        int h = _memsys->getHit();
        int m = _memsys->getMiss();
        std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
        hitLb->setText(std::to_string(h).c_str());
        missLb->setText(std::to_string(m).c_str());
    }
}

void Simulator::memStore() {
    int add = 0;
    try{
      add = std::stoi(addLE->displayText().toStdString());
    } catch(...) {
      add = 0;
    }
    uint8_t val = 0;
    try {
      val = (uint8_t)std::stoi(valLE->displayText().toStdString());
    } catch(...) {
      val = 0;
    }
    do {
      _cpu->clk++;
      clkLb->setText(std::to_string(_cpu->clk).c_str());
      std::cout<<_cpu->clk<<std::endl;
    } while(_memsys->storeByte(add, val) == 0);
    std::cout<<"store "<<add<<" "<<(int)val<<std::endl;
    if(_memsys->_cacheOn) {
        int h = _memsys->getHit();
        int m = _memsys->getMiss();
        std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
        hitLb->setText(std::to_string(h).c_str());
        missLb->setText(std::to_string(m).c_str());
    }
}

void Simulator::memImport() {
    std::string filename = QFileDialog::getOpenFileName(this,tr("Import Memory"),
               "/Users/blade/workspace/cs535/MIPS_simulator/asm").toStdString();
    std::fstream input(filename);
    uint32_t ins = 0;
    int add = 0;
    while(input){
       input>>ins;
       //std::cout<<ins<<std::endl;
       uint8_t tmp = ins & 0xff;
       while(_memsys->storeByte(add+3, tmp) != 1);
       ins >>= 8;
       tmp = ins & 0xff;
       while(_memsys->storeByte(add+2, tmp) != 1);
       ins >>= 8;
       tmp = ins & 0xff;
       while(_memsys->storeByte(add+1, tmp) != 1);
       ins >>= 8;
       tmp = ins & 0xff;
       while(_memsys->storeByte(add, tmp) != 1);
       add = add + 4;
       ins = 0;
    }
    return;
}

void Simulator::clkReset() {
    _cpu->clk = 0;
    clkLb->setText(std::to_string(_cpu->clk).c_str());
}

void Simulator::cacheOnOFF() {
    _memsys->_cacheOn = !_memsys->_cacheOn;
    if (_memsys->_cacheOn){
        cacheOnPB->setText(tr("ON"));
        _memsys->resetCache();
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    } else {
        cacheOnPB->setText(tr("OFF"));
        _memsys->resetCache();
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    }
}

void Simulator::cpuRun() {
    std::cout<<"Run"<<std::endl;
    while(!_cpu->err) {
      _cpu->step();
      clkLb->setText(std::to_string(_cpu->clk).c_str());
    }
}

void Simulator::cpuStep() {
    if (!_cpu->err) {
        std::cout<<"Step"<<std::endl;
        _cpu->step();
        clkLb->setText(std::to_string(_cpu->clk).c_str());
    }
}

void Simulator::memUpdate(uint8_t byte) {
    memView[2]->setText(std::to_string((int)byte).c_str());
}

Simulator::~Simulator() {

}
