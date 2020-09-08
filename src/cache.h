#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>

#define WORD_SIZE_8B // 8 bytes
#ifdef WORD_SIZE_8B
	#define WORD_SIZE 8 // 8 bytes
#else
	#define WORD_SIZE 8 // 8 bytes
#endif

// Enumeration of Finite-State Machine for Cache Controller
#define CACHE_FSM_NUM 4 // four states in total
enum cache_fsm {
	IDLE        = 0,
	COMPARE_TAG = 1,
	WRITE_BACK  = 2,
	ALLOCATE    = 3
};

// classes define
class Cache;
struct block_content_t;
typedef struct block_content_t* block_vector;

// helper functions
bool isValidConfig(long long cs, long long bs, long long sa);
long long hexToDec(char hexVal[]);
int log2(long long x);

void incReads();
void incWrites();
long long getReads();
long long getWrites();
unsigned int decToHex(char *hex_str, long long dec_val);
unsigned int decToHex(char *hex_str, long long dec_val);

typedef struct block_content_t {
	// 64-bit (8-byte) store/load value is presented by 16 digit in heximal format
	long long *word_val; // decimal format
    char valid_dirty; // only the last 2 bits are used, LSB+1: validity and LSB: dirty
	//unsigned int *valid_set; // vector to mark the valid/non-empty word index, e.g., 5 -> 4 -> EOF means 5th and 4th word position were written some value
				 // It should be noted that the order of this vector does not matter.	
} block_content;

class block_matrix {
	//private:
    public:
		block_content **word_vector;
		long long set_num;
		long long way_num;
        unsigned int word_num; // number of words in one cache block, e.g., 8 words in cache block of 64-byte size
		
	//public:
        bool isValid(long long set_id, long long block_id);
        bool isDirty(long long set_id, long long block_id);
		block_matrix(unsigned short setAssociativity, long long numberOfSets, long long blockSize);
		~block_matrix();
        void block_flush(long long set_id, long long block_id); // when the eviction occurs, the original content should be reset to all zero
        void block_write_back(long long set_id, long long block_id, long long *block_val); // update the content by write-back process from the upper level cache
        void block_modify(long long set_id, long long block_id, unsigned int word_index, long long word_val); // modify the block content by one word whenever the cache his occur and instruction type is "WRITE"
        long long fetch_word(long long set_id, long long block_id, unsigned int word_index);
};

// cache class
class Cache{
    private:
        long long hits, misses;
        long long* cacheBlocks;
        int level;
        std::string policy;

    public:
	    block_matrix *words;
        void incHits();
        void incMisses();
        int getLevel();
        std::string getPolicy();
        long long getTag(long long address);
        long long getIndex(long long address);
        long long getBlockPosition(long long address);
	    unsigned int getWordOffset(long long address);
        void insert(long long address, long long blockToReplace, ll word_val);

        long long getHits();
        long long getMisses();
        float getHitRate();

        virtual long long getBlockToReplace(long long address) = 0;
        virtual void update(long long blockToReplace, int status) = 0;

        virtual ~Cache();

    protected:
        Cache(long long cacheSize, long long blockSize, unsigned short setAssociativity, int level, std::string policy);
        long long cacheSize;
        long long blockSize;
        unsigned short setAssociativity;
        long long numberOfSets;
        int offsetSize;
        int indexSize;
};

enum TRANS_TYPE {
    WRITE_WORD  = 0x00,
    READ_WORD   = 0x01,
    WRITE_BLOCK = 0x02
};
typedef struct trans_package_t {
    short levelItr;
    long long address;
    long long *evictedBlock_val;
    long long rqst_word_val; // either read request or write request
    char trans_type;

   trans_package_t(short a, long long b, long long *c, long long d, char e) {
	levelItr = a;
	address = b;
	evictedBlock_val = c;
	rqst_word_val = d;
	trans_type = e;
   }
} trans_package;

class cache_contronller {
    private:
        vector<Cache*> *cache;
        unsigned short setAssociativity;
        int level_num;

    public:
        cache_controller(vector<Cache*> cache_interface, unsigned short way_num, int level);
        bool state_machine(trans_package *trans);
};