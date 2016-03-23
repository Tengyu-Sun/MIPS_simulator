#include <iostream>
#include "Memcache.h"
#include "Memory.h"
#include "Cache.h"

using namespace std;
//#include <stdio.h>
#include <random>


int main() {
 // initialize memory and test
    int memory_size = 1<<13;
    int block_size = 1<<1;
    int ways = 8;
    int memory_cycle = 100;
    int cache_cycle = 5;
    int cache_size = 1<<6;
    Memory memory(memory_size, block_size, nullptr,memory_cycle);
    int* memory_data = memory.getData();
    for(int i = 0; i < memory_size; i++) {
       memory_data[i] = i;
       // cout<< memory_data[i] <<endl;
    }
    
 // initialize cache and test
    //Cache L2Cache(cache_size, block_size, ways, &memory, cache_cycle);
    Cache L1Cache(cache_size/2, block_size, ways, &memory,cache_cycle/2);
    int max_address = 100;
    for(int i = 0; i < max_address; i++) {
      //  int address = genRandomNumber(max_address);
        int address = i;
        L1Cache.load(address);
    }
    
    for(int i = 0; i < max_address; i++) {
        //  int address = genRandomNumber(max_address);
        int address = i;
        L1Cache.load(address);
    }
    cout <<"hit: " <<L1Cache.hit << " miss: " << L1Cache.miss<< endl;
    return 0;
}
