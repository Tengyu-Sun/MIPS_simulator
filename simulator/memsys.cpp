#include "memsys.h"
#include <fstream>
#include <iostream>

MemSys::MemSys(Cache *cc, Memory *mm, bool co):_cache(cc), _mainMemory(mm),
_cacheOn(co) {
  _memSize = _mainMemory->_size;
  _cacheSize = _cache->_cachesize;
  _lineSize = _cache->_linesize;
}

int MemSys::loadWord(uint32_t add, uint32_t* val) {
  if(add+3 > _memSize - 1 || add%4 != 0) {
    std::cout<<add<<" "<<_memSize<<std::endl;
    return -1;
  }
  uint8_t tmp[4];
  int flag = -1;
  if (_cacheOn) {
      flag = _cache->load(add, tmp, 4);
  } else {

      flag = _mainMemory->load(add, tmp, 4);
  }
  if(flag == 4) {
    *val = (tmp[0]<<24) | (tmp[1]<<16) | (tmp[2]<<8) | tmp[3];
  }
  return flag;
}

int MemSys::loadByte(uint32_t add, uint8_t* val) {
  if(add > _memSize - 1) {
    return -1;
  }
  if (_cacheOn) {
      return _cache->load(add, val, 1);
  } else {
      return _mainMemory->load(add, val, 1);
  }
}

int MemSys::storeWord(uint32_t add, uint32_t val) {
    if(add + 3 > _memSize - 1 || add%4 != 0) {
      return -1;
    }
    uint8_t tmp[4];
    tmp[3] = val & 0xff;
    val >>= 8;
    tmp[2] = val & 0xff;
    val >>= 8;
    tmp[1] = val & 0xff;
    val >>=8;
    tmp[0] = val;

    if(_cacheOn) {
        return _cache->store(add, tmp, 4);
    } else {
        return _mainMemory->store(add, tmp, 4);
    }
}

int MemSys::storeByte(uint32_t add, uint8_t val) {
    if(add > _memSize - 1) {
      return -1;
    }
    if(_cacheOn) {
        return _cache->store(add, &val, 1);
    } else {
        return _mainMemory->store(add, &val, 1);
    }
}

int MemSys::directStoreByte(uint32_t add, uint8_t val) {
    if(add > _memSize - 1) {
      return -1;
    }
    return _mainMemory->store(add, &val, 1);
}

void MemSys::dump(std::string filename) {
  std::fstream output(filename, std::fstream::out);
  if(_cacheOn) {
    output<<"cache:\n";
    output<<"valid dirty lru tag data...\n";
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
