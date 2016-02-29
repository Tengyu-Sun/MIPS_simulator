//
//  Memory.cpp
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#include "Memory.h"
#include <iostream>
using namespace std;

Memory::Memory(int size, int blocksize, Memcache* nextLevel_, int circle_) {
    circle = circle_;
    countdown = circle_;
    _size = size;
    _blocksize = blocksize;
    _data = new int[size];
    nextLevel = nextLevel_;
}

Memory::~Memory() {
  delete[] _data;
}

//load a block from specific address
message Memory::load(int address) {
    int blockNumber = address / _blocksize;//?
    //int* data = _data;//?
    message msg;
    if(address > _size - 1 || address < 0) {
        //   cout << "The address requested is out of bound" << endl;
        // msg.data = nullptr;
        // msg.ok = false;
        return msg;
    } else {
        if(countdown == 0) {
            msg.data = &_data[blockNumber*_blocksize];//?
            msg.ok = true;
            countdown = circle;
            return msg;
        } else {
            countdown--;
            // msg.data = nullptr;
            // msg.ok = false;
            return msg;
        }
    }
}

// write a block of data into memory
message Memory::store(int address, int *block) {
    //int size = _size;
    //int* data = this->data;
    message msg;
    if(address > _size - 1 || address < 0) {
        // cout << "The address requested is out of bound" << endl;
        return msg;
    } else {
      if(countdown == 0) {
          int blockNumber = address / _blocksize;
          for(int i = 0; i < _blocksize; i++) {
            _data[blockNumber*_blocksize + i] = *block++;
          }
          msg.data = &_data[address];
          msg.ok = true;
          countdown = circle;
          return msg;
      } else {
          countdown--;
          //msg.ok = false;
          return msg;
      }
    }
    return msg;
}


// write integer to specific address
message Memory:: store(int address, int value) {
  // int size = this->size;
  // int* data = this->data;
  message msg;
  if(address > _size - 1 || address < 0) {
      // cout << "The address requested is out of bound" << endl;
      return msg;
  } else {
    if(countdown == 0) {
      _data[address] = value;
      msg.data = &_data[address];
      msg.ok = true;
      countdown = circle;
      return msg;
    } else {
      countdown--;
    //  msg.ok = false;
      return msg;
    }
  }
}


int* Memory::getData() {
  // if(this->data == nullptr) {
  //       return nullptr;
  // }
  return _data;
}

int Memory::getSize() {
    return _size;
}
