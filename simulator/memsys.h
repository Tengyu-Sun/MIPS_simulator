#include "cache.h"
#include "memory.h"

class MemSys {
 public:
  MemSys(Cache *cache, Memory *mainMemory, bool cacheOn);
  int load(int add, int *val);
  int store(int add, int val);
  void setCacheOn(bool flag);
 private:
  Cache *_cache;
  Memory *_mainMemory;
  int _memSize;
  bool _cacheOn;
};
