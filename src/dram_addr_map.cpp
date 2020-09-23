#include "dram.h"

dram_addr dram_system::physical2dram_addr(unsigned int addr)
{
	unsigned short BA[BANK_ADDR_WIDTH];
	dram_addr temp;


	// Extract the Cache line Byte-Offset
	addr = addr >> CACHE_LINE_WIDTH;

	// Translate column address
	temp.col = addr & ((unsigned short) pow(2, COL_ADDR_WIDTH)-1);

	// Translate row address
	temp.row = addr >> (BANK_ADDR_WIDTH+COL_ADDR_WIDTH);

	// Translate bank address
	temp.bank = (addr >> COL_ADDR_WIDTH) & ((unsigned short) pow(2, BANK_ADDR_WIDTH)-1);	
	unsigned short row_temp = temp.row >> 1;
	for(int i = 0; i < BANK_ADDR_WIDTH; i++) 
		BA[i] = (row_temp & 1) ^ ((temp.bank >> i) & 1);
	
	temp.bank = 0;
	for(int i = 0; i < BANK_ADDR_WIDTH; i++) 
		temp.bank += BA[i] << i;

	return temp;
}

unsigned int colAddr_burst_align(unsigned int addr)
{
	unsigned int temp;
	temp = addr;

	// Extract the Cache line Byte-Offset
	temp = temp >> CACHE_LINE_WIDTH;

	// Translate column address
	temp = temp  & ((unsigned short) pow(2, COL_ADDR_WIDTH)-1);
	
	// To align the column address with respect to the burst length
	temp = temp / BL;

	addr &= ~(((unsigned int) pow(2, COL_ADDR_WIDTH)-1) << CACHE_LINE_WIDTH);
	addr |= temp << CACHE_LINE_WIDTH;
	return addr;
}
