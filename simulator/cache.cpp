#include "cache.h"
#include <random>
//#include <iostream>



Cache::Cache(int indexsize, int linesize, int ways, int cycle_, ReplacePolicy rpolicy_, WritePolicy wpolicy_, Storage* nextLevel_) {
  _indexsize = indexsize;
  _linesize = linesize;
  _ways = ways;
  cycle = cycle_;
  nextLevel = nextLevel_;
  hit = 0;
  miss = 0;
  rpolicy = rpolicy_;
  wpolicy = wpolicy_;
  _cachelines = new Cacheline*[_indexsize];

  for (int i = 0; i < _indexsize; ++i) {
    _cachelines[i] = new Cacheline[_ways];
    for (int j = 0; j < _ways; ++j) {
      _cachelines[i][j].data = new uint8_t[_linesize];
    }
  }
}

Cache::~Cache() {
  for (int i = 0; i < _indexsize; ++i) {
    delete[] _cachelines[i];
  }
  delete[] _cachelines;
}

int Cache::load(uint32_t add, uint8_t *blk, int len) {
  int t = cycle;

  uint32_t als = (add/_linesize)*_linesize;
  uint32_t ale = ((add+len-1)/_linesize)*_linesize;
  uint8_t *tmp = new uint8_t[ale-als+_linesize];


  for (uint32_t j = als; j <= ale; j = j+_linesize) {
    Position candidate = inCache(j);
    if(candidate.idx == -1) {// there is a miss
      ++miss;
      emit updateMiss(miss);
      if(nextLevel != nullptr) {
        uint8_t* tmpblk = new uint8_t[_linesize];
        int flag = nextLevel->load(j, tmpblk, _linesize);
        if(flag == -1) {
          delete[] tmpblk;
          delete[] tmp;
          return -1;
        }
        t += flag;
        flag = 0;
        Position eline = evict(j, flag);
        if(flag == -1) {
          delete[] tmpblk;
          delete[] tmp;
          return -1;
        }
        t += flag;
        // copy data into the current cacheline
        for(int i = 0; i < _linesize; i++) {
            _cachelines[eline.idx][eline.way].data[i] = tmpblk[i];
            tmp[j-als+i] = tmpblk[i];
        }
        _cachelines[eline.idx][eline.way].valid = true;
        _cachelines[eline.idx][eline.way].tag = (j/_linesize)/_indexsize;
        if (rpolicy == ReplacePolicy::LRU) {
          _cachelines[eline.idx][eline.way].lru = _ways;
        }
        delete[] tmpblk;
        emit updateLine(_cachelines, eline.idx, eline.way);
      } else {
        delete[] tmp;
        return -1;
      }
    } else {
      ++hit;

      emit updateHit(hit);
      for (int i = 0; i < _linesize; ++i) {
        tmp[j-als+i] = _cachelines[candidate.idx][candidate.way].data[i];
      }
    }
    if (rpolicy == ReplacePolicy::LRU) {
      visitLRU(j);
    }
  }

  for (int i = 0; i < len; ++i) {
    blk[i] = tmp[add-als+i];
  }
  delete[] tmp;
  return t;
}

