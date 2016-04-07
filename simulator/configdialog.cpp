#include "configdialog.h"

ConfigDialog::ConfigDialog() {
    QPushButton *okPB = new QPushButton(tr("Ok"));
    connect(okPB, SIGNAL(clicked()), this, SLOT(memsysUpdate()));
    QPushButton *cancelPB = new QPushButton(tr("Cancel"));
    connect(cancelPB, SIGNAL(clicked()), this, SLOT(close()));


    QGroupBox *ccgroup = new QGroupBox("cache");
    cacheTW = new QTabWidget;
    cacheTW->addTab(new CacheConfigs(),"level 0");
    QPushButton *addPB = new QPushButton(tr("Add"));
    connect(addPB, SIGNAL(clicked()), this, SLOT(addLevel()));
    QPushButton *rmPB = new QPushButton(tr("Remove"));
    connect(rmPB, SIGNAL(clicked()), this, SLOT(removeLevel()));
    QGridLayout *ccLayout = new QGridLayout;
    ccLayout->addWidget(cacheTW, 0, 0, 1, 2);
    ccLayout->addWidget(addPB, 1, 0);
    ccLayout->addWidget(rmPB, 1, 1);
//    ccLayout->addWidget(new QLabel("index size:"), 0, 0);
//    ccLayout->addWidget(new QLabel("line size:"), 1, 0);
//    ccLayout->addWidget(new QLabel("ways:"), 2, 0);
//    ccLayout->addWidget(new QLabel("cycle:"), 3, 0);
//    ccLayout->addWidget(new QLabel("policy:"), 4, 0);
//    ccLayout->addWidget(new QLabel("level:"), 5, 0);
//    indexSizeLE = new QLineEdit("8");
//    indexSizeLE->setMaximumWidth(40);
//    ccLayout->addWidget(indexSizeLE, 0, 1);
//    lineSizeLE = new QLineEdit("4");
//    lineSizeLE->setMaximumWidth(40);
//    ccLayout->addWidget(lineSizeLE, 1, 1);
//    waysLE = new QLineEdit("2");
//    waysLE->setMaximumWidth(40);
//    ccLayout->addWidget(waysLE, 2, 1);
//    cacheCycleLE = new QLineEdit("10");
//    cacheCycleLE->setMaximumWidth(40);
//    ccLayout->addWidget(cacheCycleLE, 3, 1);
//    policyRB = new QRadioButton("LRU");
//    policyRB->setChecked(true);
//    QRadioButton *rdRB = new QRadioButton("random");
//    QGroupBox *rbGroup = new QGroupBox;
//    QHBoxLayout *rbLayout = new QHBoxLayout;
//    rbLayout->addWidget(policyRB);
//    rbLayout->addWidget(rdRB);
//    rbGroup->setLayout(rbLayout);
//    ccLayout->addWidget(rbGroup, 4, 1);
//    cacheLeveLE = new QLineEdit("1");
//    cacheLeveLE->setMaximumWidth(40);
//    ccLayout->addWidget(cacheLeveLE, 5, 1);
    ccgroup->setLayout(ccLayout);


    QGroupBox *mmGroup = new QGroupBox("memory");
    QGridLayout *mmLayout = new QGridLayout;
    mmLayout->addWidget(new QLabel("size:"), 0, 0);
    mmLayout->addWidget(new QLabel("cycle:"), 1, 0);
    memSizeLE = new QLineEdit("1024");
    memSizeLE->setMaximumWidth(40);
    mmLayout->addWidget(memSizeLE, 0, 1);
    memCycleLE = new QLineEdit("100");
    memCycleLE->setMaximumWidth(40);
    mmLayout->addWidget(memCycleLE, 1, 1);
    mmGroup->setLayout(mmLayout);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ccgroup, 0, 0);
    layout->addWidget(mmGroup, 0, 1);
    layout->addWidget(okPB, 1, 0);
    layout->addWidget(cancelPB, 1, 1);
    setLayout(layout);
    //size = 1;
}

void ConfigDialog::memsysUpdate() {
    int indexsize = 8;
    int linesize = 4;
    int ways = 2;
    int cachecycle = 10;
    int policy = 1;
    int level = 1;
    int memsize = 1024;
    try {
        memsize = std::stoi(memSizeLE->displayText().toStdString());
    } catch(...) {
        memsize = 0;
    }
    int memcycle = 100;
    try {
        memcycle = std::stoi(memCycleLE->displayText().toStdString());
    } catch(...) {
        memcycle = 0;
    }

    MemSysConfig config;
    config.cacheLevel = 1;
    config.cacheOn = false;
    config.memCycle = memcycle;
    config.memSize = memsize;
    config.cacheSettings = std::vector<CacheSettings>();
    CacheSettings cs;
    cs.cycle = 5;
    cs.indexsize = 8;
    cs.linesize = 4;
    cs.ways = 2;
    cs.rpolicy = ReplacePolicy::LRU;
    cs.wpolicy = WritePolicy::WRITEBACK;
    config.cacheSettings.push_back(cs);
    emit memsysConfig(config);
    this->accept();
}

void ConfigDialog::close() {
    this->reject();
}

void ConfigDialog::addLevel() {
    int l = cacheTW->count();
    std::string level = "level " + std::to_string(l);
    cacheTW->addTab(new CacheConfigs(), level.c_str());
}

void ConfigDialog::removeLevel() {
    cacheTW->removeTab(cacheTW->currentIndex());
}
