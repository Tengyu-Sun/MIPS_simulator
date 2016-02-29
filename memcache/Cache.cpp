//
//  Cache.cpp
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#include "Cache.h"

Cache::Cache(int cachesize, int blocksize, int ways, Memcache *nextLevel)
{
    this->cachesize = cachesize;
    this->blocksize = blocksize;
    this->ways = ways;
    this->nextLevel = nextLevel;
    cachelines = new Cacheline*[ways];
    int setSize = cachesize / ways;
    for(int i = 0; i < ways; i++) {
        for(int j = 0; j < setSize; j++) {
            //cachelines[i][j] = *new Cacheline(0,0,0,new int[blocksize]);
            cachelines[i][j] = *new Cacheline;
        }
    }
}