#include <iostream>
#include "Memcache.h"
#include "Memory.h"
#include "Cache.h"

using namespace std;
//#include <stdio.h>

int main() {
 // initialize memory and test
 Memory memory(100,NULL);
 cout << memory.countdown <<endl;
 int* data = memory.getData();
 cout << data[10] <<endl;
 message write_msg = memory.store(10,2222);
 while(!write_msg.ok) {
	write_msg = memory.store(10,2222);
 }
 message msg = memory.load(10);
 while(!msg.ok) {
	msg = memory.load(10);
 }
 cout << msg.value << endl;

 // initialize cache and test
 return 0;
}
