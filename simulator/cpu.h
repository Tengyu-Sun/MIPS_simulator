#ifndef __MIPS_Simulator__CPU__
#define __MIPS_Simulator__CPU__

#include <cstdint>
#include <QObject>
#include "memsys.h"
#include "fpu.h"
#include "vu.h"

struct Instruction {
  uint32_t add;
  uint32_t npc;
  uint32_t ins;
  int type;
  int opcode;
  int rd1;
  uint32_t A;
  uint64_t vA;
  float fA;
  int rd2;
  uint32_t B;
  uint64_t vB;
  float fB;
  int rd3;
  int imm;
  int stage;
  uint32_t aluoutput;
  float fpuoutput;
  uint64_t vuoutput;
  uint32_t lmd;
  bool cond;
  int dst; //gpr:0-15 fpr:16-31 vr:32-47
};

class CPU : public QObject {
    Q_OBJECT
public:
  uint64_t clk;
  CPU(MemSys* memsys, FPU* fpu, VU* vu);
  void run();
  void step();
  void setPipeline(bool p) {
      piped = p;
      reset();
  }
  void reset();
  bool err;
  uint32_t pc;
  bool clear;
  bool piped;
signals:
  void gprNotify(int idx, uint32_t val);
  void fprNotify(int idx, float val);
  void vrNotify(int idx, uint64_t val);
private:
  uint32_t gpr[16];  //general purpose register
  float fpr[16];  //floating point register
  uint64_t vr[16];  //vector register
  uint32_t status;

  Instruction *pipe[5];

  MemSys* _memsys;
  FPU* _fpu;
  VU* _vu;

  void ifc();
  void idc();
  void exc();
  void mem();
  void wbc();
  bool opReady(int rd, int ab);
};
#endif /* defined(__MIPS_Simulator__CPU__) */
