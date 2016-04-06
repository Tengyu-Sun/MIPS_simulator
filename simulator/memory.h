//
//  memory.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//


#ifndef __MIPS_Simulator__Memory__
#define __MIPS_Simulator__Memory__

#include "storage.h"

class Memory : public Storage {
 public:
    Memory(uint32_t size, int cycle_);
    ~Memory();
    int load(uint32_t add, uint8_t *blk, int len);
    int store(uint32_t add, uint8_t *blk, int len);
    std::string dump();
    uint32_t _size;
    uint8_t* _data;  //byte-addressable
};

#endif /* defined(__MIPS_Simulator__Memory__) */
