#ifndef __MIPS_Simulator__CPUView__
#define __MIPS_Simulator__CPUView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>

class CPUView : public QWidget {
  Q_OBJECT
public:
    CPUView() {
        clkLb = new QLabel(tr("0"));
        pcLB = new QLabel("0");
        QPushButton *clkResetPB = new QPushButton(tr("reset"));
        QPushButton *runPB = new QPushButton(tr("run"));
        QPushButton *stepPB = new QPushButton(tr("step"));
        pipedPB = new QPushButton("on");
        piped = true;
        connect(clkResetPB, SIGNAL(clicked()), this, SLOT(reset()));
        connect(runPB, SIGNAL(clicked()), this, SLOT(run()));
        connect(stepPB, SIGNAL(clicked()), this, SLOT(step()));
        connect(pipedPB, SIGNAL(clicked()), this, SLOT(pipelineSet()));
        QScrollArea *tmpSA  = new QScrollArea;
        QGroupBox *tmpGB = new QGroupBox;
        QGridLayout *tmpLayout = new QGridLayout;
        tmpLayout->addWidget(new QLabel("GPR:"), 0, 0);
        tmpLayout->addWidget(new QLabel("FPR:"), 1, 0);
        tmpLayout->addWidget(new QLabel("VR:"), 2, 0);
        for (int i = 0; i < 16; ++i) {
            gprLB[i] = new QLabel("0");
            fprLB[i] = new QLabel("0.0");
            vrLB[i] = new QLabel("0");
            tmpLayout->addWidget(gprLB[i], 0, i+1);
            tmpLayout->addWidget(fprLB[i], 1, i+1);
            tmpLayout->addWidget(vrLB[i], 2, i+1);
        }
        tmpGB->setLayout(tmpLayout);
        tmpSA->setWidget(tmpGB);
        QGridLayout *cpuLayout = new QGridLayout;
        cpuLayout->addWidget(new QLabel(tr("clock cycle:")), 0, 0);
        cpuLayout->addWidget(clkLb, 0, 1);
        cpuLayout->addWidget(new QLabel("pc:"), 0, 2);
        cpuLayout->addWidget(pcLB, 0, 3);
        cpuLayout->addWidget(tmpSA, 1, 0, 2, 4);
        cpuLayout->addWidget(new QLabel("pipeline:"), 4, 0);
        cpuLayout->addWidget(pipedPB, 4, 1);
        cpuLayout->addWidget(runPB, 5, 0);
        cpuLayout->addWidget(stepPB, 5, 1);
        cpuLayout->addWidget(clkResetPB, 5, 2);
        setLayout(cpuLayout);
    }
    QLabel *clkLb;
    QLabel *pcLB;
    QPushButton *pipedPB;
    QLabel *gprLB[16];
    QLabel *fprLB[16];
    QLabel *vrLB[16];
    bool piped;
signals:
    void cpuRun();
    void cpuStep();
    void cpuReset();
    void cpuPipelineSet(bool p);
public slots:
    void clkUpdate(int clk) {
        clkLb->setText(std::to_string(clk).c_str());
    }
    void pcUpdate(uint32_t pc) {
        pcLB->setText(std::to_string(pc).c_str());
    }
    void gprUpdate(int idx, uint32_t val) {
        gprLB[idx]->setText(std::to_string(val).c_str());
    }
    void fprUpdate(int idx, float val) {
        fprLB[idx]->setText(std::to_string(val).c_str());
    }
    void vrUpdate(int idx, uint64_t val) {
        vrLB[idx]->setText(std::to_string(val).c_str());
    }
private slots:
    void reset() {
        clkLb->setText("0");
        emit cpuReset();
    }
    void run() {
        emit cpuRun();
    }
    void step() {
        emit cpuStep();
    }
    void pipelineSet() {
        piped = !piped;
        if (piped) {
            pipedPB->setText("on");
        } else {
            pipedPB->setText("off");
        }
        clkLb->setText("0");
        emit cpuPipelineSet(piped);
    }
};


#endif
