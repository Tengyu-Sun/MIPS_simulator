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
}

void ConfigDialog::memsysUpdate() {
    MemSysConfig config;
    config.cacheLevel = cacheTW->count();
    if (config.cacheLevel > 0) {
      config.cacheOn = true;
      config.cacheSettings = std::vector<CacheSettings>(config.cacheLevel);
      CacheConfigs *cur = nullptr;
      for (int i = 0; i < config.cacheLevel; ++i) {
         cur = (CacheConfigs*) cacheTW->widget(i);
         try {
            config.cacheSettings[i].indexsize = std::stoi(cur->indexSizeLE->displayText().toStdString());
         } catch(...) {
           config.cacheSettings[i].indexsize = 0;
         }
         try {
            config.cacheSettings[i].linesize = std::stoi(cur->lineSizeLE->displayText().toStdString());
         } catch(...) {
           config.cacheSettings[i].linesize = 0;
         }
         try {
            config.cacheSettings[i].ways = std::stoi(cur->waysLE->displayText().toStdString());
         } catch(...) {
           config.cacheSettings[i].ways = 0;
         }
         try {
            config.cacheSettings[i].cycle = std::stoi(cur->cacheCycleLE->displayText().toStdString());
         } catch(...) {
           config.cacheSettings[i].cycle = 0;
         }
         if(cur->rpolicyRB->isChecked()) {
            config.cacheSettings[i].rpolicy = ReplacePolicy::LRU;
         } else {
            config.cacheSettings[i].rpolicy = ReplacePolicy::RANDOM;
         }
         if(cur->wpolicyRB->isChecked()) {
            config.cacheSettings[i].wpolicy = WritePolicy::WRITEBACK;
         } else {
            config.cacheSettings[i].wpolicy = WritePolicy::WRITETROUGH;
         }
      }
    } else {
      config.cacheOn = false;
    }

    int memsize;
    try {
        memsize = std::stoi(memSizeLE->displayText().toStdString());
    } catch(...) {
        memsize = 0;
    }
    int memcycle;
    try {
        memcycle = std::stoi(memCycleLE->displayText().toStdString());
    } catch(...) {
        memcycle = 0;
    }

    config.memCycle = memcycle;
    config.memSize = memsize;

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
    int n = cacheTW->count();
    for (int i = 0; i < n; ++i) {
        std::string level = "level " + std::to_string(i);
        cacheTW->setTabText(i, level.c_str());
    }
}
