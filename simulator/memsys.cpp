#include "memsys.h"
#include <fstream>
#include <iostream>

void init(MemSysConfig config) {
  busy = false;
  request = "";
  return;
}

int MemSys::loadWord(uint32_t add, uint32_t* val) {
  if(add+3 > _memSize - 1 || add%4 != 0) {
    std::cout<<add<<" "<<_memSize<<std::endl;
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
  if(add > _memSize - 1) {
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
    if(add + 3 > _memSize - 1 || add%4 != 0) {
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
    if(add > _memSize - 1) {
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
    if(add > _config._memSize - 1) {
      return -1;
    }
    return _mainMemory->store(add, &val, 1);
}

int MemSys::directWriteWord(uint32_t add, uint32_t val) {
    if(add + 3 > _config._memSize - 1 || add%4 != 0) {
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

void resetCache() {
  int n = _caches.size();
  for (int i = 0; i < n; ++i) {
    _caches[i]->reset();
  }
  return;
}

std::string MemSys::dump() {
  std::string res;
  if(_config._cacheOn) {
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
