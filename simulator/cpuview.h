#ifndef __MIPS_Simulator__CPUView__
#define __MIPS_Simulator__CPUView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>
#include <map>
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
                insLB[i]->setText(decode(pipe[i]->ins).c_str());
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
private:
    std::string decode(uint32_t code) {
        // get highest 7 bits -> opcode
        int opcode = (code & 0xfe000000) >> 25;
        // lookup from the opcode map
        std::string op = "";
        if(opcode == 0) return "break";

        if(opcode_decode_map.find(opcode) == opcode_decode_map.end()) {
            std::cout << "error opcode" << std::endl;
            return "";
        } else {
            op = opcode_decode_map.find(opcode)->second;
        }
        int inst_type = 0;
        if(type_map.find(op) == type_map.end()) {
            std::cout << "instruction type not supported" << std::endl;
            return "";
        } else {
            inst_type = type_map.find(op)->second;
        }
        if(inst_type == 1) {
            int offset = 0x01ffffff & code;
            return op + " " + std::to_string(offset);
        } else if(inst_type == 2) {
            int reg1 =   (code & 0x01e00000) >> 21;
            int reg2 =   (code & 0x001e0000) >> 17;
            int offset = (code & 0x0001ffff);
            return op + " " + "$" + std::to_string(reg1) + "," + "$" + std::to_string(reg2) + "," + std::to_string(offset);
        } else if(inst_type == 3) {
            int reg1 =  (code & 0x01e00000) >> 21;
            int reg2 =  (code & 0x001e0000) >> 17;
            int reg3 =  (code & 0x0001e000) >> 13;
            return op + " " + "$" + std::to_string(reg1) + "," + "$" + std::to_string(reg2) + "," + "$" + std::to_string(reg3);
        } else {
            std::cout << "error instruction type" << std::endl;
            return "...";
        }
    }
    std::map<uint32_t, std::string> opcode_decode_map = {
        //data transfer
        {0b0010000,      "lb"},
        {0b0010001,     "lbu"},
        {0b0011000,      "sb"},
        {0b0010010,      "lw"},
        {0b0011001,      "sw"},
        {0b0010100,     "lsp"},
        //arithmetic
        {0b1000000,     "add"},
        {0b1000001,     "sub"},
        {0b1010011,    "addi"},
        {0b1010100,    "subi"},
        {0b1000010,     "mul"},
        {0b1000011,     "muh"},
        {0b1000100,    "mulu"},
        {0b1000101,    "muhu"},
        {0b1000110,     "div"},
        {0b1000111,    "divu"},
        {0b1001000,    "modu"},
        {0b1001001,     "and"},
        {0b1001010,      "or"},
        {0b1001011,     "not"},
        {0b1001100,     "xor"},
        {0b1001101,      "rr"},
        {0b1001110,     "srl"},
        {0b1001111,     "sra"},
        {0b1010000,      "sl"},
        {0b1010001,     "slt"},
        {0b1010010,    "sltu"},
        {0b1010101,    "slti"},
        {0b1010110,   "sltiu"},
        //control
        {0b0000001,       "j"},
        {0b0000010,     "jal"},
        {0b0000011,     "beq"},
        {0b0000100,    "bneq"},
        {0b0000101,    "bgez"},
        {0b0000110,    "bgtz"},
        {0b0000111,    "blez"},
        {0b0001000,    "bltz"},
        {0,           "break"},
        //floating point
        {0b0100000,   "addsp"},
        {0b0100001,   "subsp"},
        {0b0100010,   "mulsp"},
        {0b0100011,   "divsp"},
        {0b0100100,   "sltsp"},
        {0b0100101,    "witf"},
        {0b0100110,    "wfti"},
        //cache
        {0b0110000,    "pref"},
        //simd
        {0b1101010,    "move"},
        {0b1101011,   "copys"},
        {0b1101100,   "copyu"},
        {0b1101101, "insertb"},
        {0b1101110,   "fillb"},
        {0b1100000,   "vaddb"},
        {0b1100001,   "vsubb"},
        {0b1100010,   "vmulb"},
        {0b1100011,   "vdivb"},
        {0b1100100,   "vmodb"},
        {0b1100101,    "ceqb"},
        {0b1100110,    "cleb"},
        {0b1100111,   "cleub"},
        {0b1101000,    "cltb"},
        {0b1101001,   "cltub"}
    };
    std::map<std::string, uint32_t> type_map = {
        //data transfer
        {"lb",      2},
        {"lbu",     2},
        {"sb",      2},
        {"lw",      2},
        {"sw",      2},
        {"lsp",     2},
        //arithmetic
        {"add",     3},
        {"sub",     3},
        {"addi",    2},
        {"subi",    2},
        {"mul",     3},
        {"muh",     3},
        {"mulu",    3},
        {"muhu",    3},
        {"div",     3},
        {"divu",    3},
        {"modu",    3},
        {"and",     3},
        {"or",      3},
        {"not",     3},
        {"xor",     3},
        {"rr",      3},
        {"srl",     3},
        {"sra",     3},
        {"sl",      3},
        {"slt",     3},
        {"sltu",    3},

        {"slti",    2},
        {"sltiu",   2},
        //control
        {"j",       1},
        {"jal",     1},
        {"beq",     2},
        {"bneq",    2},
        {"bgez",    2},
        {"bgtz",    2},
        {"blez",    2},
        {"bltz",    2},
        {"break",   1}, //?
        //floating point
        {"addsp",   3},
        {"subsp",   3},
        {"mulsp",   3},
        {"divsp",   3},
        {"sltsp",   3},
        {"witf",    3},
        {"wfti",    3},
        //cache
        {"pref",    2}, //?
        //simd
        {"move",    2},
        {"copys",   2},
        {"copyu",   2},
        {"insertb", 2},
        {"fillb",   2},
        {"vaddb",   3},
        {"vsubb",   3},
        {"vmulb",   3},
        {"vdivb",   3},
        {"vmodb",   3},
        {"ceqb",    3},
        {"cleb",    3},
        {"cleub",   3},
        {"cltb",    3},
        {"cltub",   3}
    };
};

#endif
