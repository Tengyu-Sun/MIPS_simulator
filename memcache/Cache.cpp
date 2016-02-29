//
//  Cache.cpp
//  MIPS_Simulator
//
//  Implement multi-way associative cache, can be extended to multiple levels
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 Wei Hong All rights reserved.
//

#include "Cache.h"
#include <stdlib.h>
#include <time.h>

int genRandomNumber(int ways) {
    srand(time(NULL));
    int ran = rand()*ways;
    return ran;
}


Cache::Cache(int cachesize, int blocksize, int ways, Memcache *nextLevel)
{
    this->countdown = 5;
    this->cachesize = cachesize;
    this->blocksize = blocksize;
    this->ways = ways;// 1 for direct-mapped cache, 2 for 2-way associative cache, etc
    this->nextLevel = nextLevel; // next level of cache or memory
    int numberOfBlocks = cachesize / ways;
    cachelines = new Cacheline*[numberOfBlocks]; //2-dimensional array to include all cachelines, e.g. Cacheline[numberOfBlocks][ways]
    for(int i = 0; i < numberOfBlocks; i++) {
        cachelines[i] = new Cacheline[ways];
    }
    
}

message Cache:: store(int address, int value) // use write-back policy
{
    message msg;
    return msg;
}

message Cache::load(int address)
{
    message msg;
    Cacheline* candidate = inCache(address);
    if(candidate == NULL) {// there is a miss
        miss++;
        // fetch block from the lower level
        //nextLevel->load(address);
    } else {
        
    }
    
    return msg;
}

void Cache:: evict(int blockNumber) {
    if(ways == 1) {
        cachelines[blockNumber][0].valid = 0;
        cachelines[blockNumber][0].dirty = 0;
    } else {
        int evictedWay = genRandomNumber(ways);// if all ways are occupied, we have to randommly evict one line of them.
        if(cachelines[blockNumber][evictedWay].dirty == 1) {
            if(nextLevel != NULL) {
                // write back to lower level of storage if the dirty flag is set to 1.
                // need to be implemented
            }
        }
        cachelines[blockNumber][evictedWay].valid = 0;
        cachelines[blockNumber][evictedWay].dirty = 0;
    }
}

Cacheline* Cache:: inCache(int address) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int numberOfBlocks = cachesize / blocksize;
    int offset = address % blocksize;
    int blockNumber = (address / blocksize)% numberOfBlocks;
    int tag = address / (blocksize*numberOfBlocks);
    bool cacheHit = false;
    for(int i = 0; i < ways; i++) {
        if(cachelines[blockNumber][i].valid == 0) continue;
        if(cachelines[blockNumber][i].tag == tag) {
            cacheHit = true;
            hit++;
            return &cachelines[blockNumber][i];
            break;
        }
    }
    miss++;
    return NULL;
}
