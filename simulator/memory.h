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
#include <string>

class Memory : public Memcache {
 public:
    Memory(int size, int cycle);
    ~Memory();
    int getSize() { return _size; }
    int load(int add, uint8_t *blk, int len);
    int store(int address, uint8_t *blk, int len);
    std::string dump();
 private:
    int _size;
    int _add; //current waiting add
    uint8_t* _data;  //byte-addressable
};

#endif /* defined(__MIPS_Simulator__Memory__) */
