# cache-sim  

[![GitHub release](https://img.shields.io/github/release/cache-sim/cache-sim.svg?style=for-the-badge)](https://github.com/cache-sim/cache-sim/releases/latest)
[![Travis (.org)](https://img.shields.io/travis/cache-sim/cache-sim.svg?style=for-the-badge)](https://travis-ci.org/cache-sim/cache-sim)

*A multi-level cache simulator built using C++.*  

## Work to be done (latest update: 8 Semptember, 2020)

- [x] Implement the 2D matrix for modelling the cache block content
- [x] Add the Cache Controller wrapper and interface it with cache of all levels 
- [x] Implement a DRAM controller to store and load 64B cacheline content onto/from 2D matrix wrapper
- [ ] Design the behaviour of cache controller in a finite-state machine fashion based on the ```fsm_cache.jpg```
- [ ] Add the Base-Delta-Immediate Cache Compression mecahnism to eviction of cache block from last-level cache
- [ ] Add the DRAM address mapping analyser to assess the memory utilisation of both non-zero and zero rows 

## Note (9 September, 2020):
So far the FSM of cache controller is almost completed but the 2D Matrix manipulation at eviction between LLC and DRAM is not finished yet.
The operation of state WRITE-BACK and ALLOCATION ought to be reconsidered again. 


## Installation  

No installations needed here, unless you don't have the ncurses library, in which case just run  
```sudo apt-get install libncurses5-dev```

## Input Format  

The configuration for the cache you want to simulate has to be given in the form of a text file. A sample format can be found in ```paras.cfg```  

The input format is as so:  
```
<levels>  
<policy> <cacheSize> <blockSize> <setAssociativity>
.  
.  
.  
```  

The trace for the simulator is expected to be a gzip file generated the [Pin](https://software.intel.com/en-us/articles/pin-a-binary-instrumentation-tool-downloads) tool from Intel.  

## Usage  

```./run.sh -t|--trace <TRACE> -c|--config <CONFIG_FILE> [-i|--interactive] [-d|--debug]```  
The ```-i``` option outputs the hits and misses in all the levels of the cache in live time (obviously making it slower but hopefully increases your patience :).  
The ```-d``` option checks if the the cacheBlock chosen by the policy indeed matches the index of the address being accessed, just a safety measure.  

## Contribution  

To add support for more eviction policies, have a look at the template folder and declare the required data structures and complete the functions (don't forget to put them in the policies folder). If the template seems to be lacking something, raise an issue and we can have a look at it.  
