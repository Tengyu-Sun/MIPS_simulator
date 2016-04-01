//
//  storage.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Storage__
#define __MIPS_Simulator__Storage__

#include <cstdint>
#include <string>
#include <QObject>

class Storage : public QObject {
    Q_OBJECT
  public:
    virtual ~Storage() {}
    int cycle;
    int countdown;
    uint32_t _add;//simultaneous read write?
    int _len;
    Storage *nextLevel;
    bool _idle;
    virtual int load(uint32_t address, uint8_t *blk, int len) = 0;
    virtual int store(uint32_t address, uint8_t *blk, int len) = 0;
    virtual std::string dump() = 0;
};

#endif /* defined(__MIPS_Simulator__Storage__) */
