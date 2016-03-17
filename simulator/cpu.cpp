#include "cpu.h"
#include <iostream>

CPU::CPU(MemSys* memsys){
  _memsys = memsys;
  ins = 0;
  pc = 0;
  clk = 0;
  stage = 0;
  err = false;
  for(int i=0; i<16; ++i) {
    gpr[i] = 0;
    fpr[i] = 0;
    vr[i] = 0;
  }
}

void CPU::ifc() {

  int flag = _memsys->loadWord((int)pc, &ins);
  if (flag == -1) {
    std::cout<<"error fectching instructions"<<std::endl;
    err = true;
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
  if(stage == 4) {
    pc = pc + 4;
  }
  return;
}

void CPU::wbc() {
  return;
}

void CPU::run() {
  while(!err) {
    wbc();
    stage = (stage + 1)%5;
    mem();
    stage = (stage + 1)%5;
    exc();
    stage = (stage + 1)%5;
    idc();
    stage = (stage + 1)%5;
    ifc();
    ++clk;
    stage = (stage + 1)%5;
  }
}

void CPU::step() {
  if (err) {
    return;
  }
  switch(stage) {
    case 0: wbc();
            break;
    case 1: mem();
            break;
    case 2: exc();
            break;
    case 3: idc();
            break;
    case 4: ifc();
            break;
    default: std::cout<<"wrong stage\n";
            return;
  }
  ++clk;
  stage = (stage + 1)%5;
  return;
}
