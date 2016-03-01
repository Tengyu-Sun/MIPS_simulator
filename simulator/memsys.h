#include "cache.h"
#include "memory.h"
#include <string>

class MemSys {
 public:
  MemSys(Cache *cache, Memory *mainMemory, bool cacheOn);
  int load(int add, int *val);
  int store(int add, int val);
  void dump(std::string fn);
  bool _cacheOn;
  int getHit() { return _cache->hit; };
  void resetHit() { _cache->hit = 0; }
  int getMiss() { return _cache->miss; };
  void resetMiss() { _cache->miss = 0; }
 private:
  Cache *_cache;
  Memory *_mainMemory;
  int _memSize;
};
