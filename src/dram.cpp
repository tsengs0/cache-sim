#include "dram.h"

const char *wb_log_filename = "dram_wb.csv";
const char *rd_log_filename = "dram_rd.csv";
const char *traverse_filename = "traverse_content.txt";

dram_system::dram_system(unsigned short banks, unsigned short rows, unsigned short cols, unsigned short col_size, unsigned short bursts)
{
	bank_num = banks;
	rowAddr_bits = rows;
	colAddr_bits = cols;
	col_byteWise = col_size;
	burst_length = bursts;
	burst_offset = ceil(log2(burst_length));
	col_num = pow(2, colAddr_bits-burst_offset);
	nonZeroRow_cnt = new unsigned short[burst_length]();
	dram_matrix = new bank_matrix[burst_length];

	wb_log_file.open(wb_log_filename);
	rd_log_file.open(rd_log_filename);
	traverse_file.open(traverse_filename);
}

void dram_system::save_log_file(void)
{
	wb_log_file.close();
	rd_log_file.close();
	traverse_file.close();
}

void dram_system::write_row(long long *cache_block_val, long long address)
{
	dram_addr addr;
	addr = physical2dram_addr((unsigned int) address);
	//addr.col = addr.col / burst_length; 
	cout << endl << dec << "Write-Back Cache Block onto DRAM ==> B" << addr.bank << ", R" << addr.row << ", Col_" << addr.col << endl; 
	if(dram_matrix[addr.bank].empty()) {
		col_data temp = new burst_data[col_num];
		for(unsigned int i = 0; i < col_num; i++) temp[i] = new long long[burst_length]();

		(dram_matrix[addr.bank])[addr.row] = temp;
		for(unsigned int i = 0; i < burst_length; i++) { 
			cout << hex << "0x" << cache_block_val[i] << "\t";
			((dram_matrix[addr.bank])[addr.row])[addr.col][i] = cache_block_val[i];

			if(i < burst_length-1) wb_log_file << cache_block_val[i] << ",";
			else wb_log_file << cache_block_val[i] << endl;
		}
	}
	else if(dram_matrix[addr.bank].find(addr.row) != dram_matrix[addr.bank].end()) {
		// current bank is not empty
		// note that the size of "cache_block_val" must match value of "burst_length"
		(dram_matrix[addr.bank])[addr.row][addr.col] = new long long[burst_length];
		for(unsigned int i = 0; i < burst_length; i++) {
			cout << hex << "0x" << cache_block_val[i] << "\t";
			(dram_matrix[addr.bank])[addr.row][addr.col][i] = cache_block_val[i];
			
			if(i < burst_length-1) wb_log_file << cache_block_val[i] << ",";
			else wb_log_file << cache_block_val[i] << endl;
		}
	}
	else { // current bank is not empty but the specified row is not found
		// note that the size of "cache_block_val" must match value of "burst_length"
		(dram_matrix[addr.bank])[addr.row] = new burst_data[col_num];
		(dram_matrix[addr.bank])[addr.row][addr.col] = new long long[burst_length];
		for(unsigned int i = 0; i < burst_length; i++) {
			cout << hex << "0x" << cache_block_val[i] << "\t";
			(dram_matrix[addr.bank])[addr.row][addr.col][i] = cache_block_val[i];
			
			if(i < burst_length-1) wb_log_file << cache_block_val[i] << ",";
			else wb_log_file << cache_block_val[i] << endl;
		}
	}
	cout << endl;
}

void dram_system::read_row(long long *cache_block_val, long long address)
{
	dram_addr addr;
	addr = physical2dram_addr((unsigned int) address);
	//addr.col = addr.col / burst_length; 
	if(dram_matrix[addr.bank].empty()) {
		(dram_matrix[addr.bank])[addr.row] = new burst_data[col_num];
		for(unsigned int i = 0; i < burst_length; i++)
			(dram_matrix[addr.bank])[addr.row][addr.col][i] = 0;
	
		// note that the size of "cache_block_val" must match value of "burst_length"
		for(unsigned int i = 0; i < burst_length; i++) {
			cache_block_val[i] = 0;

			if(i < burst_length-1) rd_log_file << hex << cache_block_val[i] << ",";
			else rd_log_file << cache_block_val[i] << endl;
		}

		cout << "The request data does not exist in this bank, an all-zero cacheline is thereby returned (as dummy)" << endl;
	}
	else if(dram_matrix[addr.bank].find(addr.row) == dram_matrix[addr.bank].end()) {
		(dram_matrix[addr.bank])[addr.row] = new burst_data[col_num];
		(dram_matrix[addr.bank])[addr.row][addr.col] = new long long[burst_length]();
	
		// note that the size of "cache_block_val" must match value of "burst_length"
		for(unsigned int i = 0; i < burst_length; i++) { 
			cache_block_val[i] = 0;

			if(i < burst_length-1) rd_log_file << cache_block_val[i] << ",";
			else rd_log_file << cache_block_val[i] << endl;
		}
		cout << "The request data does not exist in this row, an all-zero cacheline is thereby returned (as dummy)" << endl;
	}
	else { // the bank is not empty and the row does exist
		// note that the size of "cache_block_val" must match value of "burst_length"
		for(unsigned int i = 0; i < burst_length; i++) {
			cache_block_val[i] = (dram_matrix[addr.bank])[addr.row][addr.col][i];
			
			if(i < burst_length-1) rd_log_file << cache_block_val[i] << ",";
			else rd_log_file << cache_block_val[i] << endl;
		}
	}
}

void dram_system::content_show()
{

	for(unsigned int i = 0; i < bank_num; i++) {
	  if(!dram_matrix[i].empty()) {
	  	cout << dec << "Bank " << i << ":" << endl;
	  	for(auto it = dram_matrix[i].begin(); it != dram_matrix[i].end(); it++) {
	    		cout << dec << "Row " << it -> first << ": " << endl;
	    		col_data &temp = it -> second;
	    		for(unsigned int k = 0; k < col_num; k++) {
	      			cout << dec << "Col " << k << " ----> ";
	      			for(unsigned int l = 0; l < burst_length; l++) {
					cout << hex << "0x" << temp[k][l] << "\t";
	      			}
	      			cout << endl;
	    		}
	    		cout << endl;
	  	}
	  }
	}
}
