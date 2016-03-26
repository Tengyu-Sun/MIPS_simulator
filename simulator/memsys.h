#ifndef __MIPS_Simulator__Memsys__
#define __MIPS_Simulator__Memsys__

#include "cache.h"
#include "memory.h"
#include <string>

class MemSys {
 public:
  MemSys(Cache *cache, Memory *mainMemory, bool cacheOn);
  int loadWord(int add, uint32_t *val);
  int storeWord(int add, uint32_t val);
  int loadByte(int add, uint8_t *val);
  int storeByte(int add, uint8_t val);
  void dump(std::string fn);
  bool _cacheOn;
  int getHit() { return _cache->hit; }
  void resetCache() { _cache->reset(); }
  int getMiss() { return _cache->miss; }
 private:
  Cache *_cache;
  Memory *_mainMemory;
  uint32_t _memSize;
};

#endif /* defined(__MIPS_Simulator__Memsys__) */
