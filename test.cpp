#include <iostream>
#include <vector>
enum cache_fsm {
	IDLE        = 0,
	COMPARE_TAG = 1,
	WRITE_BACK  = 2,
	ALLOCATE    = 3
};
using namespace std;

typedef struct trans_package_t {
    short levelItr;
    long long address;
    long long *evictedBlock_val;
    long long rqst_word_val; // either read request or write request
    bool is_wr;

   trans_package_t(short a, long long b, long long *c, long long d, bool e) {
	levelItr = a;
	address = b;
	evictedBlock_val = c;
	rqst_word_val = d;
	is_wr = e;
   }
} trans_package;

void fsm(trans_package *trans);

int* allocate_sample(int i)
{
	int *temp = new int;
	*temp = i;
	return temp;
}

int main(int argc, char **argv)
{
	int a;

	for(int i = 0; i < 4; i++)
	{
		switch(i) {
			case cache_fsm::IDLE: std::cout << "IDLE" << std::endl;
			case cache_fsm::COMPARE_TAG: std::cout << "COMPARE TAG" << std::endl;
			case cache_fsm::WRITE_BACK: std::cout << "WRITE BACK" << std::endl;
			case cache_fsm::ALLOCATE: std::cout << "ALLOCATE" << std::endl;

		}
	}
	cout << endl;
	
	vector<int*> sample;
	vector<int*> &copy_sample = sample;
	vector<int*> *copy_sample_1; copy_sample_1 = &sample;
	for(int i = 0; i < 5; i++) {
		sample.push_back(allocate_sample(i));
		cout << "Allocate a memory space @ " << dec << sample[i] << endl;
	}
	cout << "Size of sample: " << sample.size() << endl;
	for(int i = 0; i < 5; i++) {
		cout << "Sample[" << i << "]: " << *(sample[i]) << " @ " << hex << sample[i] << endl;
	
	}
	cout << endl << "Size of duplicated sample: " << copy_sample.size() << endl;
	for(int i = 0; i < 5; i++) {
		cout << "Sample[" << i << "]: " << *(copy_sample[i]) << " @ " << hex << copy_sample[i] << endl;
	
	}
	cout << endl << "Size of duplicated sample 1: " << (*copy_sample_1).size() << endl;
	for(int i = 0; i < 5; i++) {
		cout << "Sample[" << i << "]: " << *((*copy_sample_1)[i]) << " @ " << hex << (*copy_sample_1)[i] << endl;
	
	}
	cout << endl << endl;
	long long *evictedBlock_val = new long long[8]();
	trans_package *trans = new trans_package(1, (long long) 0x12345, evictedBlock_val, (long long) 0x543, (bool) true);
	fsm(trans); delete [] trans;
	
	trans = new trans_package(1, (long long) 0x12345, evictedBlock_val, (long long) 0, (bool) false);
	cout << "Read Data: " << hex << trans -> rqst_word_val << endl;
	fsm(trans); 
	cout << "Read Data: " << hex << trans -> rqst_word_val << endl;
	delete [] trans;


	return 0;
}

void fsm(trans_package *trans)
{
    short levelItr;              levelItr = trans -> levelItr;
    long long address;           address = trans -> address;
    long long *evictedBlock_val; evictedBlock_val = trans -> evictedBlock_val;
    long long rqst_word_val;     rqst_word_val = trans -> rqst_word_val;
    bool is_wr;                  is_wr = trans -> is_wr;

	if(is_wr == true) { // Write Request
		cout << "Write Data: " << hex << rqst_word_val << endl;
	}
	else { // Read Request
		trans -> rqst_word_val = 0xffff;
	}
}
