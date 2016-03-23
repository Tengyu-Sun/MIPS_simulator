#ifndef __MIPS_Simulator__VU__
#define __MIPS_Simulator__VU__

#include<cstdint>

class VU {
public:
  VU(int cycle_);
  int vuCal(uint64_t A, uint64_t B, uint64_t *C, int opcode);
private:
  int cycle;
  int countdown;
};

#endif /* defined(__MIPS_Simulator__VU__) */