int Cache::store(uint32_t add, uint8_t *blk, int len) {
  int t = cycle;
  uint32_t als = (add/_linesize)*_linesize;
  uint32_t ale = ((add+len-1)/_linesize)*_linesize;

  int i = 0;
  for (uint32_t j = als; j <= ale; j = j+_linesize) {
    Position candidate = inCache(j);
    if(candidate.idx != -1) {
      ++hit;
      
      emit updateHit(hit);
    //  if (add+i > j) {
      while(add-j+i < _linesize && i < len) {
        _cachelines[candidate.idx][candidate.way].data[add-j+i] = blk[i];
        ++i;
      }
      // } else {
      //   for(int k = 0; k < _linesize && i < len; ++k) {
      //     _cachelines[candidate.idx][candidate.way].data[k] = blk[i];
      //     ++i;
      //   }
      // }
      if (wpolicy == WritePolicy::WRITEBACK) {
        _cachelines[candidate.idx][candidate.way].dirty = true;
      } else {
        uint8_t *tmp = _cachelines[candidate.idx][candidate.way].data;
        int flag = 0;
        if (nextLevel != nullptr) {
          flag = nextLevel->store(j, tmp, _linesize);
        }
        if (flag == -1) {
          return -1;
        }
        t += flag;
        _cachelines[candidate.idx][candidate.way].dirty = false;
      }
      emit updateLine(_cachelines, candidate.idx, candidate.way);
    } else {
      ++miss;
      emit updateMiss(miss);
      uint8_t* tmpblk = nullptr;
      int flag = 0;
      if (add+i > j || add+len-1 < j+_linesize-1) {
        if(nextLevel != nullptr) {
          tmpblk = new uint8_t[_linesize];
          flag = nextLevel->load(j, tmpblk, _linesize);
          if(flag == -1) {
            delete[] tmpblk;
            return -1;
          }
          t += flag;
          // Position eline = evict(j, flag);
          // // copy data into the current cacheline
          // if(flag == -1) {
          //   delete[] tmpblk;
          //   return -1;
          // }
          // t += flag;
          // for(int k = 0; k < _linesize; k++) {
          //     _cachelines[eline.idx][eline.way].data[k] = tmpblk[k];
          // }
          // _cachelines[eline.idx][eline.way].valid = true;
          // _cachelines[eline.idx][eline.way].tag = (j/_linesize)/_indexsize;
          // if (rpolicy == ReplacePolicy::LRU) {
          //   _cachelines[eline.idx][eline.way].lru = _ways;
          // }
          // while(add-j+i < _linesize && i < len) {
          //   _cachelines[eline.idx][eline.way].data[add-j+i] = blk[i];//
          //   //std::cout<<i<<" "<<add-j+i<<" "<<(int)(_cachelines[eline].data[add-j+i])<<std::endl;
          //   ++i;
          // }
          // if (wpolicy == WritePolicy::WRITEBACK) {
          //   _cachelines[eline.idx][eline.way].dirty = true;
          // } else {
          //   uint8_t *tmp = _cachelines[eline.idx][eline.way].data;
          //   int flag = 0;
          //   if (nextLevel != nullptr) {
          //     flag = nextLevel->store(j, tmp, _linesize);
          //   }
          //   if (flag == -1) {
          //     return -1;
          //   }
          //   t += flag;
          //   _cachelines[eline.idx][eline.way].dirty = false;
          // }
        //  emit updateCacheline(_cachelines, eline);
          //delete[] tmpblk;
        } else {
          return -1;
        }
      }// else {
        // int flag = 0;
        // Position eline = evict(j, flag);
        // if (flag == -1) {
        //   return -1;
        // }
        // t += flag;
        // for(int k = 0; k < _linesize && i < len; ++k) {
        //     _cachelines[eline.idx][eline.way].data[k] = blk[i];
        //     ++i;
        // }
        // _cachelines[eline.idx][eline.way].valid = true;
        //
        // _cachelines[eline.idx][eline.way].tag = (j/_linesize)/_indexsize;
        // if (rpolicy == ReplacePolicy::LRU) {
        //   _cachelines[eline.idx][eline.way].lru = _ways;
        // }
        // if (wpolicy == WritePolicy::WRITEBACK) {
        //   _cachelines[eline.idx][eline.way].dirty = true;
        // } else {
        //   uint8_t *tmp = _cachelines[eline.idx][eline.way].data;
        //   int flag = 0;
        //   if (nextLevel != nullptr) {
        //     flag = nextLevel->store(j, tmp, _linesize);
        //   }
        //   if (flag == -1) {
        //     return -1;
        //   }
        //   t += flag;
        //   _cachelines[eline.idx][eline.way].dirty = false;
        // }
        //emit updateCacheline(_cachelines, eline);
      //}
      flag = 0;
      Position eline = evict(j, flag);
      // copy data into the current cacheline
      if(flag == -1) {
        delete[] tmpblk;
        return -1;
      }
      t += flag;
      if (tmpblk != nullptr) {
        for(int k = 0; k < _linesize; k++) {
            _cachelines[eline.idx][eline.way].data[k] = tmpblk[k];
        }
        delete[] tmpblk;
      }

      _cachelines[eline.idx][eline.way].valid = true;
      _cachelines[eline.idx][eline.way].tag = (j/_linesize)/_indexsize;
      if (rpolicy == ReplacePolicy::LRU) {
        _cachelines[eline.idx][eline.way].lru = _ways;
      }

      while(add-j+i < _linesize && i < len) {
        _cachelines[eline.idx][eline.way].data[add-j+i] = blk[i];//
        //std::cout<<i<<" "<<add-j+i<<" "<<(int)(_cachelines[eline].data[add-j+i])<<std::endl;
        ++i;
      }
      if (wpolicy == WritePolicy::WRITEBACK) {
        _cachelines[eline.idx][eline.way].dirty = true;
      } else {
        uint8_t *tmp = _cachelines[eline.idx][eline.way].data;
        flag = 0;
        if (nextLevel != nullptr) {
          flag = nextLevel->store(j, tmp, _linesize);
        }
        if (flag == -1) {
          return -1;
        }
        t += flag;
        _cachelines[eline.idx][eline.way].dirty = false;
      }
      emit updateLine(_cachelines, eline.idx, eline.way);
    }
    if (rpolicy == ReplacePolicy::LRU) {
      visitLRU(j);
    }
  }
  return t;
}

