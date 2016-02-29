//
//  Memory.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Memory__
#define __MIPS_Simulator__Memory__

#include <stdio.h>
#include "Memcache.h"

class Memory: public Memcache
{
public:
    Memory(int size, int blocksize, Memcache *nextLevel);
    int* getData();
    int getSize();
    int countdown;
    Memcache *nextLevel;
    message load(int address);
    message store(int address, int* block);
    message store(int address, int value);
    
  private:
    int size;
    int blocksize;
    int* data;
    
};
#endif /* defined(__MIPS_Simulator__Memory__) */
