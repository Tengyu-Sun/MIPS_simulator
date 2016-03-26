#include "memory.h"

Memory::Memory(uint32_t size, int cycle_) {
 cycle = cycle_;
 countdown = cycle_;
 nextLevel = nullptr;
 _size = size;
 _add = 0;
 _idle = true;
 _data = new uint8_t[size];
}

Memory::~Memory() {
 delete[] _data;
}

int Memory::load(uint32_t add, uint8_t *blk, int len) {
  if(add + len > _size) {
      return -1;
  } else {
    if(_idle) {
      _add = add;
      _idle = false;
    } else if(_add != add) {
      return -1;  //previous request not finished
    }
    if (countdown > 0) {
      countdown--;
    }
    if(countdown == 0) {
      for (int i = 0; i < len; ++i){
        blk[i] = _data[add+i];
      }
      countdown = cycle;
      _idle = true;
      return len;
    } else {
      return 0;
    }
  }
}

int Memory::store(uint32_t add, uint8_t *blk, int len) {
    if(add + len > _size) {
        return -1;
    } else {
      if(_idle) {
        _add = add;
        _idle = false;
      } else if(_add != add) {
        return -1;  //previous request not finished
      }
      if (countdown > 0) {
        countdown--;
      }
      if(countdown == 0) {
        for (int i = 0; i < len; ++i){
          _data[add+i] = blk[i];
        }
        countdown = cycle;
        _idle = true;
        emit update(_data);
        return len;
      } else {
        return 0;
      }
    }
}

std::string Memory::dump() {
  std::string res;
  for(unsigned int i = 0; i < _size; ++i) {
    res += std::to_string(_data[i]) + "\n";
  }
  return res;
}
