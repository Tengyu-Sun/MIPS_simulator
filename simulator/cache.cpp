#include <random>
#include "cache.h"

int genRandomNumber(int ways) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, ways-1);
    return distribution(generator);
}

Cache::Cache(int cachesize, int linesize, int ways, int circle_, Memcache* nextLevel_) {
  _cachesize = cachesize;
  _linesize = linesize;
  _ways = ways;
  circle = circle_;
  countdown = circle_;
  nextLevel = nextLevel_;
  hit = 0;
  miss = 0;

  int nc = _cachesize*_ways;
  _cachelines = new Cacheline[nc];
  for (int i = 0; i < nc; ++i) {
    _cachelines[i].data = new int[_linesize];
  }
}

int Cache::load(int add, int *blk, int len) {
  if (add % _linesize != 0 || len != _linesize) {
    return -1;
  }

  return 1;
}

int Cache::store(int add, int *blk, int len) {
  return 1;
}

int Cache::load(int add, int *val) {
  countdown--;
  if(countdown == 0) {
    Cacheline* candidate = inCache(add);
    if(candidate == nullptr) {// there is a miss
      ++miss;
      if(nextLevel != nullptr) {
        int alignedadd = (add/_linesize)*_linesize;
        int *blk = new int[_linesize];
        do {
          ++countdown;
        } while (nextLevel->load(alignedadd, blk, _linesize) == 0);
        //copy the data in msg into current cache

        // find if any slots is empty or evict one line in this blockNumber
        //int numberOfBlocks = cachesize / blocksize;
        //int blockNumber = (address / _blocksize)% _numberOfBlocks;
        Cacheline* cacheline = evict(add);
        // copy data into the current cacheline
        for(int i = 0; i < _blocksize; i++) {
            cacheline->data[i] = data[i];
        }

        msg.ok = true;
        msg.data = cacheline->data;
        return msg;
      } else {
        countdown = circle;
        return -1;
      }
    } else {// there is a hit
      ++hit;
      *val = candidate.data[add%_linesize];
      countdown = circle;
      return 1;
    }
  } else {
    return 0;
  }
}

int Cache::store(int add, int val) {
    return 1;
}

Cacheline* Cache:: inCache(int add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int idx = ((add/_linesize)%_cachesize)*_ways;
    int tag = (add/_linesize)/_cachesize;
    //bool cacheHit = false;
    for(int i = 0; i < _ways; i++) {
        if(_cachelines[idx+i].valid == false){
          continue;
        }
        if(_cachelines[idx+i].tag == tag) {
            //cacheHit = true;
            return &_cachelines[idx+i];
        }
    }
    return nullptr;
}

// evict a cacheline from the current block (referenced by blockNumber) if all ways are occupied, return the cleared line
// if there is a line not occupied, return it
Cacheline* Cache:: evict(int add) {
  int idx = (add/_linesize)%_cachesize;
  if(_ways == 1) {
    _cachelines[idx].valid = false;
    _cachelines[idx].dirty = false;
    return &_cachelines[idx];
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
