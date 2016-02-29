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
#include <random>
//#include <stdlib.h>
//#include <time.h>
#include <iostream>
using namespace std;

int genRandomNumber(int ways) {
    // srand(time(NULL));
    // int ran = rand()*ways;
    // return ran;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,ways-1);
    return distribution(generator);
}


Cache::Cache(int cachesize, int blocksize, int ways, Memcache *nextLevel_, int circle_) {
    circle = circle_;
    countdown = circle_;
    _cachesize = cachesize;
    _blocksize = blocksize;
    _ways = ways;// 1 for direct-mapped cache, 2 for 2-way associative cache, etc
    nextLevel = nextLevel_; // next level of cache or memory
    _numberOfBlocks = cachesize / ways;
    cachelines = new Cacheline*[_numberOfBlocks]; //2-dimensional array to include all cachelines, e.g. Cacheline[numberOfBlocks][ways]
    hit = 0;
    miss = 0;
    for(int i = 0; i < _numberOfBlocks; i++) {
      cachelines[i] = new Cacheline[ways];
    }
    for(int i = 0; i < _numberOfBlocks; i++) {
        for(int j = 0; j < ways; j++){
            cachelines[i][j].data = new int[blocksize];
        }
    }
}

Cache::~Cache() {
  for(int i = 0; i < _numberOfBlocks; i++) {
    delete[] cachelines[i];
  }
  delete[] cachelines;
}

message Cache:: store(int address, int value) {// deprecated
  message msg;
  return msg;
}

message Cache:: store(int address, int* block) {// use write-back policy

    message msg;
    Cacheline* candidate = inCache(address);
    if(candidate != NULL) { // if the address exists in the cache, write to it and label the dirty flag to 1
      if(countdown == 0) {
          candidate->dirty = 1;
          for(int i = 0; i < _blocksize; i++) {
              candidate->data[i] = block[i];
          }
          msg.ok = true;
          msg.data = candidate->data;
          countdown = circle;
          return msg;
      } else {
          countdown--;
          //msg.ok = false;
          return msg;
      }
    } else {  // if not, evict a cache line corresponding to this address and write to it.
        //int numberOfBlocks = cachesize / blocksize;
        int blockNumber = (address / _blocksize)% _numberOfBlocks;
        Cacheline* cacheline = evict(blockNumber);
        if(countdown == 0) {
            cacheline->dirty = 1;
            for(int i = 0; i < _blocksize; i++) {
                cacheline->data[i] = block[i];
            }
            msg.ok = true;
            msg.data = cacheline->data;
            countdown = circle ;
            return msg;
        } else {
            countdown--;
            //msg.ok = false;
            return msg;
        }
    }
}

//load from address
message Cache::load(int address) {
    message msg;
    Cacheline* candidate = inCache(address);
    if(candidate == nullptr) {// there is a miss
        miss++;
        if(nextLevel != nullptr) {
            msg = nextLevel->load(address);
            while(!msg.ok) {
                msg = nextLevel->load(address);
            }
            //copy the data in msg into current cache
            int* data = msg.data;
            // find if any slots is empty or evict one line in this blockNumber
            //int numberOfBlocks = cachesize / blocksize;
            int blockNumber = (address / _blocksize)% _numberOfBlocks;
            int tag = address / (_blocksize*_numberOfBlocks);
            Cacheline* cacheline = evict(blockNumber);
            // copy data into the current cacheline
            for(int i = 0; i < _blocksize; i++) {
                cacheline->data[i] = data[i];
            }
            cacheline->valid = 1;
            cacheline->tag = tag;
            msg.ok = true;
            msg.data = cacheline->data;
            return msg;
        }
    } else {// there is a hit
        hit++;
        if(countdown == 0) {
            countdown = circle;
            msg.ok = true;
            msg.data = candidate->data;
            return msg;
        } else {
            countdown--;
            //msg.ok = false;
            return msg;
        }
    }
    return msg;
}
// evict a cacheline from the current block (referenced by blockNumber) if all ways are occupied, return the cleared line
// if there is a line not occupied, return it
Cacheline* Cache:: evict(int blockNumber) {
    if(_ways == 1) {
        cachelines[blockNumber][0].valid = 0;
        cachelines[blockNumber][0].dirty = 0;
        return &cachelines[blockNumber][0];
    } else {
        for(int i = 0; i < _ways; i++) {
            // return the empty line;
            if(cachelines[blockNumber][i].valid == 0) return &cachelines[blockNumber][i];
        }
        // if all ways are written, evict it to lower level storage, return the cleared line;
        int evictedWay = genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
        if(cachelines[blockNumber][evictedWay].dirty == 1) {
            if(nextLevel != NULL) {
                // write back to lower level of storage if the dirty flag is set to 1.
                int* block = cachelines[blockNumber][evictedWay].data;
                (*nextLevel).store(blockNumber*_blocksize, block);
            }
        }
        cachelines[blockNumber][evictedWay].valid = 0;
        cachelines[blockNumber][evictedWay].dirty = 0;
        return &cachelines[blockNumber][evictedWay];
    }
}

Cacheline* Cache:: inCache(int address) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
  //  int numberOfBlocks = cachesize / blocksize;
  //  int offset = address % _blocksize;
    int blockNumber = (address / _blocksize)% _numberOfBlocks;
    int tag = address / (_blocksize*_numberOfBlocks);
    bool cacheHit = false;
    for(int i = 0; i < _ways; i++) {
      //  cout<< tag << " " << cachelines[blockNumber][i].valid << " " << cachelines[blockNumber][i].tag<< endl;
        
        if(cachelines[blockNumber][i].valid == 0) continue;
        if(cachelines[blockNumber][i].tag == tag) {
           cacheHit = true;
           return &cachelines[blockNumber][i];
        }
    }

    return nullptr;
}
