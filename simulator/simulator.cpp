#include "simulator.h"
#include <string>
#include <iostream>
#include <fstream>

Simulator::Simulator(CPU *cpu, MemSys* memsys, QWidget *parent) : QMainWindow(parent) {
    _cpu = cpu;
    _memsys = memsys;

    //menu
    QAction *openAct = new QAction(tr(" open "), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(memOpen()));
    QAction *saveAct = new QAction(tr(" save "), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(memSave()));
    QAction *importAct = new QAction(tr(" import "), this);
    connect(importAct, SIGNAL(triggered()), this, SLOT(memImport()));
    QAction *configAct = new QAction(this);
    configAct->setText(" configs ");
    connect(configAct, SIGNAL(triggered()), this, SLOT(memConfig()));
    QMenu *memMenu = menuBar()->addMenu(tr("Memory"));
    memMenu->addAction(openAct);
    memMenu->addAction(importAct);
    memMenu->addAction(saveAct);
    memMenu->addAction(configAct);

    //config dialog
    configDL = new ConfigDialog();
    connect(configDL, &ConfigDialog::memsysConfig, this, &Simulator::memsysInit);

    //cpu group
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

    //memory system group
    QGroupBox *memGroup = new QGroupBox(tr("Memory System"));

    //row 0
    addLE = new QLineEdit;
    addLE->setMaximumWidth(80);
    valLE = new QLineEdit;
    valLE->setMaximumWidth(80);
    QPushButton *loadPB = new QPushButton(tr("Load"));
    connect(loadPB, SIGNAL(clicked()), this, SLOT(memLoad()));
    QPushButton *storePB = new QPushButton(tr("Store"));
    connect(storePB, SIGNAL(clicked()), this, SLOT(memStore()));

    //row 1
    cacheOnPB = new QPushButton(tr("ON"));
    if (!_memsys->_config.cacheOn){
      cacheOnPB->setText(tr("OFF"));
    }
    connect(cacheOnPB, SIGNAL(clicked()), this, SLOT(cacheOnOFF()));
    hitLb = new QLabel(tr("0"));
    missLb = new QLabel(tr("0"));

    //row 2
    ccGroup = new QGroupBox(tr("Cache"));
    QGridLayout *ccLayout = new QGridLayout;
    cacheViewTW = new QTabWidget;
    ccLayout->addWidget(cacheViewTW);
    for (int i = 0; i < _memsys->_config.cacheLevel; ++i) {
       std::string l = "level " + std::to_string(i);
       cacheViewTW->addTab(new CacheView(_memsys->_config.cacheSettings[i].indexsize,
         _memsys->_config.cacheSettings[i].ways, _memsys->_config.cacheSettings[i].linesize), l.c_str());
    }


    ccGroup->setLayout(ccLayout);
    ccGroup->setVisible(_memsys->_config.cacheOn);

    //main memory
    mv = new MemoryView(_memsys->_config.memSize);
    QGroupBox *mmGroup = new QGroupBox(tr("Main Memory"));
    mvLayout = new QGridLayout;
    mvLayout->addWidget(mv, 0, 0);
    mmGroup->setLayout(mvLayout);

    QGridLayout *memLayout = new QGridLayout;
    memLayout->addWidget(new QLabel(tr("address:")), 0, 0);
    memLayout->addWidget(addLE, 0, 1);
    memLayout->addWidget(new QLabel(tr("value:")), 0, 2);
    memLayout->addWidget(valLE, 0, 3);
    memLayout->addWidget(loadPB, 0, 4);
    memLayout->addWidget(storePB, 0, 5);
    memLayout->addWidget(new QLabel("Cache:"), 1, 0);
    memLayout->addWidget(cacheOnPB, 1, 1);
    memLayout->addWidget(new QLabel(tr("total hit:")), 1, 2);
    memLayout->addWidget(hitLb, 1, 3);
    memLayout->addWidget(new QLabel(tr("total miss:")), 1, 4);
    memLayout->addWidget(missLb, 1, 5);
    memLayout->addWidget(ccGroup, 2, 0, 1, 4);
    memLayout->addWidget(mmGroup, 2, 4, 1, 3);
    memGroup->setLayout(memLayout);

    QWidget *cw = new QWidget;
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(cpuGroup, 0, 0);
    layout->addWidget(memGroup, 0, 1);
    cw->setLayout(layout);

    setCentralWidget(cw);


}

