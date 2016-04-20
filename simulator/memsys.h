#ifndef __MIPS_Simulator__Memsys__
#define __MIPS_Simulator__Memsys__

#include "cache.h"
#include "memory.h"
#include <string>
#include <vector>
#include <map>
#include <QObject>

class MemSys : public QObject {
    Q_OBJECT
 public:
  MemSys() {
    _mainMemory = nullptr;
    _caches = std::vector<Cache*>();
  }
  ~MemSys() {
      delete _mainMemory;
      for (int i = 0; i < _caches.size(); ++i) {
          delete _caches[i];
      }
  }
  void init(MemSysConfig config);
  int loadWord(uint32_t add, uint32_t *val);
  int storeWord(uint32_t add, uint32_t val);
  int loadByte(uint32_t add, uint8_t *val);
  int storeByte(uint32_t add, uint8_t val);
  int directWriteByte(uint32_t add, uint8_t val);
  int directWriteWord(uint32_t add, uint32_t val);
  std::string dump();
  void resetCache();
  void resetMem();
  void fresh() {
      emit memNotify(_mainMemory->_data, 0, _config.memSize);
  }
  void clear() {
      busy = false;
  }
  bool isBusy() {
      return busy;
  }

  std::string getRequest() {
      return request;
  }

  MemSysConfig _config;
  std::vector<Cache*> _caches;
  Memory *_mainMemory;

signals:
  void memNotify(uint8_t *data, uint32_t add, int len);
  void cacheLineNotify(int level, Cacheline** data, int idx, int way);
  void cacheHitNotify(int level, int hit);
  void cacheMissNotify(int level, int miss);

private slots:
  void memChange(uint8_t *data, uint32_t add, int len);
  void cacheLineChange(Cacheline** data, int idx, int way);
  void cacheHitChange(int hit);
  void cacheMissChange(int miss);

private:
  std::string request;
  int countdown;
  bool busy;
  uint32_t buf32;
  uint8_t buf8;
};

#endif /* defined(__MIPS_Simulator__Memsys__) */
