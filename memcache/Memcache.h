//
//  Memcache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Memcache__
#define __MIPS_Simulator__Memcache__


#include <stdio.h>
typedef struct {
    int* data;
    bool ok;
}message;


class Memcache
{
public:
    int countdown;
    Memcache *nextLevel;
    message  load(int address);
    message  store(int address, int value);
    
};

#endif /* defined(__MIPS_Simulator__Memcache__) */