void Simulator::memOpen() {
    std::string filename =  QFileDialog::getOpenFileName(this,tr("Open Test Case"),
               "/Users/blade/workspace/cs535/MIPS_simulator").toStdString();//"/Users/blade/workspace/cs535/MIPS_simulator/GenTestCases/sequential_small.txt";
    std::fstream input(filename);
    std::string line;
    while(input){
        std::getline(input, line);
        if (line[0] == 'L') {
            uint32_t add = std::stoi(line.substr(2));
            if (add == 64) {
                std::cout<<"debug "<<add<<std::endl;

            }
            uint8_t val = 0;
            do {
               _cpu->clk++;
               clkLb->setText(std::to_string(_cpu->clk).c_str());
               //std::cout<<clk<<" ";
            } while(_memsys->loadByte(add, &val) == 0);
           // std::cout<<std::endl;
            std::cout<<"load "<<add<<": "<<(int)val<<std::endl;
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
        } else {
            return;
           // std::cout<<"unrecognized command: "<<line<<std::endl;
        }
    }
    input.close();
}

void Simulator::memSave() {
    std::fstream out("/Users/blade/workspace/cs535/output.txt");
    out<<_memsys->dump();
    out.close();
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
}

void Simulator::memImport() {
    std::string filename = QFileDialog::getOpenFileName(this,tr("Import Memory"),
               "/Users/blade/workspace/cs535/MIPS_simulator/asm").toStdString();
    std::fstream input(filename);
    uint32_t ins = 0;
    int add = 0;
    input>>ins;
    int flag = 0;
    while(input){
       //std::cout<<ins<<std::endl;
       flag =  _memsys->directWriteWord(add, ins);
       if (flag == -1) {
           std::cout<<"memImport error: "<<add<<" "<<ins<<std::endl;
       }
       add = add + 4;
       ins = 0;
       input>>ins;
    }
    input.close();
    return;
}

void Simulator::memConfig() {
    configDL->exec();
}

void Simulator::memsysInit(MemSysConfig config) {
    _memsys->init(config);
    mvLayout->removeWidget(mv);
    delete mv;
    mv = new MemoryView(_memsys->_config.memSize);
    mvLayout->addWidget(mv, 0, 0);
    int n = cacheViewTW->count();
    for (int i = 0; i < n; ++i) {
        CacheView *d =  (CacheView*) cacheViewTW->widget(i);
        cacheViewTW->removeTab(i);
        delete d;
    }
    for (int i = 0; i < _memsys->_config.cacheLevel; ++i) {
       std::string l = "level " + std::to_string(i);
       cacheViewTW->addTab(new CacheView(_memsys->_config.cacheSettings[i].indexsize,
         _memsys->_config.cacheSettings[i].ways, _memsys->_config.cacheSettings[i].linesize), l.c_str());
    }
}

void Simulator::clkReset() {
    _cpu->clk = 0;
    _cpu->err = false;
    _cpu->clear = true;
    _cpu->pc = 0;
    clkLb->setText(std::to_string(_cpu->clk).c_str());
}

void Simulator::cacheOnOFF() {
    _memsys->_config.cacheOn = !_memsys->_config.cacheOn;
    if (_memsys->_config.cacheOn){
        cacheOnPB->setText(tr("ON"));
        _memsys->resetCache();
        //TODO: reset cacheview
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    } else {
        cacheOnPB->setText(tr("OFF"));
        _memsys->resetCache();
        hitLb->setText(tr("0"));
        missLb->setText(tr("0"));
    }
    ccGroup->setVisible(_memsys->_config.cacheOn);
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

void Simulator::memUpdate(uint8_t *data, uint32_t add, int len) {
    mv->update(data, add, len);
}

void Simulator::cacheLineUpadate(int level, Cacheline** data, int idx, int way) {
    CacheView *c = (CacheView*)cacheViewTW->widget(level);
    c->lineUpdate(data, idx, way);
}

void Simulator::cacheHitUpdate(int level, int hit) {
    CacheView *c = (CacheView*)cacheViewTW->widget(level);
    c->hitUpdate(hit);
    //std::cout<<hit<<std::endl;
    int th = std::stoi(hitLb->text().toStdString()) + 1;
    std::cout<<th<<std::endl;
    hitLb->setText(std::to_string(th).c_str());
}

void Simulator::cacheMissUpdate(int level, int miss) {
    CacheView *c = (CacheView*)cacheViewTW->widget(level);
    c->missUpdate(miss);
    int tm = std::stoi(missLb->text().toStdString()) + 1;
    missLb->setText(std::to_string(tm).c_str());
}

Simulator::~Simulator() {

}
