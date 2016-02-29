//
//  Cache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Cache__
#define __MIPS_Simulator__Cache__

//#include <stdio.h>
#include "Memcache.h"
struct Cacheline {
//public:
    int valid;
    int dirty;
    int tag;
    int* data;
    Cacheline() {
      data = nullptr;
    }
    ~Cacheline() {
      if (!data) {
        delete[] data;
      }
    }
};

class Cache: public Memcache {
 public:
    Cache(int cachesize, int blocksize, int ways, Memcache *nextLevel, int circle);
    ~Cache();
    int hit;
    int miss;
    Cacheline** cachelines;
    message load(int address);
    message store(int address, int value);
    message store(int address, int* block);
    Cacheline* evict(int blockNumber);
    Cacheline* inCache(int address);
 private:
    int _cachesize;
    int _blocksize;
    int _numberOfBlocks;
    int _ways;
};




#endif /* defined(__MIPS_Simulator__Cache__) */
