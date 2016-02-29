#include "cache.h"

Cache::Cache(int cachesize, int linesize, int ways, int circle_, Memcache* nextLevel_) {
  _cachesize = cachesize;
  _linesize = linesize;
  _ways = ways;
  circle = circle_;
  countdown = circle_;
  nextLevel = nextLevel_;
  int nc = _cachesize*_ways;
  _cachelines = new Cacheline[nc];
  for (int i = 0; i < nc; ++i) {
    _cachelines[i].data = new int[_linesize];
  }
}

int Cache::load(int add, int *blk, int len) {
  
}

int Cache::store(int add, int *blk, int len) {
    return 1;
}

int Cache::load(int add, int *val) {
    return 1;
}

int Cache::store(int add, int val) {
    return 1;
}
