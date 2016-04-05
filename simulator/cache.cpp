#include <random>
#include "cache.h"
#include <iostream>

int genRandomNumber(int ways) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, ways-1);
    return distribution(generator);
}

Cache::Cache(int indexsize, int linesize, int ways, int cycle_, int policy_, Storage* nextLevel_) {
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
  _idle = true;
  _add = 0;
  _len = -1;
  buf = nullptr;
  policy = policy_;
  _cachelines = new Cacheline[_cachesize];
  //lru = new int[_cachesize];
  for (int i = 0; i < _cachesize; ++i) {
    _cachelines[i].data = new uint8_t[_linesize];
    //lru[i] = 0;
  }
}

Cache::~Cache() {
  delete[] _cachelines;
  //delete[] lru;
  delete[] buf;
}

int Cache::load(uint32_t add, uint8_t *blk, int len) {
  if (_idle) {
    _add = add;
    _len = len;
  } else if (_add != add || _len != len) {
    return -1;
  }
  if(countdown > 0) {
    countdown--;
  }
  if(countdown == 0) {

    if (missReady) {
      for (int i = 0; i < len; ++i) {
        blk[i] = buf[i];
      }
      countdown = cycle;
      _add = 0;
      _len = -1;
      missReady = false;
      _idle = true;
      delete[] buf;
      buf = nullptr;
      return len;
    } else {
      uint32_t als = (add/_linesize)*_linesize;
      uint32_t ale = ((add+len-1)/_linesize)*_linesize;

      uint8_t *tmp = new uint8_t[ale-als+_linesize];
      bool missed = false;
      for (uint32_t j = als; j <= ale; j = j+_linesize) {
        int candidate = inCache(j);
        if(candidate == -1) {// there is a miss
          ++miss;
          emit updateMiss(miss);
          missed = true;
          if(nextLevel != nullptr) {
            uint8_t* tmpblk = new uint8_t[_linesize];
            ++countdown;
            int flag = nextLevel->load(j, tmpblk, _linesize);
            while(flag == 0) {
              ++countdown;
              flag = nextLevel->load(j, tmpblk, _linesize);
            }
            if(flag == -1) {
              countdown = cycle;
              delete[] tmpblk;
              delete[] tmp;
              _add = 0;
              _len = -1;
              missReady = false;
              _idle = true;
              delete[] buf;
              buf = nullptr;
              return -1;
            }
            int eline = evict(j);
            // copy data into the current cacheline
            for(int i = 0; i < _linesize; i++) {
                _cachelines[eline].data[i] = tmpblk[i];
                tmp[j-als+i] = tmpblk[i];
            }
            _cachelines[eline].valid = true;
            _cachelines[eline].tag = (j/_linesize)/_indexsize;
            if (policy == 1) {
              _cachelines[eline].lru = _ways;
            }
            delete[] tmpblk;
            emit updateCacheline(_cachelines, eline);
          } else {
            countdown = cycle;
            _add = 0;
            _len = -1;
            missReady = false;
            _idle = true;
            delete[] buf;
            buf = nullptr;
            delete[] tmp;
            return -1;
          }
        } else {
          ++hit;
          emit updateHit(hit);
          for (int i=0; i<_linesize; ++i) {
            tmp[j-als+i] = _cachelines[candidate].data[i];
          }
        }
        if (policy == 1){
          visitLRU(j);
        }
      }
      if (missed) {
        missReady = true;
        buf = new uint8_t[len];
        for (int i = 0; i < len; ++i) {
          buf[i] = tmp[add-als+i];
        }
        delete[] tmp;
        return 0;
      } else {
        for (int i = 0; i < len; ++i) {
          blk[i] = tmp[add-als+i];
        }
        countdown = cycle;
        _add = 0;
        _len = -1;
        missReady = false;
        _idle = true;
        delete[] buf;
        buf = nullptr;
        delete[] tmp;
        return len;
      }
    }
  } else {
    return 0;
  }
}

