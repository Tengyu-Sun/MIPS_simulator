#include "memsys.h"
#include <fstream>
#include <iostream>

void MemSys::init(MemSysConfig config) {
  busy = false;
  request = "";
  countdown = 0;
  buf32 = 0;
  buf8 = 0;
  _config = config;
  if(_mainMemory != nullptr) {
    delete _mainMemory;
    _mainMemory = nullptr;
  }
  if(_caches.size() > 0) {
    for (int i = 0; i < _caches.size(); ++i) {
        delete _caches[i];
    }
    _caches = std::vector<Cache*>();
  }
  _mainMemory = new Memory(_config.memSize, _config.memCycle);
  QObject::connect(_mainMemory, &Memory::notify, this, &MemSys::memChange);
  if (_config.cacheLevel > 0) {
    _caches = std::vector<Cache*>(_config.cacheLevel);
    _caches[_config.cacheLevel-1] = new Cache(_config.cacheSettings[_config.cacheLevel-1].indexsize,
      _config.cacheSettings[_config.cacheLevel-1].linesize, _config.cacheSettings[_config.cacheLevel-1].ways,
      _config.cacheSettings[_config.cacheLevel-1].cycle, _config.cacheSettings[_config.cacheLevel-1].rpolicy,
       _config.cacheSettings[_config.cacheLevel-1].wpolicy, _mainMemory);
    QObject::connect(_caches[_config.cacheLevel-1], &Cache::updateLine, this, &MemSys::cacheLineChange);
    QObject::connect(_caches[_config.cacheLevel-1], &Cache::updateHit, this, &MemSys::cacheHitChange);
    QObject::connect(_caches[_config.cacheLevel-1], &Cache::updateMiss, this, &MemSys::cacheMissChange);
    for (int i = _config.cacheLevel-2; i >= 0; --i) {
      _caches[i] = new Cache(_config.cacheSettings[i].indexsize,
        _config.cacheSettings[i].linesize, _config.cacheSettings[i].ways,
        _config.cacheSettings[i].cycle, _config.cacheSettings[i].rpolicy,
         _config.cacheSettings[i].wpolicy, _caches[i+1]);
      QObject::connect(_caches[i], &Cache::updateLine, this, &MemSys::cacheLineChange);
      QObject::connect(_caches[i], &Cache::updateHit, this, &MemSys::cacheHitChange);
      QObject::connect(_caches[i], &Cache::updateMiss, this, &MemSys::cacheMissChange);
    }
  }
  //std::cout<<"index "<<_config.cacheSettings[_config.cacheLevel-1].indexsize<<std::endl;
  return;
}

int MemSys::loadWord(uint32_t add, uint32_t* val) {
  if(add+3 > _config.memSize - 1 || add%4 != 0) {
    std::cout<<add<<" "<<_config.memSize<<std::endl;
    return -1;
  }
  std::string tmp = "lw" + std::to_string(add);
  if (busy && tmp != request) {
    return 0;
  } else if (!busy) {
    request = tmp;
    busy = true;
    uint8_t blk[4];
    int flag = -1;
    if (_config.cacheOn) {
        flag = _caches[0]->load(add, blk, 4);
    } else {
        flag = _mainMemory->load(add, blk, 4);
    }
    if(flag == -1) {
      busy = false;
      return -1;
    }
    buf32 = (blk[0]<<24) | (blk[1]<<16) | (blk[2]<<8) | blk[3];
    countdown = flag;
  }
  if (countdown > 0) {
    --countdown;
  }
  if (countdown == 0) {
    *val = buf32;
    busy = false;
    return 1;
  } else {
    return 0;
  }
}

int MemSys::loadByte(uint32_t add, uint8_t* val) {
  if(add > _config.memSize - 1) {
    return -1;
  }
  std::string tmp = "lb" + std::to_string(add);
  if (busy && tmp != request) {
    return 0;
  } else if (!busy) {
    request = tmp;
    busy = true;
    int flag = -1;
    if (_config.cacheOn) {
        flag = _caches[0]->load(add, &buf8, 1);
    } else {
        flag = _mainMemory->load(add, &buf8, 1);
    }
    if(flag == -1) {
      busy = false;
      return -1;
    }
    countdown = flag;
  }
  if (countdown > 0) {
    --countdown;
  }
  if (countdown == 0) {
    *val = buf8;
    busy = false;
    return 1;
  } else {
    return 0;
  }
}

