# ECE6100_Lab_4
CMP Memory System Design (Individual assignment)

## Overview  
![](https://raw.githubusercontent.com/wanchen1112/ECE6100_Lab_4/master/picture.png)

## Objective
This project is the last lab assignment of ECE6100. The objective is to implement multi-level cache simulator with DRAM-based main memory. And this implementation will be extended to incorporate multiple cores, where each core has a private L1 (I and D) cache, and a shared L2 cache. Cache misses and writebacks from the shared L2 cache are serviced by a DRAM-based main memory consisting of 8 banks and per-bank row buffers.

## Background & Implementation

#### Phase 1: Design a standalone cache module
In this part, the cache miss rate is estimated (for DCACHE). For each instruction, instruction PC and instruction type (ALU, LOAD, and STORE) are provided in the trace file. As we are only interested in cache hit/miss information, there is no exact data values in the cache. What I implemented are the *cache_install()* and *cache_access()* functions. 
- The **cache_access()** function is called when the instruction is LOAD or STORE, and the hit/miss information will be returned via this function.
- If the cache line is not present, then **cache_install()** is called to install the new line to cache. The cache eviction is implemented in this function as well.

#### Phase 2: Multi-level cache 
In this part, based on the first phase implementation, the multi-level cache is implemented. We will have private L1 caches (ICACHE and DCACHE) and a shared L2 cache. The implemented functions are as follows:
- In **memsys_access_modeBC()**, it will update the required cache access latency for cache hit or miss.
- In **memsys_L2_access()**, the cache line is installed from DRAM if not present. And if it is dirty eviction, the cache line will be written back to DRAM.

#### Phase 3: Simple DRAM 
In this part, a basic row buffer (RB) for a DRAM system is implemented. Assuming there are 8 banks in DRAM and the *open page policy* is applied on the RB.
- In **dram_access_extra_credit()**, the content in RB is determined by the memory address and RB size (1K bytes for this project). 
- When the memory address is in the same as last access, it is a RB hit, then the latency will be only the sum of tCAS (column access strobe) and tBUS. 
- However, if the row is not the same as last access, it is a RB conflict. In such cases, the latency will be the sum of tBUS, tCAS, tACT (Activate a DRAM page/row), and tPRE (precharge).
- The initial case will be a RB miss (no content matched and present in RB): the latency will be the (tBUS + tCAS + tACT).

#### Phase 4: Multicore system
In this part, the multicore system is established. Each core has its own L1 caches. Besides, the cache replacement policy can be specified for each cache. For L2 cache, LRU replacement is implemented. For each core, we have different trace files to execute the experiments.

#### Phase 5: Static way partitioning
Shared caches may encouter worse performance due to neighbior applications attempt to access the same part of the cache. This problem can be mitigated with way partitioning, whereby each core can be given a certain number of ways per set as their quota. The victim selection is split into several ways to improve the performance.

## Notes:
- The evaluation for this project is to estimate the memory/cache access latency. Therefore, there is no real data processing in the implementation. Instead, we accumulated the corresponding latency for memory/cache access and had a statistics result included in \result.
- The assignment submission was divided into three parts: Part A: Phase 1, Part B: Phases 2 & 3, and Part C: Phases 4 & 5.
