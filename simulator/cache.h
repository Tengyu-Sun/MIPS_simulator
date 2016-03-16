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
    uint8_t *data;
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
    Cache(int indexsize, int linesize, int ways, int cycle_, Memcache* nextLevel_);
    ~Cache();
    int hit;
    int miss;
    bool misshit;
    int load(int add, uint8_t *blk, int len);
    int store(int address, uint8_t* blk, int len);
    int load(int address, uint8_t *val);
    int store(int address, uint8_t val);
    std::string dump();
    Cacheline* evict(int add);
    Cacheline* inCache(int address);
 private:
    int _cachesize;
    int _indexsize;
    int _linesize;
    int _ways;
    Cacheline* _cachelines;
};




#endif /* defined(__MIPS_Simulator__Cache__) */
