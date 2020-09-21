#include <iostream> 
#include <map>
#include <cmath>
#include "dram.h"
#include <ctime>
using namespace std; 

int main(int argc, char **argv) 
{ 
	srand(time(NULL));

	dram_system *mem_sys = new dram_system(8, ROW_ADDR_WIDTH, COL_ADDR_WIDTH, 1, 8);

	// Write-back
	for(unsigned int i = 0; i < 10; i++) {
		unsigned int mem_addr = rand() % (unsigned int) pow(2, 16); // 32-bit address
		burst_data cache_line = new long long[8]();		
		for(unsigned int k = 0; k < 8; k++) cache_line[k] = rand() % (unsigned int) pow(2, 20);
		//for(unsigned int j = 0; j < 8; j++) cout << hex << "0x" << cache_line[j] << "\t";
		//cout << endl << endl;
		mem_sys -> write_row(cache_line, mem_addr);
	}

	mem_sys -> content_show();	
	
	return 0;
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
