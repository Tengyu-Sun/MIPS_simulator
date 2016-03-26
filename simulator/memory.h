//
//  memory.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//


#ifndef __MIPS_Simulator__Memory__
#define __MIPS_Simulator__Memory__

#include <string>
#include "storage.h"

class Memory : public Storage {
    Q_OBJECT
 public:
    uint32_t _size;
    uint8_t* _data;  //byte-addressable

    Memory(uint32_t size, int cycle_) {
       cycle = cycle_;
       countdown = cycle_;
       nextLevel = nullptr;
       _add = 0;
       _idle = true;
       _data = new uint8_t[size];
    }
    ~Memory() {
        delete[] _data;
    }
    int load(uint32_t add, uint8_t *blk, int len);
    int store(uint32_t add, uint8_t *blk, int len);
    std::string dump();


 private:
    bool _idle;
    uint32_t _add; //current waiting add

};


#endif /* defined(__MIPS_Simulator__Memory__) */
