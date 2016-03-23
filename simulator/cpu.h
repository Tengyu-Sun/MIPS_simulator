#ifndef __MIPS_Simulator__CPU__
#define __MIPS_Simulator__CPU__

#include <cstdint>
#include "memsys.h"
#include "fpu.h"
#include "vu.h"

struct Instruction {
  int add;
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
  uint32_t imm;
  int stage;
  uint32_t aluoutput;
  float fpuoutput;
  uint32_t lmd;
  bool cond;
};

class CPU {
public:
  uint64_t clk;
  CPU(MemSys* memsys, FPU* fpu, VU* vu);
  void run();
  void step();
  bool err;

private:
  uint32_t gpr[16];  //general purpose register
  float fpr[16];  //floating point register
  uint64_t vr[16];  //vector register
  uint32_t pc;
  uint32_t status;

  Instruction *pipe[5];
  bool clear;
  MemSys* _memsys;
  FPU* _fpu;
  VU* _vu;

  void ifc();
  void idc();
  void exc();
  void mem();
  void wbc();
};
#endif /* defined(__MIPS_Simulator__CPU__) */
