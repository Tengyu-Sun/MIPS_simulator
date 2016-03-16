#include "cpu.h"
#include <iostream>

CPU::CPU(MemSys* memsys){
  _memsys = memsys;
  ins = 0;
  pc = 0;
  clk = 0;
  for(int i=0; i<16; ++i) {
    gpr[i] = 0;
    fpr[i] = 0;
    vr[i] = 0;
  }
}

void CPU::ifc() {
  uint8_t tmp[4];
  int flag = _memsys->load((int)pc, tmp, 4);
  if ( flag == 1) {
    ins = (tmp[0]<<24) | (tmp[1]<<16) | (tmp[2]<<8) | tmp[3];
    pc = pc + 4;
  } else if (flag == -1) {
    std::cout<<"error"<<std::endl;
  }
  return;
}

void CPU::idc() {
  return;
}

void CPU::exc() {
  return;
}

void CPU::mem() {
  return;
}

void CPU::wbc() {
  return;
}
