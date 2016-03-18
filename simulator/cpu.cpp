#include "cpu.h"
#include <iostream>

CPU::CPU(MemSys* memsys){
  _memsys = memsys;
  //ins = 0;
  pc = 0;
  clk = 0;
  stage = 0;
  err = false;
  for(int i=0; i<16; ++i) {
    gpr[i] = 0;
    fpr[i] = 0;
    vr[i] = 0;
  }
  for(int i=0; i<5; ++i) {
    pipe[i] = nullptr;
  }
  clear = true;
}

void CPU::ifc() {

  if (pipe[0] == nullptr && clear) {
    pipe[0] = new Instruction();
    pipe[0]->add = (int)pc;
    pipe[0]->stage = 0;
    clear = false;
  }
  if (pipe[0] != nullptr) {
    if (pipe[0]->stage == 0) {
      int flag = _memsys->loadWord(pipe[0]->add, &(pipe[0]->ins));
      if (flag == -1) {
        std::cout<<"error fectching instructions"<<std::endl;
        err = true;
      } else if (flag == 1) {
        pipe[0]->stage = 1;
        std::cout<<"instruction fectched"<<std::endl;
      }
    }
    if (pipe[0]->stage == 1){
      if (pipe[1] == nullptr){
        pipe[1] = pipe[0];
        pipe[0] = nullptr;
      }
    } else if (pipe[0]->stage > 1) {
      std::cout<<"ifc: error instruction stage"<<std::endl;
      err = true;
    }
  }
  return;
}

void CPU::idc() {
  if (pipe[1] != nullptr) {
    if (pipe[1]->stage == 1) {
      pipe[1]->opcode = (int)((pipe[1]->ins)>>25);
      std::cout<<"idc: "<<pipe[1]->opcode<<std::endl;
      pipe[1]->stage = 2;
    }
    if (pipe[1]->stage == 2) {
      if (pipe[2] == nullptr) {
        pipe[2] = pipe[1];
        pipe[1] = nullptr;
      }
    } else if (pipe[1]->stage > 2) {
      std::cout<<"idc: error instruction stage"<<std::endl;
      err = true;
    }
  }
  return;
}

void CPU::exc() {
  if (pipe[2] != nullptr) {
    if (pipe[2]->stage == 2) {
      std::cout<<"exc: "<<pipe[2]->opcode<<std::endl;
      pipe[2]->stage = 3;
    }
    if (pipe[2]->stage == 3) {
      if (pipe[3] == nullptr) {
        pipe[3] = pipe[2];
        pipe[2] = nullptr;
      }
    } else if (pipe[2]->stage > 3) {
      std::cout<<"exc: error instruction stage"<<std::endl;
      err = true;
    }
  }
  return;
}

void CPU::mem() {
  if (pipe[3] != nullptr) {
    if (pipe[3]->stage == 3) {
      pc = pc + 4;
      std::cout<<"pc: "<<pc<<std::endl;
      pipe[3]->stage = 4;
    }
    if(pipe[3]->stage == 4) {
      if (pipe[4] == nullptr) {
        pipe[4] = pipe[3];
        pipe[3] = nullptr;
      }
    } else if (pipe[3]->stage > 4) {
      std::cout<<"mem: error instruction stage"<<std::endl;
      err = true;
    }
  }
  return;
}

void CPU::wbc() {
  if (pipe[4] != nullptr) {
    if (pipe[4]->stage == 4) {
      std::cout<<"wbc: "<<pipe[4]->opcode<<std::endl;
      pipe[4]->stage = 5;
    }
    if(pipe[4]->stage == 5) {
      clear = true;
      delete pipe[4];
      pipe[4] = nullptr;
    } else if (pipe[4]->stage > 5){
      std::cout<<"wbc: error instruction stage"<<std::endl;
      err = true;
    }
  }
  return;
}

void CPU::run() {
  while(!err) {
    wbc();
    mem();
    exc();
    idc();
    ifc();
    ++clk;
  }
}

void CPU::step() {
  if (err) {
    return;
  }
  wbc();
  mem();
  exc();
  idc();
  ifc();
  ++clk;
  return;
}
