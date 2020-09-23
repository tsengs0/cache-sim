#ifndef __DRAM_H
#define __DRAM_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
using namespace std;

#define ROW_ADDR_WIDTH 15
#define BANK_ADDR_WIDTH 3
#define COL_ADDR_WIDTH 10
#define CACHE_LINE_WIDTH 6
#define BL 8

typedef struct dram_addr_t {
	unsigned int row;
	unsigned int bank;
	unsigned int col;
} dram_addr;

typedef long long* burst_data;
typedef burst_data* col_data;
typedef map<unsigned short, col_data> bank_matrix; // Key: row index; Value: row content. (for one bank)
typedef bank_matrix* dram_2d_matrix; // to construct a bank vector	

unsigned int colAddr_burst_align(unsigned int addr);
class dram_system {
	private:
		dram_2d_matrix dram_matrix;
		unsigned short bank_num;
		unsigned short rowAddr_bits; // the bit-width of row address
		unsigned short col_byteWise; // the bit-width to address the size of one row at byte-wise unit
		unsigned short burst_length;
		unsigned short *nonZeroRow_cnt;
		unsigned short burst_offset;
		unsigned short col_num;
		unsigned short colAddr_bits;
		ofstream wb_log_file, rd_log_file, traverse_file;

	public:
		dram_system(unsigned short banks, unsigned short rows, unsigned short cols, unsigned short col_size, unsigned short bursts);

		dram_addr physical2dram_addr(unsigned int addr);
		void write_row(long long *cache_block_val, long long address);
		void read_row(long long *cache_block_val, long long address);
		void content_show(void);
		void save_log_file(void);
};

#endif // __DRAM_H
