#include <random>
#include "cache.h"
#include <iostream>

int genRandomNumber(int ways) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, ways-1);
    return distribution(generator);
}

Cache::Cache(int indexsize, int linesize, int ways, int cycle_, Storage* nextLevel_) {
  _indexsize = indexsize;
  _linesize = linesize;
  _ways = ways;
  _cachesize = indexsize*ways;
  cycle = cycle_;
  countdown = cycle_;
  nextLevel = nextLevel_;
  hit = 0;
  miss = 0;
  missReady = false;
  preadd = -1;
  prelen = -1;
  buf = nullptr;
  policy = 0;
  _cachelines = new Cacheline[_cachesize];
  lru = new int[_cachesize];
  for (int i = 0; i < _cachesize; ++i) {
    _cachelines[i].data = new uint8_t[_linesize];
    lru[i] = 0;
  }
}

Cache::~Cache() {
  delete[] _cachelines;
}

int Cache::load(uint32_t add, uint8_t *blk, int len) {
  if (preadd == -1 && prelen == -1) {
    preadd = add;
    prelen = len;
  } else if (preadd != add || prelen != len) {
    std::cout<<"error add access! current: "<<add
    <<" "<<len<<" pending: "<<preadd<<" "<<prelen<<std::endl;
    return -1;
  }
  if(countdown > 0) {
    countdown--;
  }
  if(countdown == 0) {
    if (missReady) {
      for (int i=0; i<len; ++i) {
        blk[i] = buf[i];
      }
      countdown = cycle;
      preadd = -1;
      prelen = -1;
      missReady = false;
      delete[] buf;
      buf = nullptr;
      emit update(_cachelines);
      return 1;
    } else {
      int als = (add/_linesize)*_linesize;
      int ale = ((add+len-1)/_linesize)*_linesize;
      uint8_t *tmp = new uint8_t[_linesize*(ale-als+1)];
      bool missed = false;
      for (int j = als; j <= ale; ++j) {
        Cacheline* candidate = inCache(j);
        if(candidate == nullptr) {// there is a miss
          ++miss;
          missed = true;
          if(nextLevel != nullptr) {
            uint8_t* tmpblk = new uint8_t[_linesize];
            ++countdown;
            int flag = nextLevel->load(j, tmpblk, _linesize);
            while(flag == 0) {
              ++countdown;//
              flag = nextLevel->load(j, tmpblk, _linesize);
            }
            if(flag == -1) {
              countdown = cycle;
              delete[] tmpblk;
              preadd = -1;
              prelen = -1;
              missReady = false;
              delete[] buf;
              buf = nullptr;
              return -1;
            }
            Cacheline* cacheline = evict(j);
            // copy data into the current cacheline
            for(int i = 0; i < _linesize; i++) {
                cacheline->data[i] = tmpblk[i];
                tmp[(j-als)*_linesize+i] = tmpblk[i];
            }
            cacheline->valid = true;
            cacheline->tag = (j/_linesize)/_indexsize;
            cacheline->lru = _ways;
            delete[] tmpblk;
          } else {
            countdown = cycle;
            preadd = -1;
            prelen = -1;
            missReady = false;
            delete[] buf;
            buf = nullptr;
            return -1;
          }
        } else {
          ++hit;
          for (int i=0; i<_linesize; ++i) {
            tmp[(j-als)*_linesize+i] = candidate->data[i];
          }
        }
        if (policy == 1){
          visitLRU(j);
        }
      }
      if (missed) {
        missReady = true;
        buf = new uint8_t[len];
        for (int i=0; i<len; ++i) {
          buf[i] = tmp[add-als+i];
        }
        delete[] tmp;
        return 0;
      } else {
        for (int i=0; i<len; ++i) {
          blk[i] = tmp[add-als+i];
        }
        countdown = cycle;
        preadd = -1;
        prelen = -1;
        missReady = false;
        delete[] buf;
        buf = nullptr;
        delete[] tmp;
        return 1;
      }
    }
  } else {
    return 0;
  }
}

