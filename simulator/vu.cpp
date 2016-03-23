#include "vu.h"
#include <iostream>
VU::VU(int cycle_): cycle(cycle_), countdown(cycle_) {

}

int VU::vuCal(uint64_t A, uint64_t B, uint64_t *C, int opcode) {
  if(opcode < 0 || opcode > 4) {
    std::cout<<"vuCal: error opcode"<<std::endl;
    return -1;
  }
  if (countdown > 0) {
    --countdown;
  }
  if (countdown == 0 ) {
    switch(opcode) {
      case 0:
    }
  } else {
    return 0;
  }
}
