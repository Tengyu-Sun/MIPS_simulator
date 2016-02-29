//
//  memory.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//


#ifndef __MIPS_Simulator__Memory__
#define __MIPS_Simulator__Memory__

#include "memcache.h"

class Memory : public Memcache {
 public:
    Memory(int size, int circle);
    ~Memory();
    // int* getData();
    int getSize() { return _size; }
    int load(int add, int *blk, int len);
    int store(int address, int *block, int len);
    int load(int address, int *val);
    int store(int address, int value);
 private:
    int _size;
    int* _data;//byte-addressable?
};

#endif /* defined(__MIPS_Simulator__Memory__) */
