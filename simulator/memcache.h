//
//  memcache.h
//  MIPS_Simulator
//
//  Created by Wei Hong on 2/28/16.
//  Copyright (c) 2016 com.wei. All rights reserved.
//

#ifndef __MIPS_Simulator__Memcache__
#define __MIPS_Simulator__Memcache__

// struct message {
//   int* data;//?
//   bool ok;
//   message() {
//     data = nullptr;
//     ok = false;
//   }
// };
class Memcache {
  public:
    int circle;
    int countdown;
    Memcache *nextLevel;
    virtual int load(int address, int *blk, int len) = 0;// { return msg; }
    virtual int store(int address, int *blk, int len) = 0;// { return msg; }
};

#endif /* defined(__MIPS_Simulator__Memcache__) */
