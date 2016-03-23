//
//  Memcache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Memcache__
#define __MIPS_Simulator__Memcache__


//#include <stdio.h>
struct message {
  int* data;//?
  bool ok;
  message() {
    data = nullptr;
    ok = false;
  }
};


class Memcache {
 public:
    int circle;
    int countdown;
    Memcache *nextLevel;
    virtual message load(int address) = 0;// { return msg; }
    virtual message store(int address, int value) = 0;// { return msg; }
    virtual message store(int address, int* block) = 0;// { return msg; }
};

#endif /* defined(__MIPS_Simulator__Memcache__) */
