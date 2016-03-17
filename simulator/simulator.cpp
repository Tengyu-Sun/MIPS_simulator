#include "simulator.h"
#include <string>
#include <iostream>
#include <fstream>

Simulator::Simulator(MemSys* memsys, QWidget *parent) : QMainWindow(parent) {
    _memsys = memsys;
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
    clkResetPB = new QPushButton(tr("Reset"));
    connect(clkResetPB, SIGNAL(clicked()), this, SLOT(clkReset()));
    QGridLayout *cpuLayout = new QGridLayout;
    cpuLayout->addWidget(ccLb, 0, 0);
    cpuLayout->addWidget(clkLb, 0, 1);
    cpuLayout->addWidget(clkResetPB, 1, 0);
    cpuGroup->setLayout(cpuLayout);

    QGroupBox *memGroup = new QGroupBox(tr("Memory"));
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
    memTW = new QTableWidget(5,2);
    cacheTW = new QTableWidget(5,2);

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
    //memLayout->addWidget(cacheTW, 1, 0);
    //memLayout->addWidget(memTW, 1, 1);
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
               clk++;
               clkLb->setText(std::to_string(clk).c_str());
               //std::cout<<clk<<" ";
            } while(_memsys->load(add, &val) == 0);
           // std::cout<<std::endl;
            std::cout<<"load "<<add<<": "<<val<<std::endl;
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
              clk++;
              clkLb->setText(std::to_string(clk).c_str());
              //std::cout<<clk<<" ";
            } while(_memsys->store(add, val) == 0);

            std::cout<<"store "<<add<<" "<<val<<std::endl;
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
    int add = std::stoi(addLE->displayText().toStdString());
    uint8_t val = 0;
    do {
       clk++;
       clkLb->setText(std::to_string(clk).c_str());
       std::cout<<clk<<std::endl;
    } while(_memsys->load(add, &val) == 0);
    std::cout<<"load "<<add<<": "<<val<<std::endl;
    if(_memsys->_cacheOn) {
        int h = _memsys->getHit();
        int m = _memsys->getMiss();
        std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
        hitLb->setText(std::to_string(h).c_str());
        missLb->setText(std::to_string(m).c_str());
    }
}

void Simulator::memStore() {
    int add = std::stoi(addLE->displayText().toStdString());
    uint8_t val = 0;
    val = (uint8_t)std::stoi(valLE->displayText().toStdString());
    std::cout<<"test "<<(int)val<<std::endl;
    do {
      clk++;
      clkLb->setText(std::to_string(clk).c_str());
      std::cout<<clk<<std::endl;
    } while(_memsys->store(add, val) == 0);
    std::cout<<"store "<<add<<" "<<val<<std::endl;
    if(_memsys->_cacheOn) {
        int h = _memsys->getHit();
        int m = _memsys->getMiss();
        std::cout<<"hit: "<<h<<" miss: "<<m<<std::endl;
        hitLb->setText(std::to_string(h).c_str());
        missLb->setText(std::to_string(m).c_str());
    }
}

void Simulator::clkReset() {
    clk = 0;
    clkLb->setText(std::to_string(clk).c_str());
}

void Simulator::cacheOnOFF() {
    _memsys->_cacheOn = !_memsys->_cacheOn;
    if (_memsys->_cacheOn){
        cacheOnPB->setText(tr("ON"));
        _memsys->resetHit();
        _memsys->resetMiss();
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    } else {
        cacheOnPB->setText(tr("OFF"));
        _memsys->resetHit();
        _memsys->resetMiss();
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    }
}

Simulator::~Simulator() {

}

