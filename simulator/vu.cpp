#include "vu.h"
#include <iostream>
VU::VU(int cycle_): cycle(cycle_), countdown(cycle_) {

}

int VU::vuCal(uint64_t A, uint64_t B, uint64_t *C, int opcode) {
  if(opcode < 0 || opcode > 9) {
    std::cout<<"vuCal: error opcode"<<std::endl;
    return -1;
  }
  if (countdown > 0) {
    --countdown;
  }
  if (countdown == 0 ) {
    uint64_t res = 0;
    uint8_t tmpA[8];
    uint8_t tmpB[8];
    uint8_t tmpC[8];
    for (int i=7; i>=0; --i) {
      tmpA[i] = A & 0xff;
      tmpB[i] = B & 0xff;
      A >>=8;
      B >>=8;
    }
    if (opcode == 0) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] + tmpB[i];
      }
    } else if (opcode == 1) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] - tmpB[i];
      }
    } else if (opcode == 2) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] * tmpB[i];
      }
    } else if (opcode == 3) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] / tmpB[i];
      }
    } else if (opcode == 4) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] % tmpB[i];
      }
    } else if (opcode == 5) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] == tmpB[i];
      }
    } else if (opcode == 6) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = (int8_t)tmpA[i] <= (int8_t)tmpB[i];
      }
    } else if (opcode == 7) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] < tmpB[i];
      }
    } else if (opcode == 8) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = (int8_t)tmpA[i] < (int8_t)tmpB[i];
      }
    } else if (opcode == 9) {
      for(int i=0; i<8; ++i) {
        tmpC[i] = tmpA[i] < tmpB[i];
      }
    }
    res = tmpC[0];
    for(int i=1; i<8; ++i){
      res <<= 8;
      res |= tmpC[i];
    }
    *C = res;
    countdown = cycle;
    return 1;
  } else {
    return 0;
  }
}
