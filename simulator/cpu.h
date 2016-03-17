#ifndef __MIPS_Simulator__CPU__
#define __MIPS_Simulator__CPU__

#include <cstdint>
#include "memsys.h"

struct Instruction {
  int type;
  int opcode;
  int rd1;
  int rd2;
  int rd3;
  int imd;
};

class CPU {
public:
  CPU(MemSys* memsys);
  void run();
  void step();

private:
  uint32_t gpr[16];  //general purpose register
  uint32_t fpr[16];  //floating point register
  uint64_t vr[16];  //vector register
  uint32_t pc;
  uint32_t ins;
  uint64_t clk;
  int stage;
  bool err;
  MemSys* _memsys;

  void ifc();
  void idc();
  void exc();
  void mem();
  void wbc();
};


#endif /* defined(__MIPS_Simulator__CPU__) */
