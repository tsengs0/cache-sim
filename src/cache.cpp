#include "cache.h"

typedef long long ll;

ll readCounter=0, writeCounter=0;

// helper functions

bool isValidConfig(ll  cs, ll bs, ll sa){ // needs more analysis
    if((float)cs/(bs*sa) - (int)(cs/(bs*sa)) < 0.0001){ // ignoring precision errors
        return true;
    }
    else {
        return false;
    }
}

/*
ll hexToDec(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    ll base = 1;
    ll decVal = 0;

    for (int i=len-1; i>=2; i--){ //2 to avoid 0x
        if (hexVal[i]>='0' && hexVal[i]<='9'){
            decVal += (hexVal[i] - '0')*base;
        }
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f'){
            decVal += (hexVal[i] - 'a' + 10) * base;
        }
        base *= 16;
    }
    return decVal;
}
*/
// No Prefix of "0x"
ll hexToDec(char hexVal[]) {
    int len = strlen(hexVal);

    // Initializing base value to 1, i.e 16^0 
    ll base = 1;
    ll decVal = 0;

    for (int i=len-1; i>=0; i--){
        if (hexVal[i]>='0' && hexVal[i]<='9'){
            decVal += (hexVal[i] - '0')*base;
        }
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f'){
            decVal += (hexVal[i] - 'a' + 10) * base;
        }
        base *= 16;
    }
    return decVal;
}

int log2(ll x) {
    int power = 0;
    while(x > 1) {
        x = x>>1;
        power++;
    }
    return power;
}

void incReads(){
    readCounter++;
}

void incWrites(){
    writeCounter++;
}

ll getReads(){
    return readCounter;
}

ll getWrites(){
    return writeCounter;
}

std::string Cache::getPolicy(){
    return policy;
}

unsigned int decToHex(char *hex_str, long long dec_val)
{
    long long quot, temp; 
    unsigned int i, j; 
    quot=dec_val; 
	i = 0;
	
	while(quot != 0)
    	{	 
    		temp=quot % 16; 
    		if(temp<10) 
    		{ 
			hex_str[i++] = 0x30 + temp;
    		} 
    		else 
    		{ 
			hex_str[i++] = temp + 87;
    		}
    		quot=quot / 16; 
    	}
	
	return i; 
}

// Cache class
Cache::Cache(ll cacheSize, ll blockSize, unsigned short setAssociativity, int level, std::string policy){
    
    if(!isValidConfig(cacheSize, blockSize, setAssociativity)){
        printf("Invalid Cache configuration\n");
    }

    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    word_num = this -> blockSize / (long long) WORD_SIZE;
    this->setAssociativity = setAssociativity;
    this->level = level;
    this->policy = policy;
    
    words = new word_matrix(this -> setAssociativity, this -> numberOfSets, this -> blockSize);
   
    cacheBlocks = (ll*)malloc(cacheSize/blockSize * sizeof(ll));
    if(cacheBlocks == NULL){
        printf("Failed to allocate memory for L%d cache\n", this->level);
        exit(0);
    }

    numberOfSets = cacheSize/(blockSize*setAssociativity);
    offsetSize = log2(blockSize);
    indexSize = log2(numberOfSets);

    std::cout << "L" << level << " cache, size: " << (cacheSize/1024) << "-KiB" << std::endl
	      << blockSize << "B cache-line, " << setAssociativity << "-way" << std::endl
	      << "Set depth: " << numberOfSets << ", Address-(index, Byte-offset): (" << indexSize << ", " << offsetSize << ")" << std::endl << std::endl;

}

void Cache::incHits(){
    hits++;
}

void Cache::incMisses(){
    misses++;
}

int Cache::getLevel(){
    return level;
}

ll Cache::getTag(ll address){
    return address>>(indexSize + offsetSize);
}

ll Cache::getIndex(ll address){
    return (address>>offsetSize) & ((1<<indexSize)-1);
}

unsigned int getWordOffset(long long address) 
{
	long long offset_mask;
	offset_mask = (long long) offsetSize;
	offset_mask = (1 << offset_mask) - 1;
	return (unsigned int) (address & offset_mask);
}
 
ll Cache::getBlockPosition(ll address){
    ll index = getIndex(address);
    ll tag = getTag(address);
    ll iterator;
    for(iterator=index*setAssociativity; iterator<(index+1)*setAssociativity; iterator++){
        if(tag == cacheBlocks[iterator]){
            return (words -> isValid) ? iterator : -1;
        }
    }
    return -1;
}

