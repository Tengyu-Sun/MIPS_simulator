I implemented a multi-way dissociative cache with write-back policy, which can be used to build a muiti-level memory system.
Memory and cache all inherit fromt the same parent class Memcache.
Each object contains a pointer to the lower level storage. 
Main functionalities:

load(int address)

load a block from this address. If this address exists in current level of storage, after countdown is reduced to zero, return it (wrapped by a message class). If the countdown is not zero, reduce it by 1 each call. If this address doesn't exist in current level, call the load function from the lower level storage.

store(int address, int* block)

write a block into this address. If this address exists in current level of storage, after countdown if reduced to zero, label the dirty flag to 1. if it doesn't exist in current level of storage, evict a line (or find an empty line corresponding to this address), write the content into this line and label the dirty flag to 1. (it should be done aftter the countdown is reduced to 0)


