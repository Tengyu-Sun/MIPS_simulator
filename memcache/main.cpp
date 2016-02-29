#include <iostream>
#include "Memcache.h"
#include "Memory.h"
#include "Cache.h"

using namespace std;
//#include <stdio.h>

void load_memory() {
    
    
}

int main() {
 // initialize memory and test
    int memory_size = 1<<13;
    int block_size = 1<<3;
    int ways = 1;
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
    Cache cache(cache_size, block_size, ways, &memory, cache_cycle);
    cache.load(10);
    cout<< (cache.inCache(10)==nullptr) << endl;
    cout<<"miss: " << cache.miss << " hit: " << cache.hit<<endl;
    cache.load(10);
    cout<< (cache.inCache(10)==nullptr) << endl;
    cout<<"miss: " << cache.miss << " hit: " << cache.hit<<endl;
    return 0;
}
