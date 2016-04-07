#include "memory.h"
#include <cstdlib>
//#include <iostream>

Memory::Memory(uint32_t size, int cycle_) {
  cycle = cycle_;
  nextLevel = nullptr;
  _size = size;
  _data = (uint8_t*) calloc(_size, sizeof(uint8_t));  //new uint8_t[size];
}

Memory::~Memory() {
  free((void*)_data);
}

int Memory::load(uint32_t add, uint8_t *blk, int len) {
  if(add + len > _size) {
      return -1;
  } else {
    for (int i = 0; i < len; ++i){
      blk[i] = _data[add+i];
    }
    return cycle;
  }
}

int Memory::store(uint32_t add, uint8_t *blk, int len) {
    if(add + len > _size) {
        return -1;
    } else {
      for (int i = 0; i < len; ++i){
        _data[add+i] = blk[i];
      }
      emit notify(_data, add, len);
      return cycle;
    }
}

std::string Memory::dump() {
  std::string res = "address: value\n";
  for(unsigned int i = 0; i < _size; ++i) {
    res += std::to_string(i) + ": " + std::to_string(_data[i]) + "\n";
  }
  return res;
}
