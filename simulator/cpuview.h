#ifndef __MIPS_Simulator__CPUView__
#define __MIPS_Simulator__CPUView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>
#include "commons.h"

class CPUView : public QWidget {
  Q_OBJECT
public:
    CPUView() {
        clkLb = new QLabel(tr("0"));
        pcLB = new QLabel("0");
        QPushButton *clkResetPB = new QPushButton(tr("reset"));
        runPB = new QPushButton(tr("run"));
        QPushButton *stepPB = new QPushButton(tr("step"));
        QPushButton *exePB = new QPushButton(tr("exe"));
        pipedPB = new QPushButton("on");
        piped = true;
        connect(clkResetPB, SIGNAL(clicked()), this, SLOT(reset()));
        connect(runPB, SIGNAL(clicked()), this, SLOT(run()));
        connect(stepPB, SIGNAL(clicked()), this, SLOT(step()));
        connect(exePB, SIGNAL(clicked()), this, SLOT(exe()));
        connect(pipedPB, SIGNAL(clicked()), this, SLOT(pipelineSet()));
        //QScrollArea *tmpSA  = new QScrollArea;
        QGroupBox *tmpGB = new QGroupBox;
        QGridLayout *tmpLayout = new QGridLayout;
        tmpLayout->addWidget(new QLabel("GPR:"), 0, 0, 2, 1);
        tmpLayout->addWidget(new QLabel("FPR:"), 2, 0, 2, 1);
        tmpLayout->addWidget(new QLabel("VR:"), 4, 0, 2, 1);
        for (int i = 0; i < 8; ++i) {
            gprLB[i] = new QLabel("0");
            gprLB[i+8] = new QLabel("0");
            fprLB[i] = new QLabel("0.0");
            fprLB[i+8] = new QLabel("0.0");
            vrLB[i] = new QLabel("0");
            vrLB[i+8] = new QLabel("0");
            tmpLayout->addWidget(gprLB[i], 0, i+1);
            tmpLayout->addWidget(gprLB[i+8], 1, i+1);
            tmpLayout->addWidget(fprLB[i], 2, i+1);
            tmpLayout->addWidget(fprLB[i+8], 3, i+1);
            tmpLayout->addWidget(vrLB[i], 4, i+1);
            tmpLayout->addWidget(vrLB[i+8], 5, i+1);
        }
        tmpGB->setLayout(tmpLayout);
       // tmpSA->setWidget(tmpGB);
        QGroupBox *pipGroup = new QGroupBox;
        QGridLayout *pipLayout = new QGridLayout;
        for (int i = 0; i < 5; ++i) {
            insLB[i] = new QLabel("...");
            std::string s = "stage "+std::to_string(i+1)+":";
            pipLayout->addWidget(new QLabel(s.c_str()), i, 0);
            pipLayout->addWidget(insLB[i], i, 1);
        }
        pipGroup->setLayout(pipLayout);
        QGridLayout *cpuLayout = new QGridLayout;
        cpuLayout->addWidget(new QLabel(tr("clock cycle:")), 0, 0);
        cpuLayout->addWidget(clkLb, 0, 1);
        cpuLayout->addWidget(new QLabel("pc:"), 0, 2);
        cpuLayout->addWidget(pcLB, 0, 3);
        cpuLayout->addWidget(tmpGB, 1, 0, 1, 4);
        cpuLayout->addWidget(new QLabel("pipeline:"), 4, 0);
        cpuLayout->addWidget(pipedPB, 4, 1);
        cpuLayout->addWidget(pipGroup, 5, 0, 1, 4);
        cpuLayout->addWidget(runPB, 6, 0);
        cpuLayout->addWidget(exePB, 6, 1);
        cpuLayout->addWidget(stepPB, 6, 2);
        cpuLayout->addWidget(clkResetPB, 6, 3);
        setLayout(cpuLayout);

    }
    QLabel *clkLb;
    QLabel *pcLB;
    QLabel *insLB[5];
    QPushButton *pipedPB;
    QPushButton *runPB;
    QLabel *gprLB[16];
    QLabel *fprLB[16];
    QLabel *vrLB[16];
    bool piped;
signals:
    void cpuRun();
    void cpuExe();
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
    void pipeUpdate(Instruction** pipe) {
        if (pipe[0] != nullptr) {
            std::string fins = "fetch ins at: "+std::to_string(pipe[0]->add);
            insLB[0]->setText(fins.c_str());
        } else {
            insLB[0]->setText("...");
        }
        for (int i = 1; i < 5; ++i) {
            if (pipe[i] != nullptr) {
                insLB[i]->setText(std::to_string(pipe[i]->ins).c_str());
            } else {
                insLB[i]->setText("...");
            }
        }
    }
    void resetrunPB() {
       runPB->setText("run");
    }

private slots:
    void reset() {
        clkLb->setText("0");
        pcLB->setText("0");
        for (int i = 0; i < 16; ++i) {
            gprLB[i]->setText("0");
            fprLB[i]->setText("0.0");
            vrLB[i]->setText("0");
        }
        for (int i = 0; i< 5; ++i) {
             insLB[i]->setText("...");
        }
        emit cpuReset();

    }
    void run() {
        emit cpuRun();
        if (runPB->text() == "run") {
            runPB->setText("stop");
        } else {
            runPB->setText("run");
        }
    }
    void exe() {
        emit cpuExe();
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
