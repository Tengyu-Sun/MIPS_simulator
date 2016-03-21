#ifndef __MIPS_Simulator__CPU__
#define __MIPS_Simulator__CPU__

#include <cstdint>
#include "memsys.h"

struct Instruction {
  int add;
  uint32_t npc;
  uint32_t ins;
  int type;
  int opcode;
  int rd1;
  uint32_t A;
  int rd2;
  uint32_t B;
  int rd3;
  uint32_t imm;
  int stage;
  uint32_t aluoutput;
  uint32_t lmd;
  bool cond;
};

class CPU {
public:
  uint64_t clk;
  CPU(MemSys* memsys);
  void run();
  void step();

private:
  uint32_t gpr[16];  //general purpose register
  float fpr[16];  //floating point register
  uint64_t vr[16];  //vector register
  uint32_t pc;
  uint32_t status;

  Instruction *pipe[5];
  bool err;
  bool clear;
  MemSys* _memsys;

  void ifc();
  void idc();
  void exc();
  void mem();
  void wbc();
};
#endif /* defined(__MIPS_Simulator__CPU__) */