int MemSys::storeWord(uint32_t add, uint32_t val) {
    if(add + 3 > _config.memSize - 1 || add%4 != 0) {
      return -1;
    }
    std::string tmp = "sw" + std::to_string(add);
    if (busy && tmp != request) {
      return 0;
    } else if (!busy) {
      request = tmp;
      busy = true;
      uint8_t blk[4];
      blk[3] = val & 0xff;
      val >>= 8;
      blk[2] = val & 0xff;
      val >>= 8;
      blk[1] = val & 0xff;
      val >>=8;
      blk[0] = val;
      int flag = -1;
      if (_config.cacheOn) {
          flag = _caches[0]->store(add, blk, 4);
      } else {
          flag = _mainMemory->store(add, blk, 4);
      }
      if(flag == -1) {
        busy = false;
        return -1;
      }
      countdown = flag;
    }
    if (countdown > 0) {
      --countdown;
    }
    if (countdown == 0) {
      busy = false;
      return 1;
    } else {
      return 0;
    }
}

int MemSys::storeByte(uint32_t add, uint8_t val) {
    //std::cout<<"sb "<<add<<" "<<(int)val<<std::endl;
    if(add > _config.memSize - 1) {
      return -1;
    }
    std::string tmp = "sb" + std::to_string(add);
    if (busy && tmp != request) {
      return 0;
    } else if (!busy) {
      request = tmp;
      busy = true;
      int flag = -1;
      if (_config.cacheOn) {
          flag = _caches[0]->store(add, &val, 1);
      } else {
          flag = _mainMemory->store(add, &val, 1);
      }
      if(flag == -1) {
        busy = false;
        return -1;
      }
      countdown = flag;
    }
    if (countdown > 0) {
      --countdown;
    }
    if (countdown == 0) {
      busy = false;
      return 1;
    } else {
      return 0;
    }
}

int MemSys::directWriteByte(uint32_t add, uint8_t val) {
    if(add > _config.memSize - 1) {
      return -1;
    }
    return _mainMemory->store(add, &val, 1);
}

int MemSys::directWriteWord(uint32_t add, uint32_t val) {
    if(add + 3 > _config.memSize - 1 || add%4 != 0) {
      return -1;
    }
    uint8_t tmp[4];
    tmp[3] = val & 0xff;
    val >>= 8;
    tmp[2] = val & 0xff;
    val >>= 8;
    tmp[1] = val & 0xff;
    val >>=8;
    tmp[0] = val;
    return _mainMemory->store(add, tmp, 4);
}

void MemSys::resetCache() {
  int n = _caches.size();
  for (int i = 0; i < n; ++i) {
    _caches[i]->reset();
  }
  return;
}

void MemSys::resetMem() {
    _mainMemory->reset();
}

std::string MemSys::dump() {
  std::string res;
  if(_config.cacheOn) {
    res += "cache:\n";
    int n = _caches.size();
    for (int i = 0; i < n; ++i) {
      res += _caches[i]->dump();
    }
  }
  res += "\nmemory:\n";
  res += _mainMemory->dump();
  return res;
}

void MemSys::memChange(uint8_t *data, uint32_t add, int len) {
    emit memNotify(data, add, len);
}

void MemSys::cacheHitChange(int hit) {
    Cache* sender = (Cache*)QObject::sender();
    int level = -1;
    for (int i = 0; i < _caches.size(); ++i) {
        if (_caches[i] == sender) {
            level = i;
            break;
        }
    }
    if (level >= 0) {
        emit cacheHitNotify(level, hit);
    }
}

void MemSys::cacheMissChange(int miss) {
    Cache* sender = (Cache*)QObject::sender();
    int level = -1;
    for (int i = 0; i < _caches.size(); ++i) {
        if (_caches[i] == sender) {
            level = i;
            break;
        }
    }
    if (level >= 0) {
       emit cacheMissNotify(level, miss);
    }
}

void MemSys::cacheLineChange(Cacheline **data, int idx, int way) {
    Cache* sender = (Cache*)QObject::sender();
    int level = -1;
    for (int i = 0; i < _caches.size(); ++i) {
        if (_caches[i] == sender) {
            level = i;
            break;
        }
    }
    if (level >= 0) {
       emit cacheLineNotify(level, data, idx, way);
    }
}