void Cache::insert(ll address, ll blockToReplace, ll word_val) 
{
    ll index_id, block_id;
    unsigned int word_index;

    #ifdef DEBUG
    if(getIndex(address) != blockToReplace/setAssociativity){
        printf("ERROR: Invalid insertion: Address %x placed in block %lld", address, blockToReplace);
    }
    #endif
    cacheBlocks[blockToReplace] = getTag(address);
    index_id = getIndex(address);
    block_id = blockToReplace % setAssociativity;
    word_index = getWordOffset(address); 

    words -> word_vector[index_id][block_id].word_val[word_index] = word_val;
}

ll Cache::getHits(){
    return hits;
}

ll Cache::getMisses(){
    return misses;
}

float Cache::getHitRate(){
    if(hits+misses == 0){
        return 0;
    }
    return (float)(hits)/(hits+misses);
}

Cache::~Cache(){
    free(cacheBlocks);
    delete words;
}

block_matrix::word_matrix(unsigned short setAssociativity, long long numberOfSets, long long blockSize)
{
	set_num = numberOfSets;
	way_num = setAssociativity;
	word_vector = new block_vector[set_num];
    unsigned int blockSize_8B = blockSize / (long long) WORD_SIZE;
	for(long long i = 0; i < set_num; i++) {
		word_vector[i] = new block_content[way_num];
		for(long long j = 0; j < way_num; j++) {
            valid_dirty[i][j] = 0x00;
			word_vector[i][j].word_val = new long long[blockSize_8B]();
		}
	}
}

void block_matrix::block_flush(long long set_id, long long block_id)
{
    for(long long i = 0; i < word_num; i++) {
        word_vector[set_id][block_id].word_val[i] = (long long) 0;
    }
    word_vector[set_id][block_id].valid_dirty = 0x00;
}

// The update from the upper level cache
void block_matrix::block_write_back(long long set_id, long long block_id, long long *block_val)
{
    for(unsigned int i = 0; i < word_num; i++)
        word_vector[set_id][block_id].word_val[i] = block_val[i];
    word_vector[set_id][block_id].valid_dirty = 0x03;
}

// modify the block content by one word whenever the cache his occur and instruction type is "WRITE"
void block_modify(long long set_id, long long block_id, unsigned int word_index, long long word_val)
{
    word_vector[set_id][block_id].word_val[word_index] = word_val;
    word_vector[set_id][block_id].valid_dirty |= 0x01; 
}

 long long fetch_word(long long set_id, long long block_id, unsigned int word_index)
 {
   return word_vector[set_id][block_id].word_val[word_index]  
 }

bool block_matrix::isValid(long long set_id, long long block_id)
{
    return (word_vector[set_id][block_id].valid_dirty & (0x01 << 1)) ? true : false;
}
bool block_matrix::isDirty(long long set_id, long long block_id)
{
     return (word_vector[set_id][block_id].valid_dirty & 0x01) ? true : false;
}

block_matrix::~block_matrix()
{
	for(long long i = 0; i < set_num; i++) {
		for (long long j = 0; j < way_num; j++)
			delete [] word_vector[i][j].word_val;
		delete [] word_vector[i];
	}
	delete [] word_vector;
}


cache_controller::cache_controller(vector<Cache*> cache_interface, unsigned short way_num, int level)
{
    cache = cache_interface;
    setAssociativity = way_num;
    level_num = level;
}

