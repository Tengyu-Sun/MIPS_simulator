#include "memory.h"

Memory::Memory(int size, int circle_) {
  _size = size;
  circle = circle_;
  countdown = circle_;
  nextLevel = nullptr;
  _data = new int[size];
}

Memory::~Memory() {
  delete[] _data;
}

int Memory::load(int add, int *blk, int len) {
  if(add > _size - 1 || add < 0) {
      return -1; //throw exceptions?
  } else {
    //TODO: check if it is the same add as last call
    countdown--;
    if(countdown == 0) {
      for (int i=0; i<len; ++i){
        blk[i] = _data[add+i];
      }
      countdown = circle;
      return 1;
    } else {
      return 0;
    }
  }
}

int Memory::store(int add, int *blk, int len) {
    if(add > _size - 1 || add < 0) {
        return -1;//throw?
    } else {
      //TODO: check if it is the same add as last call
      countdown--;
      if(countdown == 0) {
        for (int i=0; i<len; ++i){
          _data[add+i] = blk[i];
        }
        countdown = circle;
        return 1;
      } else {
        return 0;
      }
    }
}

int Memory::load(int add, int *val) {
  return load(add, val, 1);
}

int Memory::store(int add, int val) {
  return store(add, &val, 1);
}
