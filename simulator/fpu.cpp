#include "fpu.h"
#include <iostream>

FPU::FPU(int cycle_): cycle(cycle_), countdown(cycle_) {

}

int FPU::fpuCal(float A, float B, float *C, int opcode) {
  if (opcode < 0 || opcode > 3) {
    std::cout<<"fpuCal: error opcode"<<std::endl;
    return -1;
  }
  if (countdown > 0) {
    --countdown;
  }
  if (countdown == 0) {
    switch(opcode) {
      case 0: *C = A + B;
            break;
      case 1: *C = A - B;
            break;
      case 2: *C = A * B;
            break;
      case 3: *C = A / B;
            break;
    }
    countdown = cycle;
    return 1;
  } else {
    return 0;
  }
}