bool cache_controller::state_machine(trans_package *trans)
{
    short state_cur;
    short levelItr;              levelItr = trans -> levelItr;
    long long address;           address = trans -> address;
    long long *evictedBlock_val; evictedBlock_val = trans -> evictedBlock_val;
    long long rqst_word_val;     rqst_word_val = trans -> rqst_word_val;
    char trans_type;             trans_type = trans -> trans_type;
    bool cache_hit;

    cache_hit = false;
    state_cur = cache_fsm::IDLE;
    while(cache_hit == false) {
		switch(i) {
			case cache_fsm::IDLE:      
                state_cur = cache_fsm::COMPARE_TAG;
                break;  
			
            case cache_fsm::COMPARE_TAG: 
                ll block = (*cache)[levelItr] -> getBlockPosition(address); // getBlockPosition will be implemented in cache.cpp
                // Case A) Hit and Valid: block address
                // Case B) Miss but Old Block is clean
                // Case C) Miss and Old Block is dirty
                if(block == -1) { //cache miss
                    (*cache)[levelItr] -> incMisses(); // incMisses will be implemented in cache.cpp
                    ll blockToReplace = (*cache)[levelItr] -> getBlockToReplace(address);
                    ll index = (*cache)[levelItr] -> getIndex(address);
                    bool dirty = (*cache)[levelItr] -> words->isDirty(index, blockToReplace);
                    if(dirty == true) {   
                        state_cur = cache_fsm::WRITE_BACK;
                    }
                    else { // dirty == false
                        state_cur = cache_fsm::ALLOCATE;
                    }
                }
                else { // cache hit
                    //if(levelItr > 0) cout << "hit at L" << levelItr+1 << endl;
                    cache[levelItr]->incHits(); // incHits will be implemented in cache.cpp
                    cache[levelItr]->update(block, 1); // update will be implemented in policy.cpp; 1 denotes hit
                    if(trans_type == TRANS_TYPE::WRITE_WORD) { // Write Request
                        block_modify (
                                index, // set index 
                                block % setAssociativity, 
                                (*cache)[levelItr] -> getWordOffset(address), 
                                rqst_word_val
                        );
                    }
                    else if(trans_type == TRANS_TYPE::READ_WORD){ // Read Request
                        trans -> rqst_word_val = fetch_word (
                                                    index, // set index
                                                    block % setAssociativity,
                                                    (*cache)[levelItr] -> getWordOffset(address)
                                                ); 
                    }
                    else if(trans_type == TRANS_TYPE::WRITE_BLOCK) {
                        if(levelItr < (level_num-1)) {
                            block_write_back (
                                index, // set index
                                block % setAssociativity,
                                (*cahce)[levelItr] -> words -> word_vector[index][blockToReplace % setAssociativity].word_val
                            );
                        }
                        else {
                            cout << "The write back to main memory should not be passed to this state" << endl;
                            exit(1);
                        }
                    }
                    
                    else {
                        cout << "Invalid transaction, the possible type of transaction are write word, read word and write block" << endl;
                        exit(1);
                    } 
                        
                    cache_hit = true; // finish cache access and ASSERTION to signal the "Cache Hit" consequence
                }
                break;
			
            case cache_fsm::WRITE_BACK: 
                if(levelItr != (level_num-1)) {
	                trans_package *wb_trans = new trans_package(
                                                    levelItr+1, // proceed to next backing level
                                                    address, 
                                                    (*cahce)[levelItr] -> words -> word_vector[index][blockToReplace % setAssociativity].word_val, 
                                                    (long long) 0, 
                                                    (char) TRANS_TYPE::WRITE_BLOCK
                                                );
                    // write-back the dirty/modified cache block to cache of backing level
                    state_machine(wb_trans);
	                delete [] wb_trans;
                    //cout << "Evicted Cache Block (from L" << levelItr+1 << "): " << blockToReplace << "\t->\t";
                }
                else {
                    //cout << "Evicted Cache Block (from L" << levelItr+1 << "): " << blockToReplace << endl;
                    dram_trace << dec << address << "," << "W,";
                    for(unsigned int i = 0; i < (*cahce)[levelItr] -> words -> word_num; i++) {
                        //char *block_val_log = new char[(WORD_SIZE*8) / 4]();
                        //decToHex(block_val_log, evictedBlock_val[i]);
                        if(i < ((*cahce)[levelItr] -> words -> word_num)-1)
                            dram_trace << dec << (*cahce)[levelItr] -> words -> word_vector[index][blockToReplace % setAssociativity].word_val[i] << ",";
                        else // last word of the cache block
                            dram_trace << dec << (*cahce)[levelItr] -> words -> word_vector[index][blockToReplace % setAssociativity].word_val[i] << endl;
                        //delete [] block_val_log;
                    }
                }                   

                break; 
			
            case cache_fsm::ALLOCATE:    
                    (*cache)[levelItr] -> insert(address, blockToReplace, rqst_word_val); // insert will be implemented in cache.cpp
                else { // Read Request
                    (*cahce)[levelItr] -> words -> word_vector[index][blockToReplace % setAssociativity].word_val[i]
                     
                }

                // update will be implemented in policy.cpp; will include updating the tree as in plru or updating the count as in lfu; 0 denotes miss
                (*cache)[levelItr][levelItr]->update(blockToReplace, 0); 


                break;
            
            default:

                break;
		}
        #if INTERACTIVE
        printTraceInfo();
        printCacheStatus((*cache)[levelItr]);
        #endif
	}

}