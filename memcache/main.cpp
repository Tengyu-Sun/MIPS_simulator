#include<iostream>
#include "Memcache.h"
#include "Memory.h"
#include "Cache.h"

using namespace std;
#include <stdio.h>

int main() 
{
 // initialize memory and test

 Memory memory(100,2,NULL);
 //cout << memory.countdown <<endl;
 int* data = memory.getData();
 cout << data[10] <<endl;
 int block[2] = {222,333};
 message write_msg = memory.store(10,block);
 while(!write_msg.ok) {
	write_msg = memory.store(10,block);
 } 
 message msg = memory.load(10); 
 while(!msg.ok) {
	msg = memory.load(10);
 }
 for(int i = 0; i < 2; i++) {
    cout << *msg.data++ << endl;
 }

    
 // initialize cache and test
    
 return 0;
}
