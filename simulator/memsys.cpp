#include "memsys.h"
#include <fstream>

MemSys::MemSys(Cache *cc, Memory *mm, bool co):_cache(cc), _mainMemory(mm),
_cacheOn(co) {
  _memSize = _mainMemory->getSize();
}

int MemSys::load(int add, int* val) {
    if(add > _memSize - 1 || add < 0) {
      return -1;
    }
    if (_cacheOn) {
        return _cache->load(add, val);
    } else {
        return _mainMemory->load(add, val);
    }
}

int MemSys::store(int add, int val) {
    if(add > _memSize - 1 || add < 0) {
      return -1;
    }
    if(_cacheOn) {
        return _cache->store(add, val);
    } else {
        return _mainMemory->store(add, val);
    }
}

void MemSys::dump(std::string filename) {
  std::fstream output(filename, std::fstream::out);
  if(_cacheOn) {
    output<<"cache:\n";
    Cache *cur = _cache;
    while(cur != nullptr){
      output<<cur->dump();
      output<<"\n";
      cur = (Cache*) cur->nextLevel;
    }
  }
  output<<"memory:\n";
  output<<_mainMemory->dump();
}
