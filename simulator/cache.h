//
//  cache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Cache__
#define __MIPS_Simulator__Cache__

#include <string>
#include "storage.h"


struct Cacheline {
    bool valid;
    bool dirty;
    int tag;
    int lru;
    uint8_t *data;
    Cacheline() {
      valid = false;
      dirty = false;
      tag = 0;
      lru = 0;
      data = nullptr;
    }
    ~Cacheline() {
      delete[] data;
    }
};

class Cache : public Storage {
    Q_OBJECT
 public:
    Cache(int indexsize, int linesize, int ways, int cycle_, Storage* nextLevel_);
    ~Cache();
    int load(uint32_t add, uint8_t *blk, int len);
    int store(uint32_t add, uint8_t* blk, int len);
    void reset();
    std::string dump();
    Cacheline* evict(int add);
    Cacheline* inCache(int address);
    int hit;
    int miss;
    Cacheline* _cachelines;
signals:
    void update(Cacheline* data);
 private:
    int _cachesize;
    int _indexsize;
    int _linesize;
    int _ways;
    uint8_t *buf;
    int preadd;
    int prelen;
    bool missReady;
    int policy;
    int *lru;
    int getLRUNumber(int idx);
    void visitLRU(int add);
};

#endif /* defined(__MIPS_Simulator__Cache__) */
