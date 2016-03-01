#include "cache.h"
#include "memory.h"
#include <string>

class MemSys {
 public:
  MemSys(Cache *cache, Memory *mainMemory, bool cacheOn);
  int load(int add, int *val);
  int store(int add, int val);
  void dump(std::string fn);
  void setCacheOn(bool flag);
 private:
  Cache *_cache;
  Memory *_mainMemory;
  int _memSize;
  bool _cacheOn;
};
