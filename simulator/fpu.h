#ifndef __MIPS_Simulator__FPU__
#define __MIPS_Simulator__FPU__

class FPU {
public:
  FPU(int cycle_);
  int fpuCal(float A, float B, float *C, int opcode);
private:
  int cycle;
  int countdown;
};

#endif /* defined(__MIPS_Simulator__FPU__) */
