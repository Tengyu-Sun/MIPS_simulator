//
//  cache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Cache__
#define __MIPS_Simulator__Cache__

#include "storage.h"

class Cache : public Storage {
    Q_OBJECT
 public:
    Cache(int indexsize, int linesize, int ways, int cycle_, ReplacePolicy rpolicy_, WritePolicy wpolicy_, Storage* nextLevel_);
    ~Cache();
    int load(uint32_t add, uint8_t *blk, int len);
    int store(uint32_t add, uint8_t* blk, int len);
    void reset();
    std::string dump();
    Position evict(uint32_t add, int &t);
    Position inCache(uint32_t address);
    int hit;
    int miss;
    int _linesize;
    int _indexsize;
    int _ways;
    ReplacePolicy rpolicy;
    WritePolicy wpolicy;
    Cacheline** _cachelines;

    int getLRUNumber(int idx);
    int genRandomNumber(int ways);
    void visitLRU(uint32_t add);
signals:
    void updateLine(Cacheline** data, int idx, int way);
    void updateHit(int hit);
    void updateMiss(int miss);
};

#endif /* defined(__MIPS_Simulator__Cache__) */
