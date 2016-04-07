#ifndef MIPS_Simulator_ConfigDialog
#define MIPS_Simulator_ConfigDialog

#include <QObject>
#include <QtWidgets>
#include "memsys.h"

class ConfigDialog : public QDialog {
    Q_OBJECT
public:
    ConfigDialog();
signals:
    void memsysConfig(MemSysConfig config);
private slots:
    void memsysUpdate();
    void close();
    void addLevel();
    void removeLevel();
private:
    QTabWidget *cacheTW;
    QLineEdit *memSizeLE;
    QLineEdit *memCycleLE;
};

class CacheConfigs : public QWidget {
  Q_OBJECT
public:
    CacheConfigs() {
        QGridLayout *ccLayout = new QGridLayout;
        ccLayout->addWidget(new QLabel("index size:"), 0, 0);
        ccLayout->addWidget(new QLabel("line size:"), 1, 0);
        ccLayout->addWidget(new QLabel("ways:"), 2, 0);
        ccLayout->addWidget(new QLabel("cycle:"), 3, 0);
        ccLayout->addWidget(new QLabel("policy:"), 4, 0);

        indexSizeLE = new QLineEdit("8");
        indexSizeLE->setMaximumWidth(40);
        ccLayout->addWidget(indexSizeLE, 0, 1);

        lineSizeLE = new QLineEdit("4");
        lineSizeLE->setMaximumWidth(40);
        ccLayout->addWidget(lineSizeLE, 1, 1);

        waysLE = new QLineEdit("2");
        waysLE->setMaximumWidth(40);
        ccLayout->addWidget(waysLE, 2, 1);

        cacheCycleLE = new QLineEdit("10");
        cacheCycleLE->setMaximumWidth(40);
        ccLayout->addWidget(cacheCycleLE, 3, 1);

        policyRB = new QRadioButton("LRU");
        policyRB->setChecked(true);
        QRadioButton *rdRB = new QRadioButton("random");
        QGroupBox *rbGroup = new QGroupBox;
        QHBoxLayout *rbLayout = new QHBoxLayout;
        rbLayout->addWidget(policyRB);
        rbLayout->addWidget(rdRB);
        rbGroup->setLayout(rbLayout);
        ccLayout->addWidget(rbGroup, 4, 1);
        setLayout(ccLayout);
    }
    QLineEdit *indexSizeLE;
    QLineEdit *lineSizeLE;
    QLineEdit *waysLE;
    QLineEdit *cacheCycleLE;
    //QLineEdit *cacheLeveLE;
    QRadioButton *policyRB;
};
#endif
