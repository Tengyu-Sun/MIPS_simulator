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
    QGridLayout *cpuLayout = new QGridLayout;
    cpuLayout->addWidget(ccLb, 0, 0);
    cpuLayout->addWidget(clkLb, 0, 1);
    cpuGroup->setLayout(cpuLayout);

    QGroupBox *memGroup = new QGroupBox(tr("Memory"));
    addLE = new QLineEdit;
    valLE = new QLineEdit;
    loadPB = new QPushButton(tr("Load"));
    connect(loadPB, SIGNAL(clicked()), this, SLOT(memLoad()));
    storePB = new QPushButton(tr("Store"));
    connect(storePB, SIGNAL(clicked()), this, SLOT(memStore()));
    memTW = new QTableWidget(5,2);
    cacheTW = new QTableWidget(5,2);

    QGridLayout *memLayout = new QGridLayout;
    memLayout->addWidget(addLE, 0, 0);
    memLayout->addWidget(loadPB, 0, 1);
    memLayout->addWidget(valLE, 0, 2);
    memLayout->addWidget(storePB, 0, 3);
    memLayout->addWidget(cacheTW, 1, 0);
    memLayout->addWidget(memTW, 1, 1);
    memGroup->setLayout(memLayout);

    QWidget *cw = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cpuGroup, 0, 0);
    layout->addWidget(memGroup, 0, 1);
    cw->setLayout(layout);

    setCentralWidget(cw);

}

void Simulator::memOpen() {
    std::string filename = QFileDialog::getOpenFileName(this,tr("Open Text Case"),
               "/Users/blade/workspace/cs535/").toStdString();
    std::fstream input(filename);
    std::string line;
    while(input){
        std::getline(input, line);
        if (line[0] == 'L') {
            int add = std::stoi(line.substr(2));
            int val = 0;

            do {
               clk++;
               clkLb->setText(std::to_string(clk).c_str());
               //std::cout<<clk<<" ";
            } while(_memsys->load(add, &val) == 0);
           // std::cout<<std::endl;
            std::cout<<"load "<<add<<": "<<val<<std::endl;
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
    int val;
    do {
       clk++;
       clkLb->setText(std::to_string(clk).c_str());
       std::cout<<clk<<std::endl;
    } while(_memsys->load(0, &val) == 0);
}

void Simulator::memStore() {
    int val = 10;
    do {
      clk++;
      clkLb->setText(std::to_string(clk).c_str());
      std::cout<<clk<<std::endl;
    } while(_memsys->store(0, val) == 0);
}



Simulator::~Simulator() {

}

