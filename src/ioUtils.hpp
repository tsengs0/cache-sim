#include "cache.h"
#include <stdio.h>
#if INTERACTIVE
#include <ncurses.h>
#endif

#define ll long long

#define instructionOffset 30

#define levelOffset 0
#define hitRateOffset 55
#define accessesOffset 0
#define hitsOffset 30
#define missesOffset 55


//#define TRACE_ADDR_TYPE // the type of input trace file is "pinatrace.out" where the instruction address, store/load type and  reference address are dumped
#define TRACE_ADDR_VAL_TYPE // the type of input trace file is "mem_addr_val.out" where the store/load type, reference address and stored/loaded value are dumped
#ifdef TRACE_ADDR_TYPE
ll getNextAddress(){
    char instruction[20], accessType, address[20];
    scanf("%s %c %s", instruction, &accessType, address);
    if(strcpy(instruction, "#eof") == 0) //return address 0 if end of line
        return 0;
    if(accessType == 'R')
        incReads();
    else
        incWrites();
    return hexToDec(address);
}
#else
void getNextAddress(ll *ref_addr, ll *ref_val, bool *is_wr){
    char accessType[5], address[20], imm_val[17];
    scanf("%s %s %s", accessType, address, imm_val);
    if(strcpy(accessType, "#eof") == 0) {//return address 0 if end of line
        *ref_addr = 0; 
	*ref_val  = 0;
    }
    else {
    	if(accessType[0] == 'R') {
        	incReads();
		*is_wr = false;
	}
    	else {
        	incWrites();
		*is_wr = true;
	}
    	*ref_addr = hexToDec(address);
    	*ref_val  = hexToDec(imm_val);
    }
}
#endif

#ifdef INTERACTIVE
void printTraceInfoOutline(){
    move(0, 0);
    printw("Total Number Of Instructions:"); refresh();
    move(1, 0);
    printw("Number of Reads             :"); refresh();
    move(2, 0);
    printw("Number of Writes            :"); refresh();
}

void printTraceInfo(){
    move(0, instructionOffset);
    printw("%lld", getReads() + getWrites()); refresh();
    move(1, instructionOffset);
    printw("%lld", getReads()); refresh();
    move(2, instructionOffset);
    printw("%lld", getWrites()); refresh();
}
#endif
void printTraceInfo2(){
    printf("\n");
    printf("Total Number Of Instructions: %lld\n", getReads() + getWrites());
    printf("Number of Reads             : %lld\n", getReads());
    printf("Number of Writes            : %lld\n", getWrites());   
    printf("\n");
}

#ifdef INTERACTIVE
void printCacheStatusOutline(Cache* cache){
    int verticalOffset = 5 + 3 * (cache->getLevel()-1);
    move(verticalOffset, levelOffset);
    attron(A_BOLD);
    printw("L%d: %s", cache->getLevel(), cache->getPolicy().c_str()); refresh();
    attroff(A_BOLD);
    move(verticalOffset, hitRateOffset);
    printw("Hit Rate: "); refresh();
    move(verticalOffset + 1, accessesOffset);
    printw("Accesses: "); refresh();
    move(verticalOffset + 1, hitsOffset);
    printw("Hits: "); refresh();
    move(verticalOffset + 1, missesOffset);
    printw("Misses: "); refresh();
}

void printCacheStatus(Cache* cache){
    int verticalOffset = 5 + 3 * (cache->getLevel()-1);
    move(verticalOffset, hitRateOffset+10);
    printw("%f", cache->getHitRate()); refresh();
    move(verticalOffset+1, accessesOffset+10);
    printw("%lld", cache->getHits()+cache->getMisses()); refresh();
    move(verticalOffset+1, hitsOffset+6);
    printw("%lld", cache->getHits()); refresh();
    move(verticalOffset+1, missesOffset+8);
    printw("%lld", cache->getMisses()); refresh();
}
#endif
void printCacheStatus2(Cache* cache){
    printf("L%d: %s\t\t\t\t\t\tHit Rate: %f\n",  
        cache->getLevel(), cache->getPolicy().c_str(), cache->getHitRate());
    printf("Accesses: %lld\t\tHits: %lld\t\tMisses: %lld\n",
        cache->getHits()+cache->getMisses(), cache->getHits(), cache->getMisses());
    printf("\n");
}
