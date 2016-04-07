#ifndef __MIPS_Simulator__Commons__
#define __MIPS_Simulator__Commons__

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


#endif
