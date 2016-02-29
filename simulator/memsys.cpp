#include "memsys.h"

MemSys::MemSys(Cache *cc, Memory *mm, bool co):_cache(cc), _mainMemory(mm),
_cacheOn(co) {
  _memSize = _mainMemory->getSize();
}

int MemSys::load(int add, int* val) {
    if(add > _memSize - 1 || add < 0) {
      return -1;
    }
    if (_cacheOn) {
        return 1;
    } else {
        return _mainMemory->load(add, val);
    }
}

int MemSys::store(int add, int val) {
    if(add > _memSize - 1 || add < 0) {
      return -1;
    }
    if(_cacheOn) {
        return 1;
    } else {
        return _mainMemory->store(add, val);
    }
}