Position Cache::inCache(uint32_t add) {// if the address is valid and exists in the cache, return a pointer to the cacheline.
    // calculate the block number
    int idx = (add/_linesize)%_indexsize;
    int tag = (add/_linesize)/_indexsize;
    Position res;
    for (int i = 0; i < _ways; ++i) {
        if(_cachelines[idx][i].valid == false) {
          continue;
        }
        if (_cachelines[idx][i].tag == tag) {
            res.idx = idx;
            res.way = i;
            return res;
        }
    }
    //std::cout<<add<<" missed"<<std::endl;
    return res;
}


// evict a cacheline from the current block (referenced by blockNumber) if all ways are occupied, return the cleared line
// if there is a line not occupied, return it
Position Cache::evict(uint32_t add, int &t) {
  int idx = (add/_linesize)%_indexsize;
  Position res;
  for(int i = 0; i < _ways; i++) {
      // return the empty line;
      if(_cachelines[idx][i].valid == false) {
        res.idx = idx;
        res.way = i;
        t = 0;
        return res;
      }
  }
  // if all ways are written, evict it to lower level storage, return the cleared line;

  int evictedWay = 0;

  if (rpolicy == ReplacePolicy::LRU) {
     evictedWay = getLRUNumber(idx);
  } else {
     evictedWay = genRandomNumber(_ways);// if all ways are occupied, we have to randommly evict one line of them.
  }

  if(_cachelines[idx][evictedWay].dirty == true) {
      if(nextLevel != nullptr) {
        // write back to lower level of storage if the dirty flag is set to 1.
        uint8_t *block = _cachelines[idx][evictedWay].data;
        uint32_t eadd = idx*_linesize + _cachelines[idx][evictedWay].tag*_linesize*_indexsize;
        t =  nextLevel->store(eadd, block, _linesize);
      } else {
        t = -1;
        return res;
      }
  }
  _cachelines[idx][evictedWay].valid = false;
  _cachelines[idx][evictedWay].dirty = false;
  res.idx = idx;
  res.way = evictedWay;
  return res;
}

void Cache::reset() {
  hit = 0;
  miss = 0;
  for (int i = 0; i < _indexsize; ++i) {
    delete[] _cachelines[i];
  }
  delete[] _cachelines;

  _cachelines = new Cacheline*[_indexsize];

  for (int i = 0; i < _indexsize; ++i) {
    _cachelines[i] = new Cacheline[_ways];
    for (int j = 0; j < _ways; ++j) {
          _cachelines[i][j].data = new uint8_t[_linesize];
    }
  }
}

std::string Cache::dump() {
  std::string res = "index, way: tag | valid | dirty | (lru) | data...\n";
  for(int i = 0; i < _indexsize; ++i) {
    for (int j = 0; j < _ways; ++j) {
      res += std::to_string(i) + ", " + std::to_string(j) + ": "+ std::to_string(_cachelines[i][j].tag) +
      " | " + std::to_string((int)_cachelines[i][j].valid)
       + " | " +std::to_string((int)_cachelines[i][j].dirty);
      if (rpolicy == ReplacePolicy::LRU) {
          res += " | " + std::to_string(_cachelines[i][j].lru);
      }
      for (int k = 0; k < _linesize; ++k) {
        res += std::to_string(_cachelines[i][j].data[k]) + " ";
      }
      res += "\n";
    }
  }
  return res;
}

int Cache::getLRUNumber(int idx) {
  int mi = 0;
  for (int i = 0; i < _ways; ++i) {
    if (_cachelines[idx][i].lru > _cachelines[idx][mi].lru) {
      mi = i;
    }
  }
  return mi;
}

int Cache::genRandomNumber(int ways) {
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, ways-1);
    return distribution(generator);
}

void Cache::visitLRU(uint32_t add) {
  int idx = (add/_linesize)%_indexsize;
  int tag = (add/_linesize)/_indexsize;
  for (int i = 0; i < _ways; i++) {
      if(_cachelines[idx][i].valid == false) {
        continue;
      }
      if (_cachelines[idx][i].tag == tag) {
        for(int j = 0; j < _ways; ++j) {
          if (_cachelines[idx][j].valid == true && _cachelines[idx][j].lru < _cachelines[idx][i].lru) {
            (_cachelines[idx][j].lru)++;
            emit updateLine(_cachelines, idx, j);
          }
        }
        _cachelines[idx][i].lru = 0;
        emit updateLine(_cachelines, idx, i);
        break;
      }
  }
  return;
}
