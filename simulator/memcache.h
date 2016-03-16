//
//  memcache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Memcache__
#define __MIPS_Simulator__Memcache__

#include <cstdint>

class Memcache {
  public:
    int cycle;
    int countdown;
    Memcache *nextLevel;
    virtual int load(int address, uint8_t *blk, int len) = 0;
    virtual int store(int address, uint8_t *blk, int len) = 0;
};

#endif /* defined(__MIPS_Simulator__Memcache__) */
