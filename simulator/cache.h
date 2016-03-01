//
//  cache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Cache__
#define __MIPS_Simulator__Cache__

#include "memcache.h"
#include <string>

struct Cacheline {
    bool valid;
    bool dirty;
    int tag;
    int *data;
    Cacheline() {
      valid = false;
      dirty = false;
      tag = 0;
      data = nullptr;
    }
    ~Cacheline() {
      delete[] data;
    }
};

class Cache : public Memcache {
 public:
    Cache(int cachesize, int linesize, int ways, int circle, Memcache* nextLevel_);
    ~Cache();
    int hit;
    int miss;
    bool misshit;
    int load(int add, int *blk, int len);
    int store(int address, int* blk, int len);
    int load(int address, int *val);
    int store(int address, int value);
    std::string dump();
    Cacheline* evict(int add);
    Cacheline* inCache(int address);
 private:
    int _cachesize;
    int _linesize;
    int _ways;
    Cacheline* _cachelines;
};




#endif /* defined(__MIPS_Simulator__Cache__) */
