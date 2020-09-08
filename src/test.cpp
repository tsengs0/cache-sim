    #include<iostream> 
    using namespace std; 
    #define WORD_SIZE 8 // 8 bytes
unsigned int decToHex(char *hex_str, long long dec_val);
int main() 
{ 
	unsigned int msbDigit, lsbDigit;
	char *hex_str;
	long long sample[10] = {
		((long long) 1) << 5,
		((long long) 1) << 32,
		((long long) 1) << 16,
		((long long) 1) << 8,
		((long long) 1) << 4,

		((long long) 1) << 2,
		((long long) 1) << 5,
		((long long) 1) << 32,
		((long long) 1) << 16,
		((long long) 1) << 8
	};
	
	for(int i = 0; i < 10; i++) {
		cout << "Dec: " << sample[i] << "\t->\tHex: ";
		hex_str = new char[(WORD_SIZE*8) / 4]();
		msbDigit = decToHex(hex_str, sample[i]); 
		for(int j = msbDigit-1; j >= 0; j--) cout << hex_str[j];
		delete [] hex_str;
		cout << endl;
	}
	
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
