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
    pipe[0]->npc = pc + 4;
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
      uint32_t ins = pipe[1]->ins;
      pipe[1]->imm = ins%0x20000; //sign-extended?
      if ((pipe[1]->imm) >> 16 == 1) {
        pipe[1]->imm |= 0xfffe0000;
      }
      ins = ins >> 13;
      pipe[1]->rd3 = ins%16;
      ins = ins >> 4;
      pipe[1]->rd2 = ins%16;
      ins = ins >> 4;
      pipe[1]->rd1 = ins%16;
      ins = ins >> 4;
      pipe[1]->opcode = ins;
      ins = ins >> 4;
      switch(ins) {
        case 4: pipe[1]->type = 1;
                break;
        case 0:
        case 1: pipe[1]->type = 2;
                break;
        case 5: pipe[1]->type = 3;
                break;
        case 6: pipe[1]->type = 4;
                break;
        case 7: pipe[1]->type = 5;
                break;
        case 2:
        case 3: pipe[1]->type = 6;
                break;
      }
      if (pipe[1]->type == 1) {//data transfer
        pipe[1]->A = gpr[pipe[1]->rd1];
        pipe[1]->opcode = pipe[1]->opcode%16;
        if (pipe[1]->opcode > 7) {
          pipe[1]->B = gpr[pipe[1]->rd2];
        } else {
          if (pipe[1]->opcode == 4){
            pipe[1]->rf2 = 2;//fpr
          } else {
            pipe[1]->rf2 = 1;//gpr
          }
        }
      }

      std::cout<<"idc: type:"<<pipe[1]->type<<" opcode: "<<pipe[1]->opcode<<" rd1: "
      <<pipe[1]->rd1<<" rd2: "<<pipe[1]->rd2<<" rd3: "<<pipe[1]->rd3<<" imm: "<<pipe[1]->imm<<std::endl;
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
      if (pipe[2]->type == 1) {
        pipe[2]->aluoutput = pipe[2]->A + pipe[2]->imm;
      }
      std::cout<<"exc: "<<pipe[2]->aluoutput<<std::endl;
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
      pc = pipe[3]->npc;
      if (pipe[3]->type == 1) {
        if (pipe[3]->opcode == 8) {
          int flag = _memsys->storeByte(pipe[3]->aluoutput, (uint8_t)(pipe[3]->B%256));
          if (flag == 1) {
            pipe[3]->stage = 4;
            std::cout<<"pc: "<<pc<<std::endl;
          }
        } else if (pipe[3]->opcode == 9) {
          int flag = _memsys->storeWord(pipe[3]->aluoutput, (uint32_t)(pipe[3]->B));
          if (flag == 1) {
            pipe[3]->stage = 4;
            std::cout<<"pc: "<<pc<<std::endl;
          }
        } else if (pipe[3]->opcode == 0) {
          uint8_t tmp;
          int flag = _memsys->loadByte(pipe[3]->aluoutput, &tmp);
          if (flag == 1) {
            pipe[3]->stage = 4;
            pipe[3]->lmd = (uint32_t)tmp;
            if (tmp >> 7 == 1) {
              pipe[3]->lmd |= 0xffffff00;
            }
            std::cout<<"pc: "<<pc<<std::endl;
          }
        } else if (pipe[3]->opcode == 1) {
          uint8_t tmp;
          int flag = _memsys->loadByte(pipe[3]->aluoutput, &tmp);
          if (flag == 1) {
            pipe[3]->stage = 4;
            pipe[3]->lmd = tmp;
            std::cout<<"pc: "<<pc<<std::endl;
          }
        } else if (pipe[3]->opcode == 2 || pipe[3]->opcode == 4) {
          uint32_t tmp;
          int flag = _memsys->loadWord(pipe[3]->aluoutput, &tmp);
          if (flag == 1) {
            pipe[3]->stage = 4;
            pipe[3]->lmd = tmp;
            std::cout<<"pc: "<<pc<<std::endl;
          }
        }
      }
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
      if (pipe[4]->type == 1) {
        if (pipe[4]->opcode == 0 || pipe[4]->opcode == 1 || pipe[4]->opcode == 2) {
          gpr[pipe[4]->rd2] = pipe[4]->lmd;
          std::cout<<"wbc: "<<gpr[pipe[4]->rd2]<<std::endl;
          pipe[4]->stage = 5;
        } else if (pipe[4]->opcode == 4) {
          fpr[pipe[4]->rd2] = pipe[4]->lmd;
          std::cout<<"wbc: "<<fpr[pipe[4]->rd2]<<std::endl;
          pipe[4]->stage = 5;
        }
      }
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
