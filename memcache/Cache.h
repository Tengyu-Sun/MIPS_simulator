//
//  Cache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Cache__
#define __MIPS_Simulator__Cache__

#include <stdio.h>
#include "Memcache.h"
typedef struct
{
public:
    int valid;
    int dirty;
    int tag;
    int* data;
    //Cacheline(int valid, int tag, int address, int* data);
}Cacheline;

class Cache: public Memcache
{
public:
    Cache(int cachesize, int blocksize, int ways, Memcache *nextLevel);
    int hit;
    int miss;
    Cacheline** cachelines;
    message load(int address);
    message store(int address, int value);
    void evict(int blockNumber);
    Cacheline* inCache(int address);
private:
    int cachesize;
    int blocksize;
    int ways;
};




#endif /* defined(__MIPS_Simulator__Cache__) */
