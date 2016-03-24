Implementation of a MIPS simulator consisting of Assembler, pipelined CPU, hierarchical memory system.
Current status:(done)
1. Memory system
    (a)Tunable multi-way associative cache with hierarchical structure
	(b)Random eviction
	(c)Write back, allocate policy
2. CPU
	(a)Basic pipline
	(b)Full ISA support
3. Assembler
	2-pass, translate assembly language into binary machine code

Todo:
1. LRU cacheline eviction
2. Fully functional pipeline
3. Benchmark programs (exchange sorting, matrix multiplication, etc)
4. Branch prediction?
	
