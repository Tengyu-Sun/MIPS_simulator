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

message Cache:: store(int address, int value) // deprecated
{
    message msg;
    return msg;
}

message Cache:: store(int address, int* block) // use write-back policy
{
    message msg;
    
    Cacheline* candidate = inCache(address);
    if(candidate != NULL) { // if the address exists in the cache, write to it and label the dirty flag to 1
        if(countdown == 0) {
            candidate->dirty = 1;
            for(int i = 0; i < blocksize; i++) {
                candidate->data[i] = block[i];
            }
            msg.ok = true;
            msg.data = candidate->data;
            countdown = 5;
            return msg;
        } else {
            countdown--;
            msg.ok = false;
            return msg;
        }
    } else {  // if not, evict a cache line corresponding to this address and write to it.
        int numberOfBlocks = cachesize / blocksize;
        int blockNumber = (address / blocksize)% numberOfBlocks;
        Cacheline* cacheline = evict(blockNumber);
        if(countdown == 0) {
            cacheline->dirty = 1;
            for(int i = 0; i < blocksize; i++) {
                cacheline->data[i] = block[i];
            }
            msg.ok = true;
            msg.data = cacheline->data;
            countdown = 5;
            return msg;
        } else {
            countdown--;
            msg.ok = false;
            return msg;
        }
    }
}

//load from address
message Cache::load(int address)
{
    message msg;
    Cacheline* candidate = inCache(address);
    if(candidate == NULL) {// there is a miss
        miss++;
        if(nextLevel != NULL) {
            msg = nextLevel->load(address);
            while(!msg.ok) {
                msg = nextLevel->load(address);
            }
            //copy the data in msg into current cache
            int* data = msg.data;
            // find if any slots is empty or evict one line in this blockNumber
            int numberOfBlocks = cachesize / blocksize;
            int blockNumber = (address / blocksize)% numberOfBlocks;
            Cacheline* cacheline = evict(blockNumber);
            // copy data into the current cacheline
            for(int i = 0; i < blocksize; i++) {
                cacheline->data[i] = data[i];
            }
            msg.ok = true;
            msg.data = cacheline->data;
            return msg;
        }
    } else {// there is a hit
        if(countdown == 0) {
            countdown = 5;
            msg.ok = true;
            msg.data = candidate->data;
            return msg;
        } else {
            countdown--;
            msg.ok = false;
            return msg;
        }
    }
    return msg;
}
// evict a cacheline from the current block (referenced by blockNumber) if all ways are occupied, return the cleared line
// if there is a line not occupied, return it
Cacheline* Cache:: evict(int blockNumber) {
    if(ways == 1) {
        cachelines[blockNumber][0].valid = 0;
        cachelines[blockNumber][0].dirty = 0;
        return &cachelines[blockNumber][0];
    } else {
        for(int i = 0; i < ways; i++) {
            // return the empty line;
            if(cachelines[blockNumber][i].valid == 0) return &cachelines[blockNumber][i];
        }
        // if all ways are written, evict it to lower level storage, return the cleared line;
        int evictedWay = genRandomNumber(ways);// if all ways are occupied, we have to randommly evict one line of them.
        if(cachelines[blockNumber][evictedWay].dirty == 1) {
            if(nextLevel != NULL) {
                // write back to lower level of storage if the dirty flag is set to 1.
                int* block = cachelines[blockNumber][evictedWay].data;
                (*nextLevel).store(blockNumber*blocksize,block);
            }
        }
        cachelines[blockNumber][evictedWay].valid = 0;
        cachelines[blockNumber][evictedWay].dirty = 0;
        return &cachelines[blockNumber][evictedWay];
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
        }
    }
    
    return NULL;
}
