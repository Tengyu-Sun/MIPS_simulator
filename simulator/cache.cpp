#include <random>
#include "cache.h"
#include <iostream>

int genRandomNumber(int ways) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, ways-1);
    return distribution(generator);
}

Cache::Cache(int indexsize, int linesize, int ways, int cycle_, Memcache* nextLevel_) {
  _indexsize = indexsize;
  _linesize = linesize;
  _ways = ways;
  _cachesize = indexsize*ways;
  cycle = cycle_;
  countdown = cycle_;
  nextLevel = nextLevel_;
  hit = 0;
  miss = 0;

  _cachelines = new Cacheline[_cachesize];
  for (int i = 0; i < _cachesize; ++i) {
    _cachelines[i].data = new uint8_t[_linesize];
  }
}

Cache::~Cache() {
  delete[] _cachelines;
}

int Cache::load(int add, uint8_t *block, int len) {
  if (add % _linesize != 0 || len != _linesize) {
    return -1;
  }
  if(countdown>0) {
    countdown--;
  }
  if(countdown == 0) {
    Cacheline* candidate = inCache(add);
    if(candidate == nullptr) {// there is a miss
      ++miss;
      if(nextLevel != nullptr) {
        ++countdown;
        uint8_t* blk = new uint8_t[_linesize];
        int flag = nextLevel->load(add, blk, _linesize);
        while(flag == 0) {
          ++countdown;//
          flag = nextLevel->load(add, blk, _linesize);
        }
        if(flag == -1) {
          countdown = cycle;
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
        cacheline->tag = (add/_linesize)/_indexsize;
        delete[] blk;
        misshit = true;
        return 0;//increase one hit
      } else {
        countdown = cycle;
        return -1;
      }
    } else {// there is a hit
      ++hit;
      if (misshit){
        --hit;
        misshit = false;
      }
      for(int i=0; i<len; ++i) {
        block[i] = candidate->data[i];
      }
      countdown = cycle;
      return 1;
    }
  } else {
    return 0;
  }
}

int Cache::store(int add, uint8_t *blk, int len) {
  if (add % _linesize != 0 || len != _linesize) {
    return -1;
  }
  if(countdown>0) {
    countdown--;
  }
  if(countdown == 0) {
    Cacheline* candidate = inCache(add);
    if(candidate != nullptr) {
      ++hit;
      candidate->dirty = true;
      for(int i = 0; i < len; ++i) {
          candidate->data[i] = blk[i];
      }
    } else {
      ++miss;
      Cacheline* cacheline = evict(add);
      for(int i = 0; i < len; ++i) {
          cacheline->data[i] = blk[i];
      }
      cacheline->valid = true;
      cacheline->dirty = true;
      cacheline->tag = (add/_linesize)/_indexsize;
    }
    countdown = cycle;
    return 1;
  } else {
    return 0;
  }
}

int Cache::load(int add, uint8_t *val) {
  int alignedadd = (add/_linesize)*_linesize;
  uint8_t *blk = new uint8_t[_linesize];
  int flag = load(alignedadd, blk, _linesize);
  if (flag == 1) {
    *val = blk[add%_linesize];
  }
  delete[] blk;
  return flag;
}

int Cache::store(int add, uint8_t val) {
  if(countdown>0) {
    countdown--;
  }
  if(countdown == 0) {
    int alignedadd = (add/_linesize)*_linesize;
    Cacheline* candidate = inCache(alignedadd);
    if(candidate != nullptr) {
      ++hit;
      if(misshit){
          --hit;
          misshit = false;
      }
      candidate->dirty = true;
      candidate->data[add%_linesize] = val;
    } else {
      uint8_t *blk = new uint8_t[_linesize];
      int tmp = 0;
      while(load(alignedadd, blk, _linesize) == 0){
        ++tmp;
      };
      countdown += tmp;
      misshit = true;
      return 0;
    }
    countdown = cycle;
    return 1;
  } else {
    return 0;
  }
}

Cacheline* Cache::inCache(int add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int idx = ((add/_linesize)%_indexsize)*_ways;
    int tag = (add/_linesize)/_indexsize;
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
  int idx = ((add/_linesize)%_indexsize)*_ways;
  for(int i = 0; i < _ways; i++) {
      // return the empty line;
      if(_cachelines[idx+i].valid == false) {
        return &_cachelines[idx+i];
      }
  }
  // if all ways are written, evict it to lower level storage, return the cleared line;
  //TODO:getLRU
  int evictedWay = genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
  if(_cachelines[idx+evictedWay].dirty == true) {
      if(nextLevel != nullptr) {
        // write back to lower level of storage if the dirty flag is set to 1.
        uint8_t *block = _cachelines[idx+evictedWay].data;
        while(nextLevel->store(add, block, _linesize) == 0);
      }
  }
  _cachelines[idx+evictedWay].valid = false;
  _cachelines[idx+evictedWay].dirty = false;
  return &_cachelines[idx+evictedWay];
}

std::string Cache::dump(){
  std::string res;
  for(int i=0; i<_cachesize; ++i) {
    res += std::to_string((int)_cachelines[i].valid) + " " + std::to_string((int)_cachelines[i].dirty) + " " + std::to_string(_cachelines[i].tag) + " ";
    for (int j=0; j<_linesize; ++j) {
      res += std::to_string(_cachelines[i].data[j]) + " ";
    }
    res += "\n";
  }
  return res;
}
