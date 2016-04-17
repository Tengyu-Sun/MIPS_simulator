#ifndef __MIPS_Simulator__Commons__
#define __MIPS_Simulator__Commons__
#include <cstdint>
#include <vector>

struct Cacheline {
    int tag;
    int lru;
    uint8_t *data;
    bool valid;
    bool dirty;
    Cacheline() {
      valid = false;
      dirty = false;
      tag = 0;
      lru = -1;
      data = nullptr;
    }
    ~Cacheline() {
      delete[] data;
    }
};

enum class ReplacePolicy {RANDOM, LRU};
enum class WritePolicy {WRITEBACK, WRITETROUGH};

struct Position {
  int idx;
  int way;
  Position() {
    idx = -1;
    way = -1;
  }
  Position(int i, int j) {
    idx = i;
    way = j;
  }
};

struct CacheSettings {
  int linesize;
  int indexsize;
  int ways;
  int cycle;
  ReplacePolicy rpolicy;
  WritePolicy wpolicy;
};

struct MemSysConfig {
  uint32_t memSize;
  int memCycle;
  int cacheLevel;
  bool cacheOn;
  std::vector<CacheSettings> cacheSettings;
};

struct Instruction {
  uint32_t add;
  uint32_t npc;
  uint32_t ins;
  int type;
  int opcode;
  int rd1;
  uint32_t A;
  uint64_t vA;
  float fA;
  int rd2;
  uint32_t B;
  uint64_t vB;
  float fB;
  int rd3;
  int32_t imm;
  int stage;
  uint32_t aluoutput;
  float fpuoutput;
  uint64_t vuoutput;
  uint32_t lmd;
  bool cond;
  int dst; //gpr:0-15 fpr:16-31 vr:32-47
};
#endif
