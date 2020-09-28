#include <iostream> 
#include <map>
#include <cmath>
#include "dram.h"
#include <ctime>
#include <vector>

using namespace std; 
#define MAX_TRANS_NUM 50

int main(int argc, char **argv) 
{ 
	srand(time(NULL));

	dram_system *mem_sys = new dram_system(8, ROW_ADDR_WIDTH, COL_ADDR_WIDTH, 1, 8);
	vector<unsigned int> addr;
	unsigned int mem_addr, addr_rand_max;
	unsigned int trans_num;

	addr_rand_max = (unsigned int) pow(2, 16); // 32-bit addr
	trans_num = rand() % MAX_TRANS_NUM; 	

	// Write-back
	for(unsigned int i = 0; i < trans_num; i++) {
		mem_addr = rand() % addr_rand_max;
		mem_addr = colAddr_burst_align(mem_addr);
		addr.push_back(mem_addr);
		burst_data cache_line = new long long[8]();		
		for(unsigned int k = 0; k < 8; k++) cache_line[k] = rand() % (unsigned int) pow(2, 20);
		//for(unsigned int j = 0; j < 8; j++) cout << hex << "0x" << cache_line[j] << "\t";
		//cout << endl << endl;
		cout << endl << "Write-Back Physical Address: " << hex << "0x" << mem_addr;
		mem_sys -> write_row(cache_line, mem_addr);
	}

	mem_sys -> content_show();	
	cout << "====================================================================================" << endl
	     << "====================================================================================" << endl;

	// Load
	for(unsigned int i = 0; i < trans_num; i++) {
		burst_data data_temp = new long long[8];
		mem_sys -> read_row(data_temp, addr[i]);

		cout << "Read Physical Address: " << hex << "0x" << addr[i] << endl;
		for(unsigned int j = 0; j < 8; j++) cout << dec << "(dec)_" << data_temp[j] << "\t";
		cout << endl << endl;
		delete [] data_temp;
	}
	mem_sys -> save_log_file();
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
