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
//#include <QObject>

class Storage {
  public:
    virtual ~Storage() {}
    int cycle;
    Storage *nextLevel;
    virtual int load(uint32_t address, uint8_t *block, int length) = 0;
    virtual int store(uint32_t address, uint8_t *block, int length) = 0;
    virtual std::string dump() = 0;
};

#endif /* defined(__MIPS_Simulator__Storage__) */
