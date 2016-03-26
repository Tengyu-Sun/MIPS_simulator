#include "memsys.h"
#include <fstream>

MemSys::MemSys(Cache *cc, Memory *mm, bool co):_cache(cc), _mainMemory(mm),
_cacheOn(co) {
  _memSize = _mainMemory->_size;
}

int MemSys::loadWord(int add, uint32_t* val) {
  if(add > _memSize - 1 || add < 0 || add%4 != 0) {
    return -1;
  }
  uint8_t tmp[4];
  int flag = -1;
  if (_cacheOn) {
      flag = _cache->load(add, tmp, 4);
  } else {
      flag = _mainMemory->load(add, tmp, 4);
  }
  if(flag == 1) {
    *val = (tmp[0]<<24) | (tmp[1]<<16) | (tmp[2]<<8) | tmp[3];
  }
  return flag;
}

int MemSys::loadByte(int add, uint8_t* val) {
  if(add > _memSize - 1 || add < 0) {
    return -1;
  }
  if (_cacheOn) {
      return _cache->load(add, val, 1);
  } else {
      return _mainMemory->load(add, val, 1);
  }
}

int MemSys::storeWord(int add, uint32_t val) {
    if(add > _memSize - 1 || add < 0 || add%4 != 0) {
      return -1;
    }
    uint8_t tmp[4];
    tmp[3] = val%256;
    val = val/256;
    tmp[2] = val%256;
    val = val/256;
    tmp[1] = val%256;
    val = val/256;
    tmp[0] = val;

    if(_cacheOn) {
        return _cache->store(add, tmp, 4);
    } else {
        return _mainMemory->store(add, tmp, 4);
    }
}

int MemSys::storeByte(int add, uint8_t val) {
    if(add > _memSize - 1 || add < 0) {
      return -1;
    }
    int flag = 0;
    if(_cacheOn) {
        flag = _cache->store(add, &val, 1);
    } else {
        flag = _mainMemory->store(add, &val, 1);
    }
    return flag;
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
