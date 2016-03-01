#include <random>
#include "cache.h"
#include <iostream>

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

Cache::~Cache() {
  delete[] _cachelines;
}

int Cache::load(int add, int *block, int len) {
  if (add % _linesize != 0 || len != _linesize) {
    return -1;
  }
  countdown--;
  if(countdown == 0) {
    Cacheline* candidate = inCache(add);
    if(candidate == nullptr) {// there is a miss
      ++miss;
      if(nextLevel != nullptr) {
        ++countdown;
        int* blk = new int[_linesize];
        int flag = nextLevel->load(add, blk, _linesize);
        while(flag == 0) {
          ++countdown;//
          flag = nextLevel->load(add, blk, _linesize);
        }
        if(flag == -1) {
          countdown = circle;
          delete[] blk;
          return -1;
        }
        //int numberOfBlocks = cachesize / blocksize;
        //int blockNumber = (address / _blocksize)% _numberOfBlocks;
        Cacheline* cacheline = evict(add);
        // copy data into the current cacheline
        for(int i = 0; i < _linesize; i++) {
            cacheline->data[i] = blk[i];
        }
        cacheline->valid = true;
        cacheline->tag = (add/_linesize)/_cachesize;
        delete[] blk;
        return 0;//increase one hit
      } else {
        countdown = circle;
        return -1;
      }
    } else {// there is a hit
      ++hit;
      for(int i=0; i<len; ++i) {
        block[i] = candidate->data[i];
      }
      countdown = circle;
      return 1;
    }
  } else {
    return 0;
  }
}

int Cache::store(int add, int *blk, int len) {
  if (add % _linesize != 0 || len != _linesize) {
    return -1;
  }
  countdown--;
  if(countdown == 0) {
    Cacheline* candidate = inCache(add);
    if(candidate != nullptr) {
      candidate->dirty = true;
      for(int i = 0; i < len; ++i) {
          candidate->data[i] = blk[i];
      }
    } else {
      Cacheline* cacheline = evict(add);
      for(int i = 0; i < len; ++i) {
          cacheline->data[i] = blk[i];
      }
      cacheline->valid = true;
      cacheline->dirty = true;
      cacheline->tag = (add/_linesize)/_cachesize;
    }
    countdown = circle;
    return 1;
  } else {
    return 0;
  }
}

int Cache::load(int add, int *val) {
  int alignedadd = (add/_linesize)*_linesize;
  int *blk = new int[_linesize];
  int flag = load(alignedadd, blk, _linesize);
  if (flag == 1) {
    *val = blk[add%_linesize];
  }
  delete[] blk;
  return flag;
}

int Cache::store(int add, int val) {
  int alignedadd = (add/_linesize)*_linesize;
  countdown--;
  if(countdown == 0) {
    Cacheline* candidate = inCache(alignedadd);
    if(candidate != nullptr) {
      candidate->dirty = true;
      candidate->data[add%_linesize] = val;
    } else {
      int *blk = new int[_linesize];

      while(load(alignedadd, blk, _linesize) == 0){
        std::cout<<alignedadd<<"\n";
      }
      Cacheline* cacheline = inCache(alignedadd);
      cacheline->data[add%_linesize] = val;
      cacheline->dirty = true;
      cacheline->tag = (add/_linesize)/_cachesize;
    }
    countdown = circle;
    return 1;
  } else {
    return 0;
  }
}

Cacheline* Cache::inCache(int add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
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
Cacheline* Cache::evict(int add) {
  int idx = ((add/_linesize)%_cachesize)*_ways;
  for(int i = 0; i < _ways; i++) {
      // return the empty line;
      if(_cachelines[idx+i].valid == false) return &_cachelines[idx+i];
  }
  // if all ways are written, evict it to lower level storage, return the cleared line;
  int evictedWay = genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
  if(_cachelines[idx+evictedWay].dirty == true) {
      if(nextLevel != nullptr) {
        // write back to lower level of storage if the dirty flag is set to 1.
        int *block = _cachelines[idx+evictedWay].data;
        while(nextLevel->store(add, block, _linesize) == 0);
      }
  }
  _cachelines[idx+evictedWay].valid = false;
  _cachelines[idx+evictedWay].dirty = false;
  return &_cachelines[idx+evictedWay];
}

std::string Cache::dump(){
  std::string res;
  int nc = _cachesize*_ways;
  for(int i=0; i<nc; ++i) {
    res += std::to_string((int)_cachelines[i].valid) + " " + std::to_string((int)_cachelines[i].dirty) + " " + std::to_string(_cachelines[i].tag) + " ";
    for (int j=0; j<_linesize; ++j) {
      res += std::to_string(_cachelines[i].data[j]) + " ";
    }
    res += "\n";
  }
  return res;
}
