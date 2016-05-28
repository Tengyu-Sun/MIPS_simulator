#Design and Implementation of a RISC simulator 
##Wei Hong, Tengyu Sun
### It consists of an assembler / dissambler, a pipelined CPU, a hierarchical memory system, etc:  
1. Memory system (C++)   
    (a)Tunable multi-way associative cache with hierarchical structure.   
	(b)Random / LRU eviction.   
	(c)Write back / through, allocate policy.  
2. CPU (C++)  
	(a) Five stages pipline.  
	(b) ISA support for a subset of MIPS. (see the report form instruction details)  
3. Assembler / disassembler (C++)  
	(a)A 2-pass Assembler to translate assembly language into binary machine code.  
	(b)A dissemlber to translate binary machine code into assembly language.  
4. Benchmark programs: (Assembly)  
	(a) Exchange sort.  
	(b) Matrix multiplication.  
	(c) Vectorized matrix multiplication.  
5. User interface (Qt,C++)  