int Cache::store(uint32_t add, uint8_t *blk, int len) {
  if (preadd == -1 && prelen == -1) {
    preadd = add;
    prelen = len;
  } else if (preadd != add || prelen != len) {
    std::cout<<"error add access! current: "<<add
    <<" "<<len<<" pending: "<<preadd<<" "<<prelen<<std::endl;
    return -1;
  }
  if(countdown>0) {
    countdown--;
  }
  if(countdown == 0) {
    if (missReady) {
      countdown = cycle;
      preadd = -1;
      prelen = -1;
      missReady = false;
      emit update(_cachelines);
      return 1;
    } else {
      int als = (add/_linesize)*_linesize;
      int ale = ((add+len-1)/_linesize)*_linesize;
      bool missed = false;
      int i=0;
      for (int j=als; j<=ale; ++j) {
        Cacheline* candidate = inCache(j);
        if(candidate != nullptr) {
          ++hit;
          if (add>j) {
            while(add-j+i<_linesize && i<len) {
              candidate->data[add-j+i] = blk[i];
              ++i;
            }
          } else {
            for(int k=0; k<_linesize && i<len; ++k) {
              candidate->data[k] = blk[i];
              ++i;
            }
          }
          candidate->dirty = true;
        } else {
          ++miss;
          missed = true;
          if (add+i > j || add+len-1 < j+_linesize) {
            if(nextLevel != nullptr) {
              uint8_t* tmpblk = new uint8_t[_linesize];
              ++countdown;
              int flag = nextLevel->load(j, tmpblk, _linesize);
              while(flag == 0) {
                ++countdown;//
                flag = nextLevel->load(j, tmpblk, _linesize);
              }
              if(flag == -1) {
                countdown = cycle;
                delete[] tmpblk;
                preadd = -1;
                prelen = -1;
                missReady = false;
                return -1;
              }
              Cacheline* cacheline = evict(j);
              // copy data into the current cacheline
              for(int k = 0; k < _linesize; k++) {
                  cacheline->data[k] = tmpblk[k];
              }
              while(add-j+i<_linesize && i<len) {
                cacheline->data[add-j+i] = blk[i];
                ++i;
              }
              cacheline->dirty = true;
              cacheline->valid = true;
              cacheline->tag = (j/_linesize)/_indexsize;
              cacheline->lru = _ways;
              delete[] tmpblk;
            } else {
              countdown = cycle;
              preadd = -1;
              prelen = -1;
              missReady = false;
              return -1;
            }
          } else {
            Cacheline* cacheline = evict(j);
            for(int k = 0; k < _linesize && i<len; ++k) {
                cacheline->data[k] = blk[i];
                ++i;
            }
            cacheline->valid = true;
            cacheline->dirty = true;
            cacheline->tag = (j/_linesize)/_indexsize;
            cacheline->lru = _ways;
          }
        }
        if (policy == 1) {
          visitLRU(j);
        }
      }
      if (missed) {
        missReady = true;
        return 0;
      } else {
        countdown = cycle;
        preadd = -1;
        prelen = -1;
        missReady = false;
        emit update(_cachelines);
        return 1;
      }
    }
  } else {
    return 0;
  }
}

Cacheline* Cache::inCache(int add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int idx = ((add/_linesize)%_indexsize)*_ways;
    int tag = (add/_linesize)/_indexsize;
    for (int i = 0; i < _ways; i++) {
        if(_cachelines[idx+i].valid == false) {
          continue;
        }
        if (_cachelines[idx+i].tag == tag) {
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

  int evictedWay = genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
  if (policy == 1) {
    evictedWay = getLRUNumber(idx);
  }
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

void Cache::reset() {
  hit = 0;
  miss = 0;
  countdown = cycle;
  missReady = false;
  preadd = -1;
  prelen = -1;
  delete[] buf;
  buf = nullptr;
  delete[] _cachelines;
  _cachelines = new Cacheline[_cachesize];
  for (int i = 0; i < _cachesize; ++i) {
    _cachelines[i].data = new uint8_t[_linesize];
  }
}

std::string Cache::dump() {
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

int Cache::getLRUNumber(int idx) {
  idx = (idx/_ways)*_ways;
  int mi = 0;
  for (int i=0; i<_ways; ++i) {
    if (lru[idx+i]>lru[idx+mi]) {
      mi = i;
    }
  }
  return mi;
}

void Cache::visitLRU(int add) {
  int idx = ((add/_linesize)%_indexsize)*_ways;
  int tag = (add/_linesize)/_indexsize;
  for (int i = 0; i < _ways; i++) {
      if(_cachelines[idx+i].valid == false) {
        continue;
      }
      if (_cachelines[idx+i].tag == tag) {
        for(int j=0; j<_ways; ++j) {
          if (_cachelines[idx+j].valid == true && lru[idx+j] < lru[idx+i]) {
            lru[idx+j]++;
          }
        }
        lru[idx+i] = 0;
        break;
      }
  }
  return;
}