int Cache::store(uint32_t add, uint8_t *blk, int len) {
  if (_idle) {
    _add = add;
    _len = len;
  } else if (_add != add || _len != len) {
    return -1;
  }
  if(countdown > 0) {
    countdown--;
  }
  if(countdown == 0) {
    if (missReady) {
      countdown = cycle;
      _add = 0;
      _len = -1;
      missReady = false;
      _idle = true;
      return len;
    } else {
      uint32_t als = (add/_linesize)*_linesize;
      uint32_t ale = ((add+len-1)/_linesize)*_linesize;
      bool missed = false;
      int i = 0;
      for (uint32_t j = als; j <= ale; j = j+_linesize) {
        int candidate = inCache(j);
        if(candidate != -1) {
          ++hit;
          emit updateHit(hit);
          if (add+i > j) {
            while(add-j+i < _linesize && i < len) {
              _cachelines[candidate].data[add-j+i] = blk[i];
              ++i;
            }
          } else {
            for(int k = 0; k < _linesize && i < len; ++k) {
              _cachelines[candidate].data[k] = blk[i];
              ++i;
            }
          }
          _cachelines[candidate].dirty = true;
          emit updateCacheline(_cachelines, candidate);
        } else {
          ++miss;
          emit updateMiss(miss);
          missed = true;
          if (add+i > j || add+len-1 < j+_linesize-1) {
            if(nextLevel != nullptr) {
              uint8_t* tmpblk = new uint8_t[_linesize];
              ++countdown;
              int flag = nextLevel->load(j, tmpblk, _linesize);
              while(flag == 0) {
                ++countdown;
                flag = nextLevel->load(j, tmpblk, _linesize);
              }
              if(flag == -1) {
                countdown = cycle;
                delete[] tmpblk;
                _add = 0;
                _len = -1;
                missReady = false;
                _idle = true;
                return -1;
              }
              int eline = evict(j);
              // copy data into the current cacheline
              for(int k = 0; k < _linesize; k++) {
                  _cachelines[eline].data[k] = tmpblk[k];
              }
              _cachelines[eline].dirty = true;
              _cachelines[eline].valid = true;
              _cachelines[eline].tag = (j/_linesize)/_indexsize;

              if (policy == 1) {
                _cachelines[eline].lru = _ways;
              }
              while(add-j+i < _linesize && i < len) {
                _cachelines[eline].data[add-j+i] = blk[i];//
                //std::cout<<i<<" "<<add-j+i<<" "<<(int)(_cachelines[eline].data[add-j+i])<<std::endl;
                ++i;
              }
              emit updateCacheline(_cachelines, eline);
              delete[] tmpblk;
            } else {
              countdown = cycle;
              _add = 0;
              _len = -1;
              missReady = false;
              _idle = true;
              return -1;
            }
          } else {
            int eline = evict(j);
            for(int k = 0; k < _linesize && i < len; ++k) {
                _cachelines[eline].data[k] = blk[i];
                ++i;
            }
            _cachelines[eline].valid = true;
            _cachelines[eline].dirty = true;
            _cachelines[eline].tag = (j/_linesize)/_indexsize;
            if (policy == 1) {
              _cachelines[eline].lru = _ways;
            }
            emit updateCacheline(_cachelines, eline);
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
        _add = 0;
        _len = -1;
        missReady = false;
        _idle = true;
        return len;
      }
    }
  } else {
    return 0;
  }
}

int Cache::inCache(uint32_t add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int idx = ((add/_linesize)%_indexsize)*_ways;
    int tag = (add/_linesize)/_indexsize;
    for (int i = idx; i < idx+_ways; ++i) {
        if(_cachelines[i].valid == false) {
          continue;
        }
        if (_cachelines[i].tag == tag) {
            return i;
        }
    }
    //std::cout<<add<<" missed"<<std::endl;
    return -1;
}


// evict a cacheline from the current block (referenced by blockNumber) if all ways are occupied, return the cleared line
// if there is a line not occupied, return it
int Cache::evict(uint32_t add) {
  int idx = ((add/_linesize)%_indexsize)*_ways;
  for(int i = idx; i < idx+_ways; i++) {
      // return the empty line;
      if(_cachelines[i].valid == false) {
        return i;
      }
  }
  // if all ways are written, evict it to lower level storage, return the cleared line;

  int evictedWay =  idx + genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
  if (policy == 1) {
    evictedWay = idx + getLRUNumber(idx);
  }
  if(_cachelines[evictedWay].dirty == true) {
      if(nextLevel != nullptr) {
        // write back to lower level of storage if the dirty flag is set to 1.
        uint8_t *block = _cachelines[evictedWay].data;
        uint32_t eadd = (evictedWay/_ways)*_linesize + _cachelines[evictedWay].tag*_linesize*_indexsize;
        while(nextLevel->store(eadd, block, _linesize) == 0);
      }
  }
  _cachelines[evictedWay].valid = false;
  _cachelines[evictedWay].dirty = false;
  return evictedWay;
}

void Cache::reset() {
  hit = 0;
  miss = 0;
  countdown = cycle;
  missReady = false;
  _add = 0;
  _len = -1;
  _idle = true;
  delete[] buf;
  buf = nullptr;
  delete[] _cachelines;
  //delete[] lru;
  _cachelines = new Cacheline[_cachesize];
  //lru = new int[_cachesize];
  for (int i = 0; i < _cachesize; ++i) {
    _cachelines[i].data = new uint8_t[_linesize];
  //  lru[i] = 0;
  }
}

std::string Cache::dump() {
  std::string res;
  for(int i=0; i<_cachesize; ++i) {
    res += std::to_string(_cachelines[i].tag) + " " + std::to_string((int)_cachelines[i].valid)
     + " " +std::to_string((int)_cachelines[i].dirty)
    + " " + std::to_string(_cachelines[i].lru);
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
    if (_cachelines[idx+i].lru>_cachelines[idx+mi].lru) {
      mi = i;
    }
  }
  return mi;
}

void Cache::visitLRU(uint32_t add) {
  int idx = ((add/_linesize)%_indexsize)*_ways;
  int tag = (add/_linesize)/_indexsize;
  for (int i = 0; i < _ways; i++) {
      if(_cachelines[idx+i].valid == false) {
        continue;
      }
      if (_cachelines[idx+i].tag == tag) {
        for(int j = 0; j < _ways; ++j) {
          if (_cachelines[idx+j].valid == true && _cachelines[idx+j].lru < _cachelines[idx+i].lru) {
            (_cachelines[idx+j].lru)++;
            emit updateCacheline(_cachelines, idx+j);
          }
        }
        _cachelines[idx+i].lru = 0;
        emit updateCacheline(_cachelines, idx+i);
        break;
      }
  }
  return;
}
