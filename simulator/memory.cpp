#include "memory.h"

Memory::Memory(int size, int cycle_) {
  _size = size;
  cycle = cycle_;
  countdown = cycle_;
  nextLevel = nullptr;
  _add = -1;
  _data = new uint8_t[size];
  if (size > 6) {
    _data[0] = 128;
    _data[1] = 2;
    _data[2] = 0;
    _data[3] = 5;
    _data[4] = 0;
    _data[5] = 20;
  }
}

Memory::~Memory() {
  delete[] _data;
}

int Memory::load(int add, uint8_t *blk, int len) {
  if(add > _size - 1 || add < 0) {
      return -1;
  } else {
    if(_add == -1) {
      _add = add;
    } else if(_add != add) {
      return -1;  //
    }
    countdown--;
    if(countdown == 0) {
      for (int i=0; i<len; ++i){
        blk[i] = _data[add+i];
      }
      countdown = cycle;
      _add = -1;
      return 1;
    } else {
      return 0;
    }
  }
}

int Memory::store(int add, uint8_t *blk, int len) {
    if(add > _size - 1 || add < 0) {
        return -1;//throw?
    } else {
      //TODO: check if it is the same add as last call
      countdown--;
      if(countdown == 0) {
        for (int i=0; i<len; ++i){
          _data[add+i] = blk[i];
        }
        countdown = cycle;
        return 1;
      } else {
        return 0;
      }
    }
}

std::string Memory::dump() {
  std::string res;
  for(int i=0; i<_size; ++i) {
    res += std::to_string(_data[i]) + "\n";
  }
  return res;
}
