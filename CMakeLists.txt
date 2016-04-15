cmake_minimum_required(VERSION 3.5)
project(MIPS_simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    asm/asm.cpp
    asm/exchange_sort.cpp
    asm/matrix_mulplication.cpp
    GenTestCases/GenTestCases.cpp
    memcache/Cache.cpp
    memcache/Cache.h
    memcache/Globals.cpp
    memcache/Globals.h
    memcache/main.cpp
    memcache/Memcache.cpp
    memcache/Memcache.h
    memcache/Memory.cpp
    memcache/Memory.h
    simulator/cache.cpp
    simulator/cache.h
    simulator/cacheview.h
    simulator/commons.h
    simulator/configdialog.cpp
    simulator/configdialog.h
    simulator/cpu.cpp
    simulator/cpu.h
    simulator/cpuview.h
    simulator/fpu.cpp
    simulator/fpu.h
    simulator/main.cpp
    simulator/memcache.h
    simulator/memory.cpp
    simulator/memory.h
    simulator/memoryview.h
    simulator/memsys.cpp
    simulator/memsys.h
    simulator/simulator.cpp
    simulator/simulator.h
    simulator/storage.h
    simulator/vu.cpp
    simulator/vu.h asm/disasm.cpp)

add_executable(MIPS_simulator ${SOURCE_FILES})